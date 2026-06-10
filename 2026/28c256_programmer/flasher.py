# coding: utf-8
import serial
import time
import sys
import zlib
import platform
import argparse
import os

#DEBUG = {"view_cmd": True}
DEBUG = {"view_cmd": False}

class FLASHER:
    def __init__(self, ser):
        self.ser = ser
        self.rom_size = 0
        self.max_packet_size = 2048

    def crc32(self, msg): 
        return zlib.crc32(msg).to_bytes(4, byteorder="little")
    
    def send_cmd(self, cmd, expected_len=None, timeout=3.5):
        if DEBUG["view_cmd"]:
            print("CMD: ", " ".join(f"{b:02x}" for b in cmd[:26]), f"len: {len(cmd)}")

        self.ser.reset_input_buffer()
        self.ser.write(cmd)

        ret = b""
        start_time = time.time()
        last_receive_time = time.time()
        
        while time.time() - start_time < timeout:
            if self.ser.in_waiting > 0:
                ret += self.ser.read_all()
                last_receive_time = time.time()
                if expected_len is not None and len(ret) >= expected_len:
                    break
            else:
                if len(ret) > 0 and (time.time() - last_receive_time > 0.05):
                    break
                time.sleep(0.01)
                
        if DEBUG["view_cmd"]:
            print("RET: ", ret)
        return ret

    def get_rom_info(self):
        """ 獲取ROM總容量 (0xa6) 與單包最大長度 (0xa9) """
        msg1 = b'\xa0\xa6\x08\x00'
        msg1 += self.crc32(msg1)
        ret1 = self.send_cmd(msg1, expected_len=4, timeout=1.0)
        if len(ret1) >= 4:
            self.rom_size = int.from_bytes(ret1[:4], byteorder='little')
            print(f"ROM Total Size: {self.rom_size} Bytes")
        
        msg2 = b'\xa0\xa9\x08\x00'
        msg2 += self.crc32(msg2)
        ret2 = self.send_cmd(msg2, expected_len=4, timeout=1.0)
        if ret2 != None:
            self.max_packet_size = int.from_bytes(ret2[:4], byteorder='little')
            print(f"Max Packet Size: {self.max_packet_size} Bytes")

    def test(self):
        for _ in range(3):
            if self.send_cmd(b'## test', timeout=0.1) != b'':
                return True
        return False

    def read_app_crc32(self, pos, size):
        msg = b'\xa0\xa2\x0c\x00'
        msg += size.to_bytes(2, byteorder="little")
        msg += pos.to_bytes(2, byteorder="little")
        msg += self.crc32(msg)
        ret = self.send_cmd(msg, expected_len=4)
        if len(ret) < 4:
            print("Error: Failed to get CRC32.")
            return None
        return int.from_bytes(ret[:4], byteorder="little")

    def read_image(self, pos, size, filename):
        total_to_read = size
        current_pos = pos
        chunk_size = self.max_packet_size if self.max_packet_size > 0 else 2048
        result_bytes = b''

        while total_to_read > 0:
            read_len = min(total_to_read, chunk_size)
            msg = b'\xa0\xac\x0c\x00'
            msg += read_len.to_bytes(2, byteorder='little')
            msg += current_pos.to_bytes(2, byteorder='little')
            msg += self.crc32(msg)
            
            ret = self.send_cmd(msg, expected_len=read_len)
            if len(ret) == 0:
                print(f"Error: Timeout at pos {current_pos}")
                return False
            
            result_bytes += ret[:read_len]
            total_to_read -= read_len
            current_pos += read_len
            print(f"Reading: {(len(result_bytes)/size)*100:.0f}%", end='\r')
        
        try:
            with open(filename, 'wb') as f:
                f.write(result_bytes)
            print(f"\nSaved to file: {filename}")
            return True
        except Exception as e:
            print(f"\nError saving file: {e}")
            return False

    def erase_image(self):
        msg = b'\xa0\xaf\x08\x00'
        msg += self.crc32(msg)
        return self.send_cmd(msg, timeout=10.0)

    def write_image(self, pos, filename):
        try:
            with open(filename, mode='rb') as f:
                img = f.read()
        except Exception as e:
            print(f"Failed to open file: {e}")
            return False

        if self.rom_size > 0 and (pos + len(img) > self.rom_size):
            allowed_len = self.rom_size - pos
            if allowed_len <= 0:
                print("Error: Position out of bounds.")
                return False
            print(f"Warning: File truncated to {allowed_len} bytes.")
            img = img[:allowed_len]

        totalsize = len(img)
        current_pos = pos
        chunk_size = self.max_packet_size if self.max_packet_size > 0 else 2048

        print("### %d %d" % (self.max_packet_size, chunk_size))

        while len(img) % 4 != 0:
            img += b'\xff'

        bytes_sent = 0
        while bytes_sent < totalsize:
            pb = img[bytes_sent : bytes_sent + chunk_size]
            actual_chunk_len = len(pb)

            cmd_len = 12 + actual_chunk_len
            msg = b'\xa0\xae' + cmd_len.to_bytes(2, byteorder='little')
            msg += actual_chunk_len.to_bytes(2, byteorder='little')
            msg += current_pos.to_bytes(2, byteorder='little')
            msg += pb
            msg += self.crc32(msg)

            if self.send_cmd(msg, expected_len=1, timeout=5.0) == b'':
                print(f"\nTime out at pos {current_pos}")
                return False

            bytes_sent += actual_chunk_len
            current_pos += actual_chunk_len
            print(f"Writing: {(bytes_sent/totalsize)*100:.0f}%", end='\r')
            sys.stdout.flush()

        print('\n100% Write Completed.')
        return True

