'''
*******************************************************************************
 Desc    : A script used to Load I2C Library.
 Note    : N/A
 History : 2010/07/15      Panda.Xiong      Create
*******************************************************************************
'''


import sys
import string
import os
import time
import LPTB
import LPTA
import CH341
import QRIB



class I2C:
    'I2C Abstract Layer'

    def _assert(self, message):
        print message
        raw_input('\nPress "Enter" key to exit ...')
        sys.exit(-1)


    def __init__(self, write_delay=0.0):    # default, no write delay time
        'Init I2C Library'

        # Set Write Delay Timing
        self.i2c_write_delay_ms = write_delay

        self._CurrentI2cMaster = None

        # refresh all avaliable I2C Master
        self.vAvaliableMasterTable = {}
        self.RefreshMaster()

        # select first avaliable I2C Master
        if len(self.vAvaliableMasterTable) > 0:
            self.SelectMaster(self.vAvaliableMasterTable.keys()[0])


    def RefreshMaster(self):
        'Refresh all I2C Master'

        # All supported I2C Master Table
        self._vAllI2cMasterTable = [LPTB.LPTB(),
                                    LPTA.LPTA(),
                                    CH341.CH341(),
                                    QRIB.QRIB(vWriteDelayTime = self.i2c_write_delay_ms)]

        # scan all avaliable I2C Master
        self.vAvaliableMasterTable = {}
        for i in self._vAllI2cMasterTable:
            if i.IsMasterAvaliable() == True:
                self.vAvaliableMasterTable[i.sName] = i


    def SelectMaster(self, vMasterName):
        'Select I2C Master'

        # close previous openned I2C Master
        if self._CurrentI2cMaster != None:
            self._CurrentI2cMaster.Close()

        if self.vAvaliableMasterTable.has_key(vMasterName):
            # open the selected I2C Master
            self._CurrentI2cMaster = self.vAvaliableMasterTable[vMasterName]
            self._CurrentI2cMaster.Open()
            self.i2c_host = self._CurrentI2cMaster.sName
            return True
        else:
            self.i2c_host = '(Empty)'
            return False


    def GetSDA(self):
        'Get SDA of current selected I2C Master'

        return self._CurrentI2cMaster.GetSDA()


    def GetSCL(self):
        'Get SCL of current selected I2C Master'

        return self._CurrentI2cMaster.GetSCL()


    def SetSDA(self, vData):
        'Set SDA of current selected I2C Master'

        self._CurrentI2cMaster.SetSDA(vData)


    def SetSCL(self, vData):
        'Set SCL of current selected I2C Master'

        self._CurrentI2cMaster.SetSCL(vData)


    def Start(self):
        'Generate I2C Start'

        self._CurrentI2cMaster.Start()


    def Stop(self):
        'Generate I2C Stop'

        self._CurrentI2cMaster.Stop()


    def CurrentRead(self, i2c_addr, read_len):
        'I2C Current Read Byte(s)'

        return self._CurrentI2cMaster.CurrentRead(i2c_addr, read_len)


    def CurrentWrite(self, i2c_addr, write_buf):
        'I2C Current Write Byte(s)'

        vLength   = len(write_buf)
        aWriteBuf = []
        for i in range(vLength):
            if type(write_buf[i]) == type('a'):
                aWriteBuf.append(ord(write_buf[i]))
            else:
                aWriteBuf.append(write_buf[i])

        vResult = self._CurrentI2cMaster.CurrentWrite(i2c_addr, aWriteBuf)
        time.sleep(self.i2c_write_delay_ms)
        return vResult


    def RandomRead(self, i2c_addr, start_offset, read_len):
        'I2C Random Read Byte(s)'

        return self._CurrentI2cMaster.RandomRead(i2c_addr, start_offset, read_len)


    def RandomWrite(self, i2c_addr, start_offset, write_buf):
        'I2C Random Write Byte(s)'

        vLength   = len(write_buf)
        aWriteBuf = []
        for i in range(vLength):
            if type(write_buf[i]) == type('a'):
                aWriteBuf.append(ord(write_buf[i]))
            else:
                aWriteBuf.append(write_buf[i])

        vResult = self._CurrentI2cMaster.RandomWrite(i2c_addr, start_offset, aWriteBuf)
        time.sleep(self.i2c_write_delay_ms)
        return vResult


    def WriteByte(self, i2c_addr, offset, value):
        'I2C Random Write Byte'

        aWriteBuf = []
        if type(value) == type('a'):
            aWriteBuf.append(ord(value))
        else:
            aWriteBuf.append(value)

        return self.RandomWrite(i2c_addr, offset, aWriteBuf)


    def ReadByte(self, i2c_addr, offset):
        'I2C Random Read Byte'

        (vResult, aReadBuf) = self.RandomRead(i2c_addr, offset, 1)
        if not vResult:
            return -1
        else:
            return aReadBuf[0]


    def Detect(self, i2c_addr):
        'I2C Detect Address'

        return self._CurrentI2cMaster.Detect(i2c_addr)



'auto-test scripts'
if __name__ == '__main__':
    i2c_dll = I2C()

    if len(i2c_dll.vAvaliableMasterTable) == 0:
        i2c_dll._assert('No Avaliable I2C Master!')

    print ' All I2C Masters :'
    for i in i2c_dll.vAvaliableMasterTable.keys():
        if i == i2c_dll.i2c_host:
            print '  --> %s (Active)' % i
        else:
            print '  --> %s' % i

    i2c_dll.Start()
    i2c_dll.Stop()

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
