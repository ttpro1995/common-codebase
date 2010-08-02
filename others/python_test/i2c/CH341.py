# coding=cp936
'''
*******************************************************************************
 Desc    : USB to I2C (CH341) Driver.
 Note    : N/A
 History : 2010/08/02      Panda.Xiong       Create
*******************************************************************************
'''

from ctypes import *
import sys


mCH341A_CMD_I2C_STREAM     = 0xAA
mCH341A_CMD_I2C_STM_STA    = 0x74
mCH341A_CMD_I2C_STM_END    = 0x00
mCH341A_CMD_I2C_STM_STO    = 0x75
mCH341A_CMD_I2C_STM_OUT    = 0x80

mCH341_DEVICE_ARRIVAL      = 3       # 设备插入事件,已经插入
mCH341_DEVICE_REMOVE_PEND  = 1       # 设备将要拔出
mCH341_DEVICE_REMOVE       = 0       # 设备拔出事件,已经拔出

mCH341_USB_RATE_20K        = 0
mCH341_USB_RATE_100K       = 1
mCH341_USB_RATE_400K       = 2
mCH341_USB_RATE_750K       = 3

mCH341_OUT_SCL_BIT         = 18
mCH341_OUT_SDA_BIT         = 19
mCH341_IN_SDA_BIT          = 23



