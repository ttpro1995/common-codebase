r'''
  Name  : help/?
  Desc  : Used to display help information.
  Usage : help/?           --- display all supported commands.
          help/? <command> --- display help message of specific command.
'''

import os
import sys
import config


def _help_all():
    print ' All Available Commands:',
    v_cmd_count = 0
    for v_path in config.cmd_search_path:
        for v_cmd_file in os.listdir(os.getcwd() + v_path):
            if (v_cmd_file[:len(config.cmd_file_prefix)] == config.cmd_file_prefix)  \
                    and (v_cmd_file[-3:] == '.py'):
                if v_cmd_count % 4 == 0:
                    print
                    print ' ',
                print '%-17s' % v_cmd_file[len(config.cmd_file_prefix):-3],
                v_cmd_count += 1
    
    # list all remap command
    for v_cmd in config.cmd_remap_table:
        if v_cmd_count % 4 == 0:
            print
            print ' ',
        print '%-17s' % v_cmd,
        v_cmd_count += 1
    
    print
    return -1


def _help_cmd(v_cmd):
    # maybe this is a remapped command
    if v_cmd in config.cmd_remap_table:
        v_cmd = config.cmd_remap_table[v_cmd]
    
    for v_path in config.cmd_search_path:
        for v_cmd_file in os.listdir(os.getcwd() + v_path):
            if v_cmd_file == config.cmd_file_prefix + v_cmd + '.py':
                v_cmd_import = __import__(config.cmd_file_prefix + v_cmd)
                print v_cmd_import.__doc__
                del v_cmd_import
                return -1

    print " No help message for command '%s'!" % v_cmd
    return -1


def exec_cmd(argv):
    if len(argv) == 0:
        return _help_all()
    elif len(argv) == 1:
        return _help_cmd(argv[0])
    else:
        print ' Do not support more than one help command!'
        return -1
