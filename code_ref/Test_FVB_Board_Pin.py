''' 
*******************************************************************************
 Desc    : A script used to test FVB Board Pin.
 Note    : N/A
 History : 2009/12/03    Panda.Xiong   Create
*******************************************************************************
'''

# =============================================================================
# Common Part, do not modify
# =============================================================================

from ctypes import *
import sys
import os

def _assert(msg):
    print msg
    raw_input('\nPress "Enter" key to exit ...')
    sys.exit(-1)

# clear screen
os.system('cls')


# I2C host type definition
dictI2cHost = {
                1: 'LPTA',           \
                2: 'LPTB',           \
                3: 'USB-CH341'       \
              }

# load I2C Library dll
try:
    I2C = cdll.LoadLibrary('lib_i2c.dll')
except EOFError:
    print '\nLoad I2C Library Fail!'
    raw_input('Press "Enter" key to exit ...')
    sys.exit(-1)
print '\nLoad I2C Library Success.'

# init I2C Library
result = I2C.Init()
if result in dictI2cHost:
    print ' --> I2C Host    :', dictI2cHost[result]
else:
    _assert('Unknown Host!')

vDllVersion = I2C.GetDllVersion()
sDllVersion = 'v%d.%d.%d.%d'                      \
              % ((vDllVersion >> 24) & 0xFF,      \
                 (vDllVersion >> 16) & 0xFF,      \
                 (vDllVersion >>  8) & 0xFF,      \
                 (vDllVersion >>  0) & 0xFF)
print ' --> DLL Version :', sDllVersion


# =============================================================================
# Porting Part
# =============================================================================
print __doc__

import WConio
import time

# Set Control FVB Board I2C Address & DUT I2C Address
vFVB_Addr  = 0x88
vDUT_Addr  = 0xA2
DELAY_TIME = 1
TEST_PIN   = 'TXDIS'

# Set FVB Board I2C Memory
dictFVB_Mem = {                                       \
 'MOD_PWR' : 0x24,   # enable Module Power            \
 'TXDIS'   : 0x25,   # Set TxDisable Output.          \
 'AS0'     : 0x26,   # Set AS0 Output.                \
 'AS1'     : 0x27    # Set AS1 Output.                \
}

# backup the current Cursor position
vCursorX = WConio.wherex()
vCursorY = WConio.wherey()

vState = 0
while True:
    print 'Set %-7s to %d.' % (TEST_PIN, vState)
    I2C.WriteByte(vFVB_Addr, dictFVB_Mem[TEST_PIN], vState)
    
    time.sleep(DELAY_TIME)
    vState = not vState

    # restore the Cursor position
    WConio.gotoxy(vCursorX, vCursorY)