class CH341:
    'I2C USB-CH341 Driver'

    def _assert(self, sErrMsg):
        print sErrMsg
        raw_input('\nPress "Enter" key to exit ...')
        sys.exit(-1)


    def __init__(self,
                 sName         = 'USB_CH341',
                 vUsbCH341ID   = 0,
                 vUsbCH341Mode = mCH341_USB_RATE_100K
                 ):
        'Init I2C USB-CH341 Driver'

        self.sName = sName

        self._vUsbCH341ID   = vUsbCH341ID
        self._vUsbCH341Mode = vUsbCH341Mode

        self._vPrevStatus   = (1<<mCH341_OUT_SCL_BIT) | (1<<mCH341_OUT_SDA_BIT)


    def IsMasterAvaliable(self):
        'Check this I2C Master is available or not'

        if self.Open() == False:
            return False

        self.Close()

        return True


    def Open(self):
        'Open I2C USB-CH341 Driver'

        # try to load CH341 dll
        try:
            self._ch341 = windll.LoadLibrary('CH341DLL.DLL')
        except:
            self._assert('\nLoad "CH341DLL.DLL" fail!')

        # close device first
        self._ch341.CH341CloseDevice(self._vUsbCH341ID)

        # open device
        if self._ch341.CH341OpenDevice(self._vUsbCH341ID) == -1:
            return False

        # set USB rate
        if self._ch341.CH341SetStream(self._vUsbCH341ID, self._vUsbCH341Mode) == False:
            return False

        # set callback API, for CH341 event handling
        NOTIFY_FUNC = CFUNCTYPE(c_ulong)
        _callback   = NOTIFY_FUNC(self.NotifyRoutine)
        self._ch341.CH341SetDeviceNotify(self._vUsbCH341ID, None, _callback)

        return True


    def Close(self):
        'Close I2C USB-CH341 Driver'

        # close device first
        self._ch341.CH341CloseDevice(self._vUsbCH341ID)
        del self._ch341


    def SetSDA(self, vData):
        'Set SDA for USB-CH341 Driver'

        if ((self._vPrevStatus >> mCH341_OUT_SDA_BIT) & 1) != vData:
            # Note: set bit 4 to 1, to enable bit16-23 output
            vEnableBit = (1 << 4) | (1 << mCH341_OUT_SDA_BIT)

            if vData != 0:
                self._vPrevStatus |= (1 << mCH341_OUT_SDA_BIT)
            else:
                self._vPrevStatus &= ~(1 << mCH341_OUT_SDA_BIT)

            self._ch341.CH341SetOutput(self._vUsbCH341ID, vEnableBit, 0, self._vPrevStatus)


    def SetSCL(self, vData):
        'Set SCL for USB-CH341 Driver'

        if ((self._vPrevStatus >> mCH341_OUT_SCL_BIT) & 1) != vData:
            # Note: set bit 4 to 1, to enable bit16-23 output
            vEnableBit = (1 << 4) | (1 << mCH341_OUT_SCL_BIT)

            if vData != 0:
                self._vPrevStatus |= (1 << mCH341_OUT_SCL_BIT)
            else:
                self._vPrevStatus &= ~(1 << mCH341_OUT_SCL_BIT)

            self._ch341.CH341SetOutput(self._vUsbCH341ID, vEnableBit, 0, self._vPrevStatus)


    def GetSDA(self, vData):
        'Get SDA for USB-CH341 Driver'

        vStatus = c_ulong(0)

        if self._ch341.CH341GetInput(self._vUsbCH341ID, addressof(vStatus)) == False:
            return 1

        return (vStatus.value >> mCH341_IN_SDA_BIT) & 1


    def GetSCL(self, vData):
        'Get SCL for USB-CH341 Driver'

        # currently, has no way to read the SCL line level,
        #  so, only return the OUTPUT SCL line level.
        return (vStatus._vPrevStatus >> mCH341_OUT_SCL_BIT) & 1


    def NotifyRoutine(self, vEventStatus):
        'CH341 notify Routine'

        if vEventStatus == mCH341_DEVICE_ARRIVAL:
            # open device
            if self._ch341.CH341OpenDevice(self._vUsbCH341ID) == -1:
                self._assert('NotifyRoutine: CH341OpenDevice failed!')

            # set USB rate
            if self._ch341.CH341SetStream(self._vUsbCH341ID, self._vUsbCH341Mode) == False:
                self._assert('NotifyRoutine: CH341SetStream failed!')

        elif vEventStatus == mCH341_DEVICE_REMOVE:
            self._ch341.CH341CloseDevice(self._vUsbCH341ID)
        else:
            pass   # do nothing


    def RandomRead(self, vI2cAddr, vOffset, vReadLen):
        'I2C Random read one or more bytes'

        if self.Detect(vI2cAddr) == False:
            return (False, [])

        # prepare read/write buffer
        aWriteBuf = (c_ubyte * 2)()
        aReadBuf  = (c_ubyte * vReadLen)()

        # sending Addr+W & Offset
        aWriteBuf[0] = (vI2cAddr & 0xFE)
        aWriteBuf[1] = vOffset
        vStatus = self._ch341.CH341StreamI2C(self._vUsbCH341ID, 2, byref(aWriteBuf), 0, byref(aReadBuf))
        if vStatus == 0:
            return (False, aReadBuf)

        # sending Addr+R & read
        aWriteBuf[0] = (vI2cAddr | 0x01)
        vStatus = self._ch341.CH341StreamI2C(self._vUsbCH341ID, 1, byref(aWriteBuf), vReadLen, byref(aReadBuf))
        if vStatus == 0:
            return (False, aReadBuf)
        else:
            return (True, aReadBuf)


    def RandomWrite(self, vI2cAddr, vOffset, aBuf):
        'I2C Random write one or more bytes'

        if self.Detect(vI2cAddr) == False:
            return False

        # prepare read/write buffer
        vWriteLen = len(aBuf)
        vTotalLen = 2 + vWriteLen
        aWriteBuf = (c_ubyte * vTotalLen)()
        aReadBuf  = (c_ubyte * 1)()

        # sending Addr+W & offset & write
        aWriteBuf[0] = (vI2cAddr & 0xFE)
        aWriteBuf[1] = vOffset
        for i in range(vWriteLen):
            aWriteBuf[2+i] = aBuf[i]

        vStatus = self._ch341.CH341StreamI2C(self._vUsbCH341ID, vTotalLen, byref(aWriteBuf), 0, byref(aReadBuf))
        if vStatus == 0:
            return False
        else:
            return True


    def CurrentRead(self, vI2cAddr, vReadLen):
        'I2C Current read one or more bytes'

        if self.Detect(vI2cAddr) == False:
            return (False, [])

        # prepare read/write buffer
        aWriteBuf = (c_ubyte * 1)()
        aReadBuf  = (c_ubyte * vReadLen)()

        # sending Addr+W
        aWriteBuf[0] = (vI2cAddr | 0x01)
        vStatus = self._ch341.CH341StreamI2C(self._vUsbCH341ID, 1, byref(aWriteBuf), vReadLen, byref(aReadBuf))
        if vStatus == 0:
            return (False, aReadBuf)
        else:
            return (True, aReadBuf)


    def CurrentWrite(self, vI2cAddr, aBuf):
        'I2C Current write one or more bytes'

        if self.Detect(vI2cAddr) == False:
            return False

        # prepare read/write buffer
        vWriteLen = len(aBuf)
        vTotalLen = 1 + vWriteLen
        aWriteBuf = (c_ubyte * vTotalLen)()
        aReadBuf  = (c_ubyte * 1)()

        # sending Addr+W
        aWriteBuf[0] = (vI2cAddr & 0xFE)
        for i in range(vWriteLen):
            aWriteBuf[1+i] = aBuf[i]

        vStatus = self._ch341.CH341StreamI2C(self._vUsbCH341ID, vTotalLen, byref(aWriteBuf), 0, byref(aReadBuf))
        if vStatus == 0:
            return False
        else:
            return True


    def Detect(self, vI2cAddr):
        'I2C Detect Slave'

        # prepare read/write buffer
        vLen      = c_ulong(0)
        aWriteBuf = (c_ubyte * 6)()

        aWriteBuf[0] = mCH341A_CMD_I2C_STREAM
        aWriteBuf[1] = mCH341A_CMD_I2C_STM_STA
        aWriteBuf[2] = mCH341A_CMD_I2C_STM_OUT
        aWriteBuf[3] = (vI2cAddr & 0xFE)
        aWriteBuf[4] = mCH341A_CMD_I2C_STM_STO
        aWriteBuf[5] = mCH341A_CMD_I2C_STM_END

        vStatus = self._ch341.CH341WriteRead(self._vUsbCH341ID, 6, byref(aWriteBuf), 4, 1, addressof(vLen), byref(aWriteBuf))
        if vStatus:
            if (vLen.value != 0) and ((aWriteBuf[vLen.value-1] & 0x80) == 0):
                return True

        return False



