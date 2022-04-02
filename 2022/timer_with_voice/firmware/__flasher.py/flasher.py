import os
import struct
import sys
import time
import zlib
import serial

BAUDRATE = 500000
READ_PACKET_SIZE = 512
WRITE_PACKET_SIZE = 512
ERASE_PACKET_SIZE = 32768
BLOCK_SIZE = 4096
LIMIT_256K = False 
MAX_FILES = 48

vendors = {
    #     0x01: ("AMD", "AM"),    # AM29 series only
    0x01: ("Spansion / Cypress", "S"),
    0x04: ("Fujitsu", "MBM"),
    0x1c: ("EON Silicon Devices", "EN"),
    0x1f: ("Atmel / Adesto", "AT"),
    0x20: ("ST", "M"),
    0x31: ("Catalyst", "CAT"),
    0x37: ("AMIC", "A"),
    0x40: ("SyncMOS / MVC", "V"),
    0x4a: ("Excel Semiconductor Inc.", "ES"),
    0x52: ("Alliance Semiconductor", "AS"),
    0x5e: ("Tenx Technologies", "ICE"),
    0x62: ("Sanyo / ON Semiconductor", "LE"),
    0x89: ("Intel", ""),
    0x85: ("Puya Semiconductor", "P"),
    0x8c: ("Elite Semiconductor", "F"),
    0x97: ("Texas Instruments", "TMS"),
    0x9d: ("PMC", "PM"),
    0xa1: ("Fudan Microelectronics", "FM"),
    0xa2: ("Sharp", "LH"),
    0xa6: ("Sharp", "LH"),
    #     0xad: ("Bright Microelectronics", "BM"),    #BM9 series only
    #     0xad: ("Hyundai", "HY"),
    0xb0: ("Sharp", "LH"),
    0xbf: ("SST", "SST"),
    0xc2: ("Macronix", "MX"),
    0xc8: ("GigaDevice", "GD"),
    #     0xd5: ("ISSI", "IS"),    # IS29 series only
    0xd5: ("Nantronics", "N"),
    0xef: ("Winbond", "W"),
    0xf8: ("Fidelix", "FM")
}


def CalcLRC(msg):
    """ 计算LRC校验和, LRC为除LRC外所有字节之和取1补码; 带LRC的字节串按字节求和后低字节为零 """
    lrc = 0
    for c in msg:
        lrc += c
    lrc = lrc % 256
    lrc = 256 - lrc
    if lrc == 256:
        lrc = 0
    return lrc


def AddLRC(msg):
    """ 在msg结尾附加校验和 """
    return msg + CalcLRC(msg).to_bytes(1, byteorder='little')


def ViewMsg(msg):
    return
    """ for debug """
    for i in msg:
        print("%02x " % i, end="")
    print()


def scan():
    """ 检测可用串口 """
    ports = []
    for i in range(32):
        try:
            port = 'COM' + str(i + 1)
            s = serial.Serial(port)
            ports.append(port)
            s.close()
        except serial.SerialException:
            pass
    return ports


def Connect(port, baudrate):
    try:
        ser = serial.Serial(port, baudrate, timeout=0.01)
        return ser
    except serial.SerialException:
        raise serial.SerialException


