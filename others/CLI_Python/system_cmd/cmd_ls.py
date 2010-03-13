r'''
  Name  : ls/ll/dir
  Desc  : List all files under current working directory.
  Usage : ls/ll/dir
'''

import os


def exec_cmd(argv):
    print " " + os.getcwd() + " :"
    
    for v_cmd_file in os.listdir(os.getcwd()):
        print "  |- " + v_cmd_file
    
    return -1
