
# Description: translate python source file to exe file.

SRC_FILE = "CLI.py"

# ##############################################################################
from distutils.core import setup
import py2exe
import sys
import subprocess
import os

if len(sys.argv) == 1:
    sys.argv.append('py2exe')

# Add all including files list here
includes = ["encodings", "encodings.*"]

setup(
    version     = "0.0.1",
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
                     "icon_resources" : [(1, "D:\compiler\python\child.ico")]
                  }],
)

# use "upx" to compress the exe file
EXE_FILE = os.path.join(".\\dist", os.path.basename(SRC_FILE)[0:-3])
subprocess.call('upx -9 -v "%s.exe"' % EXE_FILE)

# copy exe file to ..\bin directory
os.system(r'copy /y %s.exe .\\' % EXE_FILE)

#delete unused directory
os.system(r'rd /s/q .\build')
os.system(r'rd /s/q .\dist')