'auto-test scripts'
if __name__ == '__main__':
    print ' ---  Test Start  ---\n'

    ch341 = CH341()

    print ' --> Name : %s' % ch341.sName,
    if ch341.IsMasterAvaliable():
        print '(Avaliable)'
    else:
        print '(Not Avaliable)'

    ch341.Open()


    print '\n All I2C Slaves :'
    aI2cAddrTable = []
    for vAddr in range(0x00, 0xFF, 2):
        if ch341.Detect(vAddr):
            print '  --> 0x%02X' % vAddr
            aI2cAddrTable.append(vAddr)

    for i in range(len(aI2cAddrTable)):
        vAddr = aI2cAddrTable[i]
        print '\n Read %02X.00-FF :' % vAddr,
        (vResult, aReadBuf) = ch341.RandomRead(vAddr, 0x00, 0x100)
        if vResult == True:
            for i in range(len(aReadBuf)):
                if i%16 == 0:
                    print '\n  %02X.%02X :' % (vAddr, i),
                elif i%8 == 0:
                    print '-',
                print '%02X' % aReadBuf[i],
            print
        else:
            print ' --- Read Fail!'

        if vAddr == 0xA0:
            print '\n Random Write 0xA0 --------',
            if ch341.RandomWrite(vAddr, 0x00, aReadBuf) == False:
                print 'Fail!'
            else:
                print 'Done!'

    for i in range(len(aI2cAddrTable)):
        vAddr = aI2cAddrTable[i]
        print '\n Read %02X.00-FF :' % vAddr,
        (vResult, aReadBuf) = ch341.CurrentRead(vAddr, 0x100)
        if vResult == True:
            for i in range(len(aReadBuf)):
                if i%16 == 0:
                    print '\n  %02X.%02X :' % (vAddr, i),
                elif i%8 == 0:
                    print '-',
                print '%02X' % aReadBuf[i],
            print
        else:
            print ' --- Read Fail!'

        if vAddr == 0xA0:
            print '\n Current Write 0xA0 --------',
            if ch341.CurrentWrite(vAddr, aReadBuf) == False:
                print 'Fail!'
            else:
                print 'Done!'


    ch341.Close()

    print '\n ---  Test  End   ---'
