'''
*******************************************************************************
 Desc    : LPT to I2C LPTB Driver.
 Note    : N/A
 History : 2010/7/30      Panda.Xiong       Create
*******************************************************************************
'''


import LPT


class LPTB:
    'I2C LPTB Driver'

    def __init__(self,
                 sName   = 'I2C_LPTB',
                 vSclOut = [0, True],
                 vSdaOut = [1, True],
                 vSclIn  = [6, True],
                 vSdaIn  = [5, True]
                 ):
        'Init I2C LPTB Driver'

        self.sName = sName

        self._vSclOut = vSclOut
        self._vSdaOut = vSdaOut
        self._vSclIn  = vSclIn
        self._vSdaIn  = vSdaIn

        self._vSCL = 1


    def IsMasterAvaliable(self):
        'Check this I2C Master is available or not'

        self.Open()

        # force stop all commulications on I2C Bus
        self.Stop()

        # Stretch SCL to 0, to Lock I2C Bus,
        #  to prevent other I2C Devices sending un-wanted data on the I2C Bus.
        self.SetSCL(0)

        # Set SDA to 0, then read, check the SDA can be controlled by software
        vData = 0
        self.SetSDA(vData)
        if self.GetSDA() != vData:
            # force stop all commulications on I2C Bus
            self.Stop()
            self.Close()
            return False

        # Set SDA to 1, then read, check the SDA can be controlled by software
        vData = 1
        self.SetSDA(vData)
        if self.GetSDA() != vData:
            # force stop all commulications on I2C Bus
            self.Stop()
            self.Close()
            return False

        # force stop all commulications on I2C Bus
        self.Stop()
        self.Close()

        # I2C Bus can be controlled by software, assume this I2C Host path is OK
        return True


    def Open(self):
        'Open I2C LPTB Driver'

        self._lpt = LPT.LPT()


    def Close(self):
        'Close I2C LPTB Driver'

        del self._lpt


    def SetSDA(self, vData):
        'Set SDA for I2C LPTB Driver'

        self._lpt.WriteBit(self._vSdaOut[0], vData ^ self._vSdaOut[1])


    def SetSCL(self, vData):
        'Set SCL for I2C LPTB Driver'

        self._vSCL = vData
        self._lpt.WriteBit(self._vSclOut[0], vData ^ self._vSclOut[1])


    def GetSDA(self):
        'Get SDA for I2C LPTB Driver'

        return self._lpt.ReadBit(self._vSdaIn[0]) ^ self._vSdaIn[1]


    def GetSCL(self):
        'Get SCL for I2C LPTB Driver'

        # I2C LPTB cannot read SCL line data,
        #  we only return the previous written data.
        return self._vSCL


    def Start(self):
        'Generate I2C Start'

        # I2C Start/ReStart:
        #  SDA changed from HIGH(1) to LOW(0), while SCL is HIGH(1).
        self.SetSDA(1)
        self.SetSCL(1)
        self.SetSDA(0)
        self.SetSCL(0)


    def Stop(self):
        'Generate I2C Stop'

        # Note: this is a I2C Start, if current SCL/SDA is HIGH;
        #       this will only happen while initializing I2C Master Bus.
        self.SetSDA(0)
        self.SetSCL(0)

        # I2C Stop:
        #  SDA changed from LOW(0) to HIGH(1), while SCL is HIGH(1).
        self.SetSCL(1)
        self.SetSDA(1)


    def ReStart(self):
        'Generate I2C Repeat-Start'

        self.Start()


    def SendAck(self):
        'Send ACK'

        self.SetSCL(0)
        self.SetSDA(0)

        self.SetSCL(1)
        self.SetSCL(0)
        self.SetSDA(1)


    def SendNoAck(self):
        'Send NACK'

        self.SetSDA(1)
        self.SetSCL(1)
        self.SetSCL(0)


    def SendByte(self, vData):
        'Send Byte Data'

        # Send Byte Data: MSB first, LSB last
        for i in range(8, 0, -1):
            self.SetSCL(0)
            self.SetSDA((vData >> (i-1)) & 1)
            self.SetSCL(1)

        # check I2C Slave is stretching the clock line
        vTimeOutCount = 0
        while self.GetSCL() == 0:
            time.sleep(0.01)    # delay 1ms
            vTimeOutCount += 1
            if vTimeOutCount > 25:
                return False    # Timeout

        # Check ACK
        self.SetSCL(0)
        self.SetSDA(1)
        self.SetSCL(1)
        for i in range(10): # for slow responding devices
            vAck = self.GetSDA()
            if vAck == 0:
                break   # ACK received
        self.SetSCL(0)

        # ACK=0, Send Byte success;
        # ACK=1, Send Byte fail.
        if vAck == 0:
            return True
        else:
            return False


    def ReceiveByte(self):
        'Receive Byte Data'

        # Receive Byte Data: MSB first, LSB last
        vData = 0x00
        for i in range(8, 0, -1):
            self.SetSCL(0)
            self.SetSCL(1)
            vData = (vData<<1) | self.GetSDA()

        self.SetSCL(0)

        return vData


    def CheckReady(self):
        'Check I2C hardware is ready to operate or not'

        if (self.GetSCL() == 1) and (self.GetSDA() == 1):
            # The I2C only can be started, while SCL/SDA is at high level
            return True

        elif (self.GetSCL() == 1) and (self.GetSDA() == 0):
            # SCL line is OK, but SDA line has been stretched by I2C Slave,
            #  we should attempt to reset the I2C Bus,
            #  to recover the SDA to idle state.
            #
            # According to SFF-8431 & INF-8077 Spec,
            #  we can follow below steps to recover SDA line:
            #
            #   Memory (Management Interface) Reset:
            #    1) Clock up to 9 cycles.
            #    2) Look for SDA high in each cycle while SCL is high.
            #    3) Create a START condition as SDA is high.

            for i in range(0, 8):
                self.SetSCL(0)
                self.SetSCL(1)

                if self.GetSDA() == 1:
                    # we have successfully recover the SDA line to idle state
                    return True

        else:
            # Unsupported stretching on SCL/SDA line, operation fail
            pass

        return False


    def _ReadBytes(self, vI2cAddr, vOffset, vReadLen):
        'I2C Read one or more bytes, with/without offset'

        vI2cAddr &= 0xFE
        aReadBuf  = []

        # make sure the I2C Bus is ready
        if self.CheckReady() == False:
            # we should reset the I2C, or the I2C bus will keep on fail
            self.Stop()
            return (False, aReadBuf)

        # send I2C start
        self.Start()

        if len(vOffset) > 0:
            # send I2C slave address + Write
            if self.SendByte(vI2cAddr) == False:
                # we should reset the I2C, or the I2C bus will keep on fail
                self.Stop()
                return (False, aReadBuf)

            # send offset
            for i in range(len(vOffset)):
                if self.SendByte(vOffset[i]) == False:
                    # we should reset the I2C, or the I2C bus will keep on fail
                    self.Stop()
                    return (False, aReadBuf)

            # send I2C repeat start
            self.ReStart()

        # send I2C slave address + Read
        if (self.SendByte(vI2cAddr+1) == False):
            # we should reset the I2C, or the I2C bus will keep on fail
            self.Stop()
            return (False, aReadBuf)

        # read data
        for i in range(vReadLen):
            aReadBuf.append(self.ReceiveByte())

            # if it's the last byte data, don't send ACK
            if i == (vReadLen-1):
                self.SendNoAck()
            else:
                self.SendAck()

        # send I2C stop
        self.Stop()

        return (True, aReadBuf)


    def _WriteBytes(self, vI2cAddr, vOffset, vWriteBuf):
        'I2C Write one or more bytes, with/without offset'

        vI2cAddr &= 0xFE

        # make sure the I2C Bus is ready
        if self.CheckReady() == False:
            # we should reset the I2C, or the I2C bus will keep on fail
            self.Stop()
            return False

        # send I2C start
        self.Start()

        # send I2C slave address + Write
        if self.SendByte(vI2cAddr) == False:
            # we should reset the I2C, or the I2C bus will keep on fail
            self.Stop()
            return False

        # send offset
        for i in range(len(vOffset)):
            if self.SendByte(vOffset[i]) == False:
                # we should reset the I2C, or the I2C bus will keep on fail
                self.Stop()
                return False

        # write data
        for i in range(len(vWriteBuf)):
            if self.SendByte(vWriteBuf[i]) == False:
                # we should reset the I2C, or the I2C bus will keep on fail
                self.Stop()
                return False

        # send I2C stop
        self.Stop()

        return True


    def RandomRead(self, vI2cAddr, vOffset, vReadLen):
        'I2C Random read one or more bytes'

        # prepare offset buffer
        aOffsetBuf = []
        aOffsetBuf.append(vOffset)

        return self._ReadBytes(vI2cAddr, aOffsetBuf, vReadLen)


    def RandomWrite(self, vI2cAddr, vOffset, aWriteBuf):
        'I2C Random write one or more bytes'

        # prepare offset buffer
        aOffsetBuf = []
        aOffsetBuf.append(vOffset)

        return self._WriteBytes(vI2cAddr, aOffsetBuf, aWriteBuf)


    def CurrentRead(self, vI2cAddr, vReadLen):
        'I2C Current read one or more bytes'

        # prepare offset buffer
        aOffsetBuf = []

        return self._ReadBytes(vI2cAddr, aOffsetBuf, vReadLen)


    def CurrentWrite(self, vI2cAddr, aWriteBuf):
        'I2C Current write one or more bytes'

        # prepare offset buffer
        aOffsetBuf = []

        return self._WriteBytes(vI2cAddr, aOffsetBuf, aWriteBuf)


    def Detect(self, vI2cAddr):
        'I2C Detect Slave'

        # prepare offset buffer
        aOffsetBuf = []
        aWriteBuf  = []

        return self._WriteBytes(vI2cAddr, aOffsetBuf, aWriteBuf)



