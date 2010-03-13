r'''
  Name  : pwd
  Desc  : Display current working directory.
  Usage : pwd
'''

import os


def exec_cmd(argv):
    print ' ' + os.getcwd()
    return -1
