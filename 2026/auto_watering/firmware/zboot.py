# coding: utf-8

import serial
import time
#import os
import sys
import bincopy 
import zlib
import platform
import argparse

DEBUG = dict()
#DEBUG["view_cmd"] = True #False
DEBUG["view_cmd"] = False

class ZBOOT:
    def __init__(self, ser): self.ser = ser

    def crc32(self, msg): return zlib.crc32(msg).to_bytes(4, byteorder="little")
    def send_cmd(self, cmd, expected_len=None, timeout=1.5):

        if DEBUG["view_cmd"] is True:
            print("CMD: ", end="")
            count = 0
            for i in cmd:
                print("%02x " % i, end="")
                count += 1
                if count > 16: break
            print("len: ", len(cmd))

        self.ser.reset_input_buffer()
        
        self.ser.write(cmd)
        #print("initial timeout: ", self.ser.timeout)

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
                # 關鍵：如果已經有數據，且 50ms 沒新數據，判定為多行結束
                if len(ret) > 0 and (time.time() - last_receive_time > 0.05):
                    break
                time.sleep(0.01)
                
        if DEBUG["view_cmd"] is True:
            print("RET: ", ret)
        return ret

    def get_max_app_size(self):
        # 0x80 读取APP可写入的区域大小
        cmd = b'\x80\x80\x08\x00'
        cmd += self.crc32(cmd)
        ret = self.send_cmd(cmd)
        return int.from_bytes(ret, byteorder='little')

    def get_app_size(self):
        # 0x82 读取APP区域实际大小, 向上取整到2字节
        cmd = b'\x82\x00\x7e'
        ret = self.send_cmd(cmd)
        return int.from_bytes(ret, byteorder='little')
    
    def check_empty(self):
        # 0x86 判断APP区是否为空
        cmd = b'\x80\x86\x08\x00'
        cmd += self.crc32(cmd)
        ret = self.send_cmd(cmd)
        return True if ret == b'\x00' else False

    def get_max_packet_size(self):
        # 0x88 读取最大包尺寸
        cmd = b'\x80\x88\x08\x00'
        cmd += self.crc32(cmd)
        ret = self.send_cmd(cmd)
        return int.from_bytes(ret, byteorder='little')

    def read_image(self, bin_size):
        # 0x81 读取APP区域 81 00 size(4字节) pos(4字节) 校验(1字节)
        size = 256
        pos = 0
        ret = b''
        count = 0
        while bin_size > 0:
            if bin_size < size:
                size = bin_size
            msg = b'\x81\x00' + size.to_bytes(4, byteorder='little')
            msg += pos.to_bytes(4, byteorder='little')
            msg += self.crc32(msg)
            ret += self.send_cmd(msg)
            bin_size -= size
            pos += size
            count += 1
            print(count)
        return ret

    def erase_image(self, size):
        # 0x84 擦除APP区域
        msg = b'\x80\x84\x08\x00'
        msg += self.crc32(msg)
        ret = self.send_cmd(msg, expected_len=1, timeout=10)
        return ret

    def read_app_crc32(self, ofs, size):
        # 0x83 计算APP区域CRC32校验值
        msg = b'\x80\x83\x10\x00'
        msg += ofs.to_bytes(4, byteorder="little")
        msg += size.to_bytes(4, byteorder="little")
        msg += self.crc32(msg)
        ret = self.send_cmd(msg)
        return int.from_bytes(ret, byteorder="little")

    def write_image(self, img):
        # 0x85 写入APP区域 

        size = self.get_max_packet_size()
        count = 0
        pos = 0

        totalsize = len(img)
        while len(img) % 4 != 0:
            img += b'\xff'
        while len(img) > 0:  # transfer
            sys.stdout.flush()
            perc = (1 - len(img) / totalsize) * 100
            sys.stdout.write('%.0f%%       ' % perc)
            sys.stdout.flush()
            sys.stdout.write('\r')
            sys.stdout.flush()
            pb = img[0:size]
            msg = b'\x80\x85' + (len(pb) + 12).to_bytes(2, byteorder='little') + pos.to_bytes(4, byteorder='little') + pb
            msg += self.crc32(msg)
            count += 1
            if pb != (b'\xff' * size):  # 全0xff则跳过
                ret = self.send_cmd(msg, expected_len=1, timeout=2.0)
                if ret == b'':
                    print("Time out, check configurations!")
                    exit()

            img = img[size:]
            pos += len(pb)
            if img == (b'\xff' * len(img)):  # 全0xff则跳过剩余部分
                break
        print('100%')

    def jump_to_bootloader(self):
        msg = b'## reboot'
        self.send_cmd(msg)

    def jump_to_app(self):
        # 0x87 跳转至APP区域
        msg = b'\x80\x87\x08\x00'
        msg += self.crc32(msg)
        self.send_cmd(msg)

    def sysinfo(self):
        return self.send_cmd(b'## sysinfo').decode('ascii')

    def test(self):
        for i in range(3):
            ret = self.send_cmd(b'## test')
            if ret != b'':
                return True
        return False