def scan(baudrate=None):
    import glob
    ports = []
    
    if baudrate is not None:
        baudrates = [int(baudrate)]
    else:
        baudrates = [1500000]
    
    if platform.system() == 'Linux':
        ports.extend(glob.glob('/dev/ttyU*'))
        ports.extend(glob.glob('/dev/ttyA*'))
    elif platform.system() == 'Windows':
        for i in range(32):
            try:
                port = f"COM{i + 1}"
                s = serial.Serial(port)
                ports.append(port)
                s.close()
            except serial.SerialException:
                pass
    
    for port in ports:
        for baud in baudrates:
            try:
                ser = serial.Serial(port, baud, timeout=0.05)
                a = FLASHER(ser)
                if a.test():
                    ser.close()
                    return port, baud
                ser.close()
            except Exception:
                continue
    return None, None

def main():
    parser = argparse.ArgumentParser(description="AT28CXX EEPROM CLI")
    parser.add_argument('-b', '--baudrate', type=int, default=1500000)
    parser.add_argument('-s', '--serial', dest='port', default=None)
    
    subparsers = parser.add_subparsers(dest='action', help='Available commands')

    p_crc = subparsers.add_parser('crc32', help='Calculate CRC32 of device memory')
    p_crc.add_argument('pos', type=lambda x: int(x, 0), help='Starting position offset')
    p_crc.add_argument('size', type=lambda x: int(x, 0), help='Size in bytes')

    p_read = subparsers.add_parser('read', help='Read device memory to file')
    p_read.add_argument('pos', type=lambda x: int(x, 0), help='Starting position offset')
    p_read.add_argument('size', type=lambda x: int(x, 0), help='Size in bytes')
    p_read.add_argument('filename', help='Output filename')

    subparsers.add_parser('erase', help='Erase entire chip')

    p_write = subparsers.add_parser('write', help='Write file content into device memory')
    p_write.add_argument('pos', type=lambda x: int(x, 0), help='Starting position offset')
    p_write.add_argument('filename', help='Input filename')

    # 【新增】如果使用者未傳入任何參數（或只有 -b / -s 卻沒輸入命令），顯示幫助訊息
    if len(sys.argv) == 1 or (len(sys.argv) > 1 and sys.argv[-1] in [None, '']) or '-h' in sys.argv or '--help' in sys.argv:
        parser.print_help()
        sys.exit(0)

    args = parser.parse_args()

    # 【新增】此處雙重檢查：如果解析出的 action 為 None，提示錯誤並列出子命令幫助 [1]
    if args.action is None:
        parser.print_help()
        sys.exit(1)

    if args.port is None:
        port, baudrate = scan(baudrate=args.baudrate)
        if port is None:
            print("No device detected. Please use -s to specify port.")
            sys.exit(1)
        ser = serial.Serial(port, baudrate)
    else:
        ser = serial.Serial(args.port, args.baudrate, timeout=0.2)

    flasher = FLASHER(ser)
    print("Connecting to device...")
    flasher.get_rom_info()

    if args.action == 'crc32':
        crc = flasher.read_app_crc32(args.pos, args.size)
        if crc is not None:
            print(f"Device CRC32: {crc:08x}")
    elif args.action == 'read':
        flasher.read_image(args.pos, args.size, args.filename)
    elif args.action == 'erase':
        flasher.erase_image()
        print("Erase completed.")
    elif args.action == 'write':
        flasher.write_image(args.pos, args.filename)

    ser.close()

if __name__ == "__main__":
    main()

