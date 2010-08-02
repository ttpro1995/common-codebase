'''
*******************************************************************************
 Desc    : LPT to I2C LPTA Driver.
 Note    : N/A
 History : 2010/7/30      Panda.Xiong       Create
*******************************************************************************
'''


import LPTB


# LPTA is inherited from LPTB, only the pin configuration is different.
class LPTA(LPTB.LPTB):
    'I2C LPTA Driver'

    def __init__(self,
                 sName   = 'I2C_LPTA',
                 vSclOut = [0, True],
                 vSdaOut = [1, True],
                 vSclIn  = [6, True],
                 vSdaIn  = [5, False]
                 ):
        'Init I2C LPTA Driver'

        LPTB.LPTB.__init__(self)

        self.sName = sName

        self._vSclOut = vSclOut
        self._vSdaOut = vSdaOut
        self._vSclIn  = vSclIn
        self._vSdaIn  = vSdaIn

        self._vSCL = 1



'auto-test scripts'
if __name__ == '__main__':
    print ' ---  Test Start  ---\n'

    lpta = LPTA()
    lpta.Open()

    print ' --> Name : %s' % lpta.sName,
    if lpta.IsMasterAvaliable():
        print '(Avaliable)'
    else:
        print '(Not Avaliable)'


    vData = 0
    print '  Write SCL to %d.' % vData
    lpta.SetSCL(vData)
    print '  Read SCL =%d.' % lpta.GetSCL()

    vData = 1
    print '  Write SCL to %d.' % vData
    lpta.SetSCL(vData)
    print '  Read SCL =%d.' % lpta.GetSCL()


    vData = 0
    print '  Write SDA to %d.' % vData
    lpta.SetSDA(vData)
    print '  Read SDA =%d.' % lpta.GetSDA()

    vData = 1
    print '  Write SDA to %d.' % vData
    lpta.SetSDA(vData)
    print '  Read SDA =%d.' % lpta.GetSDA()

    print '\n All I2C Slaves :'
    aI2cAddrTable = []
    for vAddr in range(0x00, 0xFF, 2):
        if lpta.Detect(vAddr):
            print '  --> 0x%02X' % vAddr
            aI2cAddrTable.append(vAddr)

    for i in range(len(aI2cAddrTable)):
        vAddr = aI2cAddrTable[i]
        print '\n Read %02X.00-FF :' % vAddr,
        (vResult, aReadBuf) = lpta.RandomRead(vAddr, 0x00, 0x100)
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



    lpta.Close()

    print '\n ---  Test  End   ---'
