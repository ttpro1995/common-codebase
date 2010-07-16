'''
*******************************************************************************
 Desc    : A script used to Load I2C Library.
 Note    : N/A
 History : 2010/07/15      Panda.Xiong      Create
*******************************************************************************
'''


from ctypes import *
import sys
import string
import os
import time


class I2C:
    def _assert(self, message):
        print 'Error: ' + message
        sys.exit(-1)


    def __init__(self):
        'Init I2C Library'

        # load I2C Library dll
        try:
            self.i2c = cdll.LoadLibrary('lib_i2c.dll')
        except EOFError:
            self._assert('\nLoad I2C Library Fail!')

        # I2C host type definition
        I2cHostType = {
                        1: 'LPTA',           \
                        2: 'LPTB',           \
                        3: 'USB-CH341'       \
                      }

        # Init I2C Library
        self.i2c_host = self.i2c.Init()
        if self.i2c_host in I2cHostType:
            self.i2c_host = I2cHostType[self.i2c_host]
        else:
            self._assert('No Valid I2C Host Detected!')

        # Get I2C Library Version
        dll_version = self.i2c.GetDllVersion()
        self.i2c_dll_version = 'v%d.%d.%d.%d'                      \
                               % ((dll_version >> 24) & 0xFF,      \
                                  (dll_version >> 16) & 0xFF,      \
                                  (dll_version >>  8) & 0xFF,      \
                                  (dll_version >>  0) & 0xFF)

        # Set Write Delay Timing
        self.i2c_write_delay_ms = 0.2


    def CurrentRead(self, i2c_addr, read_len):
        'I2C Current Read Byte(s)'

        aReadBuf = (c_ubyte * read_len)()
        vResult  = self.i2c.CurrentReadBytes(i2c_addr, read_len, byref(aReadBuf))

        return (vResult, aReadBuf)


    def CurrentWrite(self, i2c_addr, write_buf):
        'I2C Current Write Byte(s)'

        vLength   = len(write_buf)
        aWriteBuf = (c_ubyte * vLength)()
        for i in range(vLength):
            aWriteBuf[i] = write_buf[i]

        vResult = self.i2c.CurrentWriteBytes(i2c_addr, vLength, byref(aWriteBuf))
        time.sleep(self.i2c_write_delay_ms)
        return vResult


    def RandomRead(self, i2c_addr, start_offset, read_len):
        'I2C Random Read Byte(s)'

        aReadBuf = (c_ubyte * read_len)()
        vResult  = self.i2c.RandomReadBytes(i2c_addr, start_offset, read_len, byref(aReadBuf))

        return (vResult, aReadBuf)


    def RandomWrite(self, i2c_addr, start_offset, write_buf):
        'I2C Random Write Byte(s)'

        vLength   = len(write_buf)
        aWriteBuf = (c_ubyte * vLength)()
        for i in range(vLength):
            aWriteBuf[i] = write_buf[i]

        vResult = self.i2c.RandomWriteBytes(i2c_addr, start_offset, vLength, byref(aWriteBuf))
        time.sleep(self.i2c_write_delay_ms)
        return vResult


    def Detect(self, i2c_addr):
        'I2C Detect Address'

        return self.i2c.Detect(i2c_addr)



'auto-test scripts'
if __name__ == '__main__':
    i2c_dll = I2C()
    print ' I2C Host    :', i2c_dll.i2c_host
    print ' DLL Version :', i2c_dll.i2c_dll_version

    print '\n All I2C Slaves :'
    aI2cAddrTable = []
    for vAddr in range(0x00, 0xFF, 2):
        if i2c_dll.Detect(vAddr):
            print '  --> 0x%02X' % vAddr
            aI2cAddrTable.append(vAddr)

    for i in range(len(aI2cAddrTable)):
        vAddr = aI2cAddrTable[i]
        print '\n Read %02X.00-FF :' % vAddr,
        (vResult, aReadBuf) = i2c_dll.RandomRead(vAddr, 0x00, 0x100)
        if vResult:
            for i in range(len(aReadBuf)):
                if i%16 == 0:
                    print '\n  %02X.%02X :' % (vAddr, i),
                elif i%8 == 0:
                    print '-',
                print '%02X' % aReadBuf[i],
            print
        else:
            print ' --- Read Fail!'
