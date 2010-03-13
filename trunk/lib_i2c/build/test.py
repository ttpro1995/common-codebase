
# test lib_i2c.dll

from ctypes import *
import ctypes

LIB_I2C_DLL_NAME = "lib_i2c.dll"
print "\nLoad %s ................" % (LIB_I2C_DLL_NAME),
I2C = cdll.LoadLibrary(LIB_I2C_DLL_NAME)
print "Done.",
dll_version = I2C.LIB_I2C_GetDllVersion()
print "  DLL Version: v%X.%X.%X.%X" %                              \
	(((dll_version >> 24) & 0xFF), ((dll_version >> 16) & 0xFF),   \
	 ((dll_version >>  8) & 0xFF), ((dll_version >>  0) & 0xFF))

LPT_BASE_ADDRESS = 0x378
print "Set LPT Base Address to 0x%X ..." % (LPT_BASE_ADDRESS),
if I2C.LIB_I2C_SetLptAddress(LPT_BASE_ADDRESS):
	print "Done."
else:
	print "Fail!"

while True:
    print "Detecting I2C Host ..............",
    result = I2C.Init()
    if result == 1:
    	print "LPTA."
    elif result == 2:
    	print "LPTB."
    elif result == 3:
    	print "USB."
    else:
    	print "Unknown Host! result=", result

    print "\nAll I2C Slave:"
    vCount = 0
    for vAddr in range(0x00, 0xFF, 2):
    	if I2C.Detect(vAddr) != 0:
    		vCount += 1
    		print " Device %d -- 0x%02X" % (vCount, vAddr)

    print "\nDump A2[DIRECT]:",
    PAGE_LEN = 0x80
    aBuf = (c_ubyte * PAGE_LEN)()
    if I2C.ReadBytes(0xA2, 0x00, PAGE_LEN, byref(aBuf)) != 0:
    	for vLoop in range(0, PAGE_LEN):
    		if vLoop % 0x10 == 0:
    			print "\n %02X :" % vLoop,
    		elif vLoop % 0x08 == 0:
    			print "-",
    		print "%02X" % aBuf[vLoop],
    print ""

    print "\n --- Test End ---"
    print " Press 'Enter' key to exit ..."
    raw_input()
