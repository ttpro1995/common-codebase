'''
*******************************************************************************
 Desc    : A script used to translate python source file to exe file.
 Usage   : setup.py <python_main_filename> [--console | --windows]
 Note    : N/A
 History : 2010/02/08      Panda.Xiong      Create
*******************************************************************************
'''

import distutils.core
import py2exe
import sys
import subprocess
import os

if (len(sys.argv) == 3)                                                     \
        and ((sys.argv[2] == '--console') or (sys.argv[2] == '--windows')):
    SRC_FILE    = sys.argv[1]
    TARGET_TYPE = sys.argv[2]
else:
    print __doc__
    raw_input("--- Press 'Entry' key to exit ---")

sys.argv.pop()
sys.argv[1] = 'py2exe'

# Add all including files list here
includes = ["encodings", "encodings.*"]

if TARGET_TYPE == '--console':
    distutils.core.setup(
        version     = "1.00",
        description = "N/A",
        name        = SRC_FILE,
        options     = {
                        "py2exe": {
                                    "compressed"   : 1,
                                    "optimize"     : 2,  # use better optimize level
                                    "includes"     : includes,
                                    "bundle_files" : 1   # generate a signel exe file
                                  }
                      },

        zipfile     = None,

        # for windows without console, set this to "windows",
        # else, set this to "console".
        console     = [{
                         "script"         : SRC_FILE,
                         "icon_resources" : [(1, os.getenv("windir")+"\child.ico")]
                      }],
    )
else:
    distutils.core.setup(
        version     = "1.00",
        description = "N/A",
        name        = SRC_FILE,
        options     = {
                        "py2exe": {
                                    "compressed"   : 1,
                                    "optimize"     : 2,  # use better optimize level
                                    "includes"     : includes,
                                    "bundle_files" : 1   # generate a signel exe file
                                  }
                      },

        zipfile     = None,

        # for windows without console, set this to "windows",
        # else, set this to "console".
        windows     = [{
                         "script"         : SRC_FILE,
                         "icon_resources" : [(1, os.getenv("windir")+"\child.ico")]
                      }],
    )

# use "upx" to compress the exe file
# os.path.basename(SRC_FILE)[0:-3]
SRC_FILE_BASE = ''.join(os.path.splitext(os.path.basename(SRC_FILE))[0:-1])
EXE_FILE = os.path.join(r".\dist", SRC_FILE_BASE)
subprocess.call('upx -9 -v "%s.exe"' % EXE_FILE)

# copy exe file to current directory
os.system(r'copy /y %s.exe .\\' % EXE_FILE)

#delete unused directory
os.system(r'rd /s/q .\build')
os.system(r'rd /s/q .\dist')
