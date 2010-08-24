'''
*******************************************************************************
 Desc    : USB to I2C (QRIB) Driver.
 Note    : N/A
 History : 2010/08/02      Panda.Xiong       Create
*******************************************************************************
'''


from ctypes import *
import sys
import struct
import time


QRIB_MAX_LENGTH = 32  # maximum read/write length

SI_SUCCESS = 0


PKT_MAGIC        = 0xdeadf00d
PKT_DIR_HOST2DEV = 0
PKT_DIR_DEV2HOST = 1
PKT_TYPE_I2C     = 3
PKT_SUBTYPE_I2C_READ            = 0
PKT_SUBTYPE_I2C_WRITE           = 1
PKT_SUBTYPE_I2C_ADV_SETUP_WRITE = 2
PKT_SUBTYPE_I2C_ADV_SETUP_READ  = 3
PKT_SUBTYPE_I2C_READ_ALT        = 4
PKT_SUBTYPE_I2C_WRITE_ALT       = 5

PKT_ERR_SUCCESS = 0


# Access Type for QRIB (Only bit[7:0] is valid)
#  bit[7]: =1, Without Offset; =0, With Offset;
#  bit[6]: =1, use two separate sequences for reads;
#  bit[5]: =1, do not support I2C Slave Clock Stretching; =0, support I2C Slave Clock Stretching;
#  bit[4]: =1, do not toggle SCL at end of transaction and short stop/start
ACCESS_TYPE_NO_OFFSET      = (1 << 7)
ACCESS_TYPE_NO_RD2SEQ      = (1 << 6)
ACCESS_TYPE_NO_NOSTRETCH   = (1 << 5)
ACCESS_TYPE_NO_NOCLRSCL    = (1 << 4)
ACCESS_TYPE_NO_ACCESS_TYPE = 0

I2C_READ  = 1
I2C_WRITE = 0