'auto-test scripts'
if __name__ == '__main__':
    print ' ---  Test Start  ---\n'

    lptb = LPTB()

    print ' --> Name : %s' % lptb.sName,
    if lptb.IsMasterAvaliable():
        print '(Avaliable)'
    else:
        print '(Not Avaliable)'

    lptb.Open()


    vData = 0
    print '  Write SCL to %d.' % vData
    lptb.SetSCL(vData)
    print '  Read SCL =%d.' % lptb.GetSCL()

    vData = 1
    print '  Write SCL to %d.' % vData
    lptb.SetSCL(vData)
    print '  Read SCL =%d.' % lptb.GetSCL()


    vData = 0
    print '  Write SDA to %d.' % vData
    lptb.SetSDA(vData)
    print '  Read SDA =%d.' % lptb.GetSDA()

    vData = 1
    print '  Write SDA to %d.' % vData
    lptb.SetSDA(vData)
    print '  Read SDA =%d.' % lptb.GetSDA()

    print '\n All I2C Slaves :'
    aI2cAddrTable = []
    for vAddr in range(0x00, 0xFF, 2):
        if lptb.Detect(vAddr):
            print '  --> 0x%02X' % vAddr
            aI2cAddrTable.append(vAddr)

    for i in range(len(aI2cAddrTable)):
        vAddr = aI2cAddrTable[i]
        print '\n Read %02X.00-FF :' % vAddr,
        (vResult, aReadBuf) = lptb.RandomRead(vAddr, 0x00, 0x100)
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



    lptb.Close()

    print '\n ---  Test  End   ---'