class Cli:

    def __init__(self, ser, addr):
        self.ser = ser
        self.addr = addr

    def SendCmd(self, cmd):
        """ 发送命令, 读取全部返回数据 """
        ViewMsg(cmd)  # for debug
        self.ser.write(cmd)
        timeout = time.time() + 1.5
        while self.ser.in_waiting == 0 and time.time() < timeout:
            time.sleep(0.03)
        ret = self.ser.read_all()
        ViewMsg(ret)  # for debug
        return ret

    def SendCmd2(self, cmd, size):
        """ 发送命令, 按指定size读取返回数据 """
        self.ser.write(cmd)
        time.sleep(0.1)
        ret = self.ser.read(size)
        print(len(ret))
        return ret

    def __ReadCRC32(self, addr, size):
        """ 按指定地址和长度计算crc32校验值 """
        msg = b'\x92\x00\x00\x00' + size.to_bytes(4, byteorder='little')
        msg += addr.to_bytes(4, byteorder='little')
        ViewMsg(msg)
        msg = AddLRC(msg)
        ViewMsg(msg)
        ret = self.SendCmd(msg)
        ViewMsg(ret)

        return struct.unpack('L', ret)[0]

    def __Read(self, addr, total_size, show_progress):
        """ 按指定地址和长度读取数据 """
        size = READ_PACKET_SIZE
        ret = b''
        count = 0
        total_count = total_size // size
        if total_count == 0:
            total_count = 1

        while total_size > 0:
            if total_size < size:
                size = total_size
            cmd = b'\x9c\x00' + size.to_bytes(2, byteorder='little')
            cmd += addr.to_bytes(4, byteorder='little')
            cmd = AddLRC(cmd)
            for i in range(10):
                msg = self.SendCmd(cmd)
                if len(msg) == size:
                    break
            else:
                for i in cmd:
                    print("%02x" % i, end=" ")
                print()
                print(len(msg), size)
                print("Communication Error!")
                exit(1)
            ret += msg
            sys.stdout.flush()
            total_size -= size
            addr += size
            count += 1
            if show_progress == True:
                print('\rReading %d%%...' %
                      (count * 100 / total_count), end='')
                sys.stdout.flush()
        if show_progress == True:
            print()
        return ret

    def ReadFile(self):
        """ 读取文件 """
        if len(sys.argv) != 3:
            print("Usage: flasher.py -r filename")
            exit()
        filename = sys.argv[2]
        file_list = self.__GetFileList()
        for fname, addr, size, timestr, crc32 in file_list:
            if fname == filename:
                break
        else:
            print("%s not found in flash." % filename)
            return
        if os.path.exists(filename):
            print("%s already exists, overwrite? (y/n)" % filename, end="")
            while True:
                ans = input()
                if ans == "y":
                    break
                elif ans == "n":
                    return

        content = self.__Read(addr, size, True)
        f = open(filename, mode='wb')
        f.write(content)
        f.close()

    def __ReadInfo(self):
        msg = b'\x90' + b'\0'
        msg = AddLRC(msg)
        msg = self.SendCmd(msg)
        msg = msg[:-1]
        if LIMIT_256K:
            msg = b'\x12' + msg[1:]  # debug
        return struct.unpack('BBB', msg)

    def __Erase(self, addr, total_size, show_progress):  # 内部实现
        """ 按指定地址和长度擦除 """
        ret = b''
        count = 0
        size = ERASE_PACKET_SIZE
        if show_progress == True:
            total_count = total_size // size
            if total_size % size != 0:
                total_count += 1
            if total_count == 0:
                total_count = 1
        while total_size > 0:
            if total_size < size:
                size = total_size
            msg = b'\x9d\x00' + \
                size.to_bytes(2, byteorder='little') + \
                addr.to_bytes(4, byteorder='little')
            msg = AddLRC(msg)
            ret += self.SendCmd(msg)
            total_size -= size
            addr += size
            count += 1
            if show_progress == True:
                print('\rErasing %d%%...' %
                      (count * 100 / total_count), end='')
                sys.stdout.flush()
        print()

    def __Write(self, content, addr, show_progress):
        """ 按指定content和地址写入 """

        size = WRITE_PACKET_SIZE
        count = 0
        total_count = len(content) // size
        if total_count == 0:
            total_count = 1

        while len(content) > 0:  # transfer
            pb = content[0:size]
            msg = b'\x9e\x00' + \
                len(pb).to_bytes(2, byteorder='little') + \
                addr.to_bytes(4, byteorder='little') + pb
            msg = AddLRC(msg)

            if pb != (b'\xff' * size):  # 全0xff则跳过
                while True:
                    ret = self.SendCmd(msg)
                    if len(ret) > 0:
                        break
            content = content[size:]
            addr += len(pb)
            if content == (b'\xff' * len(content)):
                break

            count += 1
            if show_progress == True:
                print('\rWriting %d%%...' %
                      (count * 100 / total_count), end='')
                sys.stdout.flush()
        print()

    def ReadInfo(self):
        """ 读取flash信息 """
        capacity, flashid, vendor_id = self.__ReadInfo()
        print("Manufacturer: %s  Capacity: %dKB" %
              (vendors[vendor_id][0], 2 ** (capacity - 10)))

    def __CheckBlockEmpty(self, addr):
        """ 检查指定地址所在块是否为空块 """
        msg = b'\x91\x00\x00\x00' + addr.to_bytes(4, byteorder='little')
        msg = AddLRC(msg)
        ret = self.SendCmd(msg)
        if ret[0] == 1:
            return True
        else:
            return False

    def UsageRaw(self):
        """ 列出块物理使用情况 """
        capacity, flashid, vendor_id = self.__ReadInfo()
        block_count = 2 ** capacity // BLOCK_SIZE
        print("Capacity: %dKB    Number of blocks: %d" %
              (2 ** capacity // 1024, block_count))
        print("-: Empty block    X: Data block    R: Reserved block")
        for i in range(block_count):
            if i % 64 == 0:
                print("%06X-%06X: " %
                      (i * BLOCK_SIZE, (i + 64) * BLOCK_SIZE - 1), end="")
            if i == block_count - 1:
                print("R", end="")
            elif self.__CheckBlockEmpty(i * BLOCK_SIZE) == True:
                print("-", end="")
            else:
                print("X", end="")
            sys.stdout.flush()
            if i % 64 == 63:
                print()
        print()

    def Usage(self):
        """ 列出块逻辑使用情况 """
        capacity, flashid, vendor_id = self.__ReadInfo()
        block_count = 2 ** capacity // BLOCK_SIZE
        print("Capacity: %dKB    Number of blocks: %d" %
              (2 ** capacity // 1024, block_count))
        print("-: Empty block    X: Data block    R: Reserved block")
        block_lists = self.__FindAvailableSpace()
        block_lists.sort(key=lambda x: x[0])
        for i in range(block_count):
            if i % 64 == 0:
                print("%06X-%06X: " %
                      (i * BLOCK_SIZE, (i + 64) * BLOCK_SIZE - 1), end="")
            if i == block_count - 1:
                print("R", end="")
            else:
                for j in block_lists:
                    if i >= j[0] and i < j[1]:
                        print("-", end="")
                        break
                else:
                    print("X", end="")
            sys.stdout.flush()
            if i % 64 == 63:
                print()
        print()

    def EraseFile(self):
        """ 按指定地址和长度擦除 """
        if len(sys.argv) != 4:
            print("err1")
            exit()
        try:
            addr = int(sys.argv[2])
            total_size = int(sys.argv[3])
        except:
            print("err2")
            exit()
        self.__Erase(addr, total_size, True)

    def __GetMetaSectorAddr(self):
        """ 读取信息块 """
        capacity, flashid, vendor_id = self.__ReadInfo()
        block_count = 2 ** capacity // BLOCK_SIZE
        return (block_count - 1) * BLOCK_SIZE

    def __WriteFileMeta(self, fname, addr, size, timestamp, crc32):
        """ 需要写入的信息: 文件名、起始位置、大小、时间戳、CRC32
        # fname:40B;  addr:4B;  size:4B;  timestamp:4B;  crc32:4B; reserved:8B
        # total: 64B;   total_file_count: 64  需要预先读取保留区
        """
        meta_addr = self.__GetMetaSectorAddr()
        meta_sector = self.__Read(meta_addr, BLOCK_SIZE, False)
        for i in range(64):
            meta_item = meta_sector[i * 64:(i + 1) * 64]
            if meta_item == b'\xff' * 64:  # 找到空位置
                break
        else:  # meta区已写满
            print("rotating...")
            for j in range(64):
                meta_item = meta_sector[j * 64:(j + 1) * 64]
                if meta_item[0] == 0:
                    meta_sector = meta_sector[:j * 64] + \
                        b'\xff' * 64 + meta_sector[(j + 1) * 64:]
                    i = j  # 记录最后一个空位置
            self.__Erase(meta_addr, BLOCK_SIZE, False)
            self.__Write(meta_sector, meta_addr, False)

        meta = fname.encode(encoding='utf-8')
        if len(meta) > 40:
            meta = meta[:40]
        if len(meta) < 40:
            meta += b'\xff' * (40 - len(meta))
        meta += struct.pack('LLLL', addr, size, timestamp, crc32)
        meta += b'\xff' * 8
        self.__Write(meta, meta_addr + i * 64, False)

    def DeleteFile(self):
        """ 删除文件 """
        if len(sys.argv) != 3:  # 指定地址写入
            print("Usage: flasher.py -d filename")
            return
        fname = sys.argv[2]

        meta_sector = self.__Read(
            self.__GetMetaSectorAddr(), BLOCK_SIZE, False)
        for i in range(64):
            meta_item = meta_sector[i * 64:(i + 1) * 64]
            if meta_item == b'\xff' * 64:
                continue
            if meta_item[0] == 0:
                continue
            meta_fname = meta_item[:40]
            while meta_fname[-1] == 0xff:
                meta_fname = meta_fname[:-1]
            if fname == meta_fname.decode(encoding='utf-8'):
                self.__Write(
                    b'\x00\x00', self.__GetMetaSectorAddr() + i * 64, False)
                print("%s deleted." % fname)
                return
        print("Can't find %s" % fname)

    def __GetFileList(self):
        """ 从信息块读取文件列表 """
        meta_sector = self.__Read(
            self.__GetMetaSectorAddr(), BLOCK_SIZE, False)
        file_list = []
        for i in range(64):
            meta_item = meta_sector[i * 64:(i + 1) * 64]
            if meta_item == b'\xff' * 64:
                continue
            if meta_item[0] == 0:
                continue
            meta_fname = meta_item[:40]
            while meta_fname[-1] == 0xff:
                meta_fname = meta_fname[:-1]
            fname = meta_fname.decode(encoding='utf-8')
            meta_item = meta_item[40:56]
            addr, size, timestamp, crc32 = struct.unpack('LLLL', meta_item)
            timestr = time.strftime(
                "%Y/%m/%d %H:%M:%S", time.localtime(timestamp))
            file_list.append((fname, addr, size, timestr, crc32))
        return file_list

    def ListFile(self):
        """ 列出文件 """
        print("%-20s  %-8s    %-8s  %-20s  %-8s" %
              ("Filename", "Addr", "Size", "Date/Time", "CRC32"))
        total_size = 0
        file_list = self.__GetFileList()
        for fname, addr, size, timestr, crc32 in file_list:
            print("%-20s  0x%08x  %-8d  %-20s  0x%08x" %
                  (fname, addr, size, timestr, crc32))
            total_size += size
        print()
        block_lists = self.__FindAvailableSpace()
        available_blocks = 0
        max_continuous_blocks = 0
        for i in block_lists:
            blocks = i[1] - i[0]
            available_blocks += blocks
            if blocks > max_continuous_blocks:
                max_continuous_blocks = blocks
        print("\t%d files, %d bytes, " % (len(file_list), total_size), end=" ")
        print("%d bytes free, " % (available_blocks * BLOCK_SIZE), end=" ")
        print("%d bytes continuous" % (max_continuous_blocks * BLOCK_SIZE))

    def Check(self):
        """ 计算所有文件的CRC32校验值, 与信息块中储存的CRC32校验值对比 """
        total_size = 0
        file_list = self.__GetFileList()
        for fname, addr, size, timestr, crc32 in file_list:
            crc32_actual = self.__ReadCRC32(addr, size)
            if crc32 == crc32_actual:
                print("%s stored: %08x actual: %08x crc32 ok!" % (fname, crc32, crc32_actual))
            else:
                print("%s stored: %08x actual: %08x crc32 mismatch!" % (fname, crc32, crc32_actual))

    def __SplitAvailableSpace(self, block_lists, gap):
        """ 检查可用空间 """
        if gap[1] < gap[0]:
            return
        for i in block_lists:
            if gap[0] < i[0]:
                continue
            if gap[1] > i[1]:
                continue
            if gap[0] == i[0] and gap[1] == i[1]:
                block_lists.remove(i)
                return
            if gap[0] == i[0] and gap[1] < i[1]:
                block_lists.remove(i)
                block_lists.insert(0, (gap[1], i[1]))
            if gap[1] == i[1] and gap[0] > i[0]:
                block_lists.remove(i)
                block_lists.insert(0, (i[0], gap[0]))
            if gap[0] > i[0] and gap[1] < i[1]:
                block_lists.remove(i)
                block_lists.insert(0, (gap[1], i[1]))
                block_lists.insert(0, (i[0], gap[0]))
        pass

    def __FindAvailableSpace(self):
        """ 寻找可用空间 """
        meta_sector = self.__Read(
            self.__GetMetaSectorAddr(), BLOCK_SIZE, False)
        capacity, flashid, vendor_id = self.__ReadInfo()
        block_count = 2 ** capacity // BLOCK_SIZE
        block_lists = [(0, block_count - 1)]
        for i in range(64):
            meta_item = meta_sector[i * 64:(i + 1) * 64]
            if meta_item == b'\xff' * 64:
                continue
            if meta_item[0] == 0:
                continue
            meta_item = meta_item[40:56]
            addr, size, timestamp, crc32 = struct.unpack('LLLL', meta_item)
            block_head = addr // BLOCK_SIZE
            block_num = (size - 1) // BLOCK_SIZE + 1
            self.__SplitAvailableSpace(
                block_lists, (block_head, block_head + block_num))
        block_lists.sort(key=lambda x: x[1] - x[0])
        return block_lists

    def WriteFile(self):
        """ 写入文件 """
        if len(sys.argv) < 3 or len(sys.argv) > 4:
            print("Usage: flasher.py -w filename [addr]")
            exit()
        try:
            path = sys.argv[2]
        except:
            print("err2")
            exit()
        try:
            size = os.path.getsize(path)
        except:
            print("Failed to open file", path)
            exit()

        block_lists = self.__FindAvailableSpace()
        addr = 0
        if len(sys.argv) == 4:  # 指定地址写入
            addr = int(sys.argv[3])
            addr &= 0xfffff000
            head_block = addr // 4096
            block_count = (size - 1) // BLOCK_SIZE + 1
            for i in block_lists:
                if head_block >= i[0] and block_count <= i[1] - head_block:
                    break
            else:
                print("No available space for writing.")
                return

        else:
            for i in block_lists:
                if size < (i[1] - i[0]) * BLOCK_SIZE:
                    addr = i[0] * BLOCK_SIZE
                    break
            else:
                print("Not enough continuous space for writing.")
                return
        file_list = self.__GetFileList()
        fname = os.path.basename(path)
        for i in file_list:
            if fname == i[0]:
                print("%s already in flash." % fname)
                return
        if len(file_list) >= MAX_FILES:
            print("Can't write more than %d files." % MAX_FILES)
            return

        self.__Erase(addr, size, True)
        f = open(path, mode='rb')
        content = f.read()
        f.close()
        self.__Write(content, addr, True)
        orig_crc32 = zlib.crc32(content)
        if orig_crc32 == self.__ReadCRC32(addr, size):
            print("Verifying... OK!")
            timestamp = int(time.time())
            self.__WriteFileMeta(fname, addr, size, timestamp, orig_crc32)
        else:
            print("Verifying... Failed!")
            print("orig_crc32: %08x  written_crc32: %08x" %
                  (orig_crc32, self.__ReadCRC32(addr, size)))


if __name__ == '__main__':
    # 以下用于单元测试
    baudrate = BAUDRATE
    ports = scan()
    if len(ports) == 0:
        print("No usable port detected.")
        exit()

    try:
        ser = Connect(ports[0], baudrate)
    except:
        print("port in use, exit")
        exit()

    print("Connected, Port=%s, baudrate=%d" % (ports[0], baudrate))
    print()

    cli = Cli(ser, 0x1)
    actions = {
        "-i": (cli.ReadInfo, "Read flash information"),
        "-r": (cli.ReadFile, "Read a file"),
        "-l": (cli.ListFile, "List files"),
        "-d": (cli.DeleteFile, "Delete a file"),
        "-c": (cli.Check, "Check files"),
        "-u": (cli.Usage, "Read blocks usage"),
        "-ur": (cli.UsageRaw, "Read blocks usage (raw)"),
        "-e": (cli.EraseFile, "Erase from addr"),
        "-w": (cli.WriteFile, "Write file"),
    }

    if len(sys.argv) == 1:
        print("Usage: %s -opts [file] [addr]" % os.path.basename(sys.argv[0]))
        for action in actions:
            print("    %s: %s" % (action, actions[action][1]))
        exit()
    else:
        action = sys.argv[1]
    if action in actions:
        actions[action][0]()

    ser.close()