def scan(baudrate=None):
    '''scan for available ports '''
    ports = []
    baudrates = [1500000] if baudrate is None else [baudrate]
    if platform.system() == 'Linux':
        ports.extend(glob('/dev/ttyU*'))  # ttyUSB*
        ports.extend(glob('/dev/ttyA*'))  # ttyACM*
    elif platform.system() == 'Windows':
        for i in range(32):
            try:
                port = 'COM' + str(i + 1)
                s = serial.Serial(port)
                ports.append(port)
                s.close()
            except serial.SerialException:
                pass
    else:
        raise Exception('Unsupported platform: {}'.format(platform.system()))
    #print(ports)
    for port in ports:
        for baudrate in baudrates:
            try:
                print('Probe for {} @ {}bps...'.format(port, baudrate))
                ser = serial.Serial(port, baudrate, timeout=0.05)
            except serial.SerialException as e:
                print('Serial error:', e)
                break
            except Exception:
                continue
            a = ZBOOT(ser)
            if a.test():
                ser.close()
                return port, baudrate
            ser.close()

    return None, None

def get_args():
    parser = argparse.ArgumentParser("zboot.py")
    parser.add_argument('filename', nargs='?', 
            help='firmware binary/ihex file to be downloaded.  if omitted, just query sysinfo.')
    parser.add_argument('-s', '--serial', dest='port', nargs='?', default=None,
            help='Name of serial port. "COM*" in windows, "/dev/ttyUSB*" in linux. Will auto scan if omitted. This is the default mode.')
    parser.add_argument('-b', '--baudrate', nargs='?', 
            help='Override baudrate, 500k bps by default.')
    parser.add_argument('-T', '--type', nargs='?', 
            help='Override input file type, hex file by default, bin file option')
    parser.add_argument('-F', '--func', nargs='?', help=argparse.SUPPRESS)
    return parser.parse_args(), parser

def handle(ser, args):

    file = args.filename
    file_type = args.type

    zboot = ZBOOT(ser)
    #print("###", file, file_type)
    if file == "test":  # unit test
        print("test!")
        print(args.func)
        code = int(args.func, 16)
        if code == 0x80:
            zboot.max_space = zboot.get_max_app_size()
            print(zboot.max_space)
        exit(0)
    if file == None:
        print(zboot.sysinfo())
        exit(0)

    if file_type == "bin":
        print("firmware file type is:", file_type)
        try:
            f = open(file, mode='rb')
        except:
            print("Failed to open binary file.")
            exit()
        img = f.read()
        f.close()
    else:
        print("firmware file type is ihex")
        try:
            f = bincopy.BinFile(file)
        except:
            print("Failed to open ihex file.")
            exit()
        img = f.as_binary()

    orig_len = len(img)

    print('Jumping to bootloader...')
    sys.stdout.flush()
    zboot.jump_to_bootloader()
    time.sleep(0.1)
    
    max_space = zboot.get_max_app_size()
    if orig_len > max_space:
        print("Insufficient space! %lu bytes needed, %lu bytes available." % (orig_len, max_space))
        exit(0)

    if zboot.check_empty() == False:
        print('Erasing...')
        sys.stdout.flush()
        zboot.erase_image(len(img))

    print('Writing...')
    sys.stdout.flush()
    zboot.write_image(img)

    print('Verifying...')
    sys.stdout.flush()
    img = img[:orig_len]
    crc = zboot.read_app_crc32(0, orig_len)
    crc2 = zlib.crc32(img)
    print('Written %d bytes.' % len(img))
    print('Original crc32: %08x' % crc2)
    print('Written crc32:  %08x' % crc)

    sys.stdout.flush()
    if crc == crc2:
        zboot.jump_to_app()
        print('crc match, starting app.')
    else:
        print("crc mismatch!")
    ser.close()


def main():
    args, parser = get_args()

    if args.port is None:       # auto scan!
        port, baudrate = scan(baudrate=args.baudrate)
        ser = serial.Serial(port, baudrate) if port is not None else None
    else:                       # check
        ser = serial.Serial(args.port, args.baudrate, timeout=0.2)
        zboot = ZBOOT(ser)
        if not zboot.test():
            ser.close()
            ser = None
            print("Device not detected")
        else:
            pass #print("test pass")

    if ser is not None:
        handle(ser, args)
        ser.close()

if __name__ == "__main__":
    main()
