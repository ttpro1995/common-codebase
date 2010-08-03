'''
*******************************************************************************
 Desc    : USB to I2C (QRIB) Driver.
 Note    : N/A
 History : 2010/08/02      Panda.Xiong       Create
*******************************************************************************
'''


from ctypes import *
import sys


# Masks for the serial number and description
SI_RETURN_SERIAL_NUMBER   = 0x00
SI_RETURN_DESCRIPTION     = 0x01


# Masks for return values from the device
SI_SUCCESS                = 0x00
SI_DEVICE_NOT_FOUND       = 0xFF
SI_INVALID_HANDLE         = 0x01
SI_READ_ERROR             = 0x02
SI_RX_QUEUE_NOT_READY     = 0x03
SI_WRITE_ERROR            = 0x04
SI_RESET_ERROR            = 0x05
SI_INVALID_BUFFER         = 0x06
SI_INVALID_REQUEST_LENGTH = 0x07
SI_DEVICE_IO_FAILED       = 0x08

SI_QUEUE_NO_OVERRUN       = 0x00
SI_QUEUE_OVERRUN          = 0x01
SI_QUEUE_READY            = 0x02

SI_MAX_DEVICE_STRLEN      = 256
SI_MAX_READ_SIZE          = 64
SI_MAX_WRITE_SIZE         = 64

INVALID_HANDLE_VALUE      = 0x1

MAX_PACKET_SIZE           = 0x40
MAX_WRITE_PKTS            = 0x08

FT_READ_MSG               = 0x00
FT_WRITE_MSG              = 0x01
FT_READ_ACK               = 0x02
FT_MSG_SIZE               = 0x03


class USB_PKT_T(Structure):
    _fields_ = [
                ('vMagic',     c_ushort),
                ('vDir',       c_ubyte),
                ('vClass',     c_ubyte),
                ('vResult',    c_ubyte),
                ('vCookieLen', c_ubyte),
                ('aCookie',    c_ubyte * (64-6))
               ]


class QRIB:
    'USB to I2C (QRIB) Driver'

    def _assert(self, sErrMsg):
        print sErrMsg
        raw_input('\nPress "Enter" key to exit ...')
        sys.exit(-1)


    def __init__(self,
                 sName           = 'USB_QRIB',
                 vReadTimeoutMs  = 200,
                 vWriteTimeoutMs = 200
                 ):
        'Init I2C USB-QRIB Driver'

        self.sName = sName

        self._vReadTimeoutMs  = vReadTimeoutMs
        self._vWriteTimeoutMs = vWriteTimeoutMs

        self._pHandle = c_ulong(0)


    def IsMasterAvaliable(self):
        'Check this I2C Master is available or not'

        if self.Open() == False:
            return False

        self.Close()

        return True


    def Open(self):
        'Open I2C USB-QRIB Driver'

        # try to load QRIB dll
        try:
            self._qrib = windll.LoadLibrary('SiUSBXp.dll')
        except:
            self._assert('\nLoad "SiUSBXp.dll" fail!')

        if self._qrib.SI_Open(0, byref(self._pHandle)) != SI_SUCCESS:
            return False

        # Set read/write timeout value
        if self._qrib.SI_SetTimeouts(self._vReadTimeoutMs, self._vWriteTimeoutMs) != SI_SUCCESS:
            return False

        # Flush read/write buffer
        if self._qrib.SI_FlushBuffers(self._pHandle, 0, 0) != SI_SUCCESS:
            return False

        return True


    def Close(self):
        'Close I2C USB-QRIB Driver'

        # Flush read/write buffer
        if self._qrib.SI_FlushBuffers(self._pHandle, 0, 0) != SI_SUCCESS:
            return False

        self._qrib.SI_Close(self._pHandle)
        del self._qrib

        return True


    def Start(self):
        'Generate I2C Start'

        pass


    def Stop(self):
        'Generate I2C Stop'

        pass


    def RandomRead(self, vI2cAddr, vOffset, vReadLen):
        'I2C Random read one or more bytes'

        return (False, [])


    def RandomWrite(self, vI2cAddr, vOffset, aBuf):
        'I2C Random write one or more bytes'

        return False


    def CurrentRead(self, vI2cAddr, vReadLen):
        'I2C Current read one or more bytes'

        return (False, [])


    def CurrentWrite(self, vI2cAddr, aBuf):
        'I2C Current write one or more bytes'

        return False


    def Detect(self, vI2cAddr):
        'I2C Detect Slave'

        return False




'auto-test scripts'
if __name__ == '__main__':
    print ' ---  Test Start  ---\n'

    qrib = QRIB()

    print ' --> Name : %s' % qrib.sName,
    if qrib.IsMasterAvaliable():
        print '(Avaliable)'
    else:
        print '(Not Avaliable)'

    qrib.Open()


    print '\n All I2C Slaves :'
    aI2cAddrTable = []
    for vAddr in range(0x00, 0xFF, 2):
        if qrib.Detect(vAddr):
            print '  --> 0x%02X' % vAddr
            aI2cAddrTable.append(vAddr)

    for i in range(len(aI2cAddrTable)):
        vAddr = aI2cAddrTable[i]
        print '\n Read %02X.00-FF :' % vAddr,
        (vResult, aReadBuf) = qrib.RandomRead(vAddr, 0x00, 0x100)
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
            if qrib.RandomWrite(vAddr, 0x00, aReadBuf) == False:
                print 'Fail!'
            else:
                print 'Done!'

    for i in range(len(aI2cAddrTable)):
        vAddr = aI2cAddrTable[i]
        print '\n Read %02X.00-FF :' % vAddr,
        (vResult, aReadBuf) = qrib.CurrentRead(vAddr, 0x100)
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
            if qrib.CurrentWrite(vAddr, aReadBuf) == False:
                print 'Fail!'
            else:
                print 'Done!'


    qrib.Close()

    print '\n ---  Test  End   ---'
