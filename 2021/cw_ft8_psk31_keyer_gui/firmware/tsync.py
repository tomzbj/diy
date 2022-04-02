import serial
import time
import os
import sys
import bincopy 

#VIEWCFG = True
VIEWCFG = False

class Cli: 
    def __init__(self, ser):
        self.ser = ser

    def send_ascii_cmd(self, cmd):

        if VIEWCFG == True:
            print(cmd)
        self.ser.write(cmd)
        timeout = time.time() + 1.5
        while self.ser.in_waiting == 0 and time.time() < timeout:
            time.sleep(0.02)
        time.sleep(0.02)
        ret = self.ser.read_all()
        if VIEWCFG == True:
            print("RET: ", ret)
        return ret

def tsync(port, baudrate):
    ser = serial.Serial(port, baudrate, timeout=0.2)
    a = Cli(ser)
#    ms = int(time.time() * 1000) % (86400 * 1000)
    t = int(time.time())
    ret = a.send_ascii_cmd(b"## wr time %lu" % t)
    ret = a.send_ascii_cmd(b"## rd time");
    print(ret)

def scan():
    '''scan for available ports '''
    ports = []
    baudrates = [500000]
    for i in range(32):
        try:
            port = 'COM' + str(i + 1)
            s = serial.Serial(port)
            ports.append(port)
            s.close()
        except serial.SerialException:
            pass
    print(ports)
    for port in ports:
        for baudrate in baudrates:
            try:
                ser = serial.Serial(port, baudrate, timeout=0.05)
            except:
                continue
            a = Cli(ser)
            for i in range(3):
                ret = a.send_ascii_cmd(b'## test')
                if ret != b'':
                    return port, baudrate
            ser.close()
 
    return None, None 

if __name__ == '__main__':
    port, baudrate = scan() 
    if port == None:
        print('Serial port not deteced.')
        exit()
    
    tsync(port, baudrate)