class QRIB:
    'USB to I2C (QRIB) Driver'

    def _assert(self, sErrMsg):
        print sErrMsg
        raw_input('\nPress "Enter" key to exit ...')
        sys.exit(-1)


    def __init__(self,
                 sName           = 'USB_QRIB',
                 vReadTimeoutMs  = 200,
                 vWriteTimeoutMs = 200,
                 vWriteDelayTime = 0.05   # write delay time, unit of second
                 ):
        'Init I2C USB-QRIB Driver'

        self.sName = sName

        self._vReadTimeoutMs  = vReadTimeoutMs
        self._vWriteTimeoutMs = vWriteTimeoutMs
        self._vWriteDelayTime = vWriteDelayTime

        self._pHandle = c_ulong(0)

        self._vAccessType = 0


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


    def _create_pkt(self, bRead, vAddr, vOffset, vAccessLen, aDataBuf):
        'create QRIB packet buffer'

        # QRIB only support maximum 64 Bytes packet: Header(9B) + Data(55B)
        aPkt  = (c_ubyte * 64)()

        vLoop = 0
        aPkt[0]  = (PKT_MAGIC >> 24) & 0xFF
        aPkt[1]  = (PKT_MAGIC >> 16) & 0xFF
        aPkt[2]  = (PKT_MAGIC >>  8) & 0xFF
        aPkt[3]  = (PKT_MAGIC >>  0) & 0xFF
        aPkt[4]  = PKT_DIR_HOST2DEV
        aPkt[5]  = PKT_TYPE_I2C
        aPkt[6]  = (PKT_SUBTYPE_I2C_WRITE, PKT_SUBTYPE_I2C_READ)[bRead]
        aPkt[7]  = 4 + len(aDataBuf)
        aPkt[8]  = 0
        aPkt[9]  = self._vAccessType
        aPkt[10] = vAddr
        aPkt[11] = vOffset
        aPkt[12] = vAccessLen

        for vLoop in range(len(aDataBuf)):
            aPkt[13+vLoop] = aDataBuf[vLoop]

        return (13+len(aDataBuf), aPkt)


    def _I2C_Write(self, vAddr, vOffset, aBuf):
        'I2C write one or more bytes'

        # send I2C Write command
        (vPktLen, aPktBuf) = self._create_pkt(I2C_WRITE, vAddr, vOffset, len(aBuf), aBuf)
        vWrittenLen = c_long(0)
        if (self._qrib.SI_Write(self._pHandle, aPktBuf, vPktLen, byref(vWrittenLen), 0) != SI_SUCCESS)  \
                or (vWrittenLen.value != vPktLen):
            return False

        # read I2C Write ACK
        vPktLen  = 9
        vReadLen = c_long(0)
        if (self._qrib.SI_Read(self._pHandle, byref(aPktBuf), vPktLen, byref(vReadLen), 0) != SI_SUCCESS)  \
                or (vReadLen.value != vPktLen) \
                or (aPktBuf[8] != PKT_ERR_SUCCESS):
            return False

        return True


    def _I2C_Read(self, vAddr, vOffset, vLen):
        'I2C read one or more bytes'

        # send I2C Read command
        (vPktLen, aPktBuf) = self._create_pkt(I2C_READ, vAddr, vOffset, vLen, [])
        vWrittenLen = c_long(0)
        if (self._qrib.SI_Write(self._pHandle, aPktBuf, vPktLen, byref(vWrittenLen), 0) != SI_SUCCESS)  \
                or (vWrittenLen.value != vPktLen):
            return (False, [])

        # receive I2C Read data
        vPktLen += vLen
        vReadLen = c_long(0)
        if (self._qrib.SI_Read(self._pHandle, byref(aPktBuf), vPktLen, byref(vReadLen), 0) != SI_SUCCESS)  \
                or (vReadLen.value != vPktLen) \
                or (aPktBuf[8] != PKT_ERR_SUCCESS):
            return (False, [])

        return (True, aPktBuf[13 : 13+vLen])


    def _I2C_Detect(self, vAddr):
        'I2C Detect Slave'

        # send I2C Write Command, without offset
        (vPktLen, aPktBuf) = self._create_pkt(I2C_WRITE, vAddr, 0, 0, [])
        vWrittenLen = c_long(0)
        if (self._qrib.SI_Write(self._pHandle, aPktBuf, vPktLen, byref(vWrittenLen), 0) != SI_SUCCESS)  \
                or (vWrittenLen.value != vPktLen):
            return False

        # read I2C Write ACK
        vPktLen  = 9
        vReadLen = c_long(0)
        if (self._qrib.SI_Read(self._pHandle, byref(aPktBuf), vPktLen, byref(vReadLen), 0) != SI_SUCCESS)  \
                or (vReadLen.value != vPktLen) \
                or (aPktBuf[8] != PKT_ERR_SUCCESS):
            return False

        return True



    def RandomRead(self, vI2cAddr, vOffset, vReadLen):
        'I2C Random read one or more bytes'

        self._vAccessType = ACCESS_TYPE_NO_ACCESS_TYPE

        aReadBuf = []
        for vLoop in range(vOffset, vOffset+vReadLen, QRIB_MAX_LENGTH):
            if (vLoop+QRIB_MAX_LENGTH) <= (vOffset+vReadLen):
                vTmpLen = QRIB_MAX_LENGTH
            else:
                vTmpLen = vOffset+vReadLen - vLoop

            (vResult, aTmpReadBuf) = self._I2C_Read(vI2cAddr, vLoop, vTmpLen)
            if vResult == False:
                return (False, [])
            else:
                aReadBuf += aTmpReadBuf

        return (True, aReadBuf)


    def RandomWrite(self, vI2cAddr, vOffset, aBuf):
        'I2C Random write one or more bytes'

        self._vAccessType = ACCESS_TYPE_NO_ACCESS_TYPE

        vWriteLen = len(aBuf)
        for vLoop in range(vOffset, vOffset+vWriteLen, QRIB_MAX_LENGTH):
            if (vLoop+QRIB_MAX_LENGTH) <= (vOffset+vWriteLen):
                vTmpLen = QRIB_MAX_LENGTH
            else:
                vTmpLen = vOffset+vWriteLen - vLoop

            vResult = self._I2C_Write(vI2cAddr, vLoop, aBuf[vLoop : vLoop+vTmpLen])
            if vResult == False:
                return vResult

            # add this, to force delay some time, to fix bug:
            #  when writing too long data, the target may be fail to response.
            time.sleep(self._vWriteDelayTime)

        return True


    def CurrentRead(self, vI2cAddr, vReadLen):
        'I2C Current read one or more bytes'

        self._vAccessType = ACCESS_TYPE_NO_OFFSET
        vOffset = 0

        aReadBuf = []
        for vLoop in range(vOffset, vOffset+vReadLen, QRIB_MAX_LENGTH):
            if (vLoop+QRIB_MAX_LENGTH) <= (vOffset+vReadLen):
                vTmpLen = QRIB_MAX_LENGTH
            else:
                vTmpLen = vOffset+vReadLen - vLoop

            (vResult, aTmpReadBuf) = self._I2C_Read(vI2cAddr, vLoop, vTmpLen)
            if vResult == False:
                return (False, [])
            else:
                aReadBuf += aTmpReadBuf

        return (True, aReadBuf)


    def CurrentWrite(self, vI2cAddr, aBuf):
        'I2C Current write one or more bytes'

        self._vAccessType = ACCESS_TYPE_NO_OFFSET
        vOffset = 0

        vWriteLen = len(aBuf)
        for vLoop in range(vOffset, vOffset+vWriteLen, QRIB_MAX_LENGTH):
            if (vLoop+QRIB_MAX_LENGTH) <= (vOffset+vWriteLen):
                vTmpLen = QRIB_MAX_LENGTH
            else:
                vTmpLen = vOffset+vWriteLen - vLoop

            vResult = self._I2C_Write(vI2cAddr, vLoop, aBuf[vLoop : vLoop+vTmpLen])
            if vResult == False:
                return vResult

            # add this, to force delay some time, to fix bug:
            #  when writing too long data, the target may be fail to response.
            time.sleep(self._vWriteDelayTime)

        return True


    def Detect(self, vI2cAddr):
        'I2C Detect Slave'

        self._vAccessType = ACCESS_TYPE_NO_OFFSET

        return self._I2C_Detect(vI2cAddr)



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
