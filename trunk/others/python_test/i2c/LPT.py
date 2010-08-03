'''
*******************************************************************************
 Desc    : A script used to accessing Parallel Port, based on "inpout32.dll".
 Note    : N/A
 History : 2010/7/30      Panda.Xiong       Create
*******************************************************************************
'''


from ctypes import windll
import sys


class LPT:
    'LPT Parallel Port'

    def _assert(self, sErrMsg):
        print sErrMsg
        raw_input('\nPress "Enter" key to exit ...')
        sys.exit(-1)


    def __init__(self, vBaseAddr=0x378):
        'Init LPT Port, default base address: 0x378'

        self._vLptBaseAddr = vBaseAddr

        # try to load Parallel Port dll
        try:
            self._inpout32 = windll.LoadLibrary('inpout32.dll')
        except:
            self._assert('Load "inpout32.dll" fail!')

        self._vLptOut = 0xFF


    def Inp32(self):
        'Read Data from LPT Port'

        return self._inpout32.Inp32(self._vLptBaseAddr+1)


    def Oup32(self, vData):
        'Write Data to LPT Port'

        self._inpout32.Out32(self._vLptBaseAddr+0, vData)


    def WriteBit(self, vBit, vData):
        'Change bit value for LPT Port'

        # record new output value
        self._vLptOut = (self._vLptOut & ~(1 << vBit)) | (vData << vBit)

        self.Oup32(self._vLptOut)


    def ReadBit(self, vBit):
        'Read bit value from LPT Port'

        vReadData = self.Inp32()
        return (self.Inp32() >> vBit) & 1


    def SetLptBaseAddr(self, vNewAddr):
        'Set New LPT Port Base Address'

        self._vLptBaseAddr = vNewAddr



# auto-test scripts
if __name__ == '__main__':
    print ' ---  Test Start  ---'

    lpt = LPT()

    vData = 0x5A5A
    print '  Write 0x%04X to LPT Port.' % vData
    lpt.Oup32(vData)
    print '  Read data from LPT Port: 0x%04X' % lpt.Inp32()

    for vBit in range(16):
        vData = 0
        print '  Write Bit %d=%d to LPT Port.' % (vBit, vData)
        lpt.WriteBit(vBit, vData)
        print '  Read Bit %d=%d to LPT Port.' % (vBit, lpt.ReadBit(vBit))

        vData = 1
        print '  Write Bit %d=%d to LPT Port.' % (vBit, vData)
        lpt.WriteBit(vBit, vData)
        print '  Read Bit %d=%d to LPT Port.' % (vBit, lpt.ReadBit(vBit))

    print ' ---  Test  End   ---'
