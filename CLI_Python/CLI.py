r'''
    CLI Kernel (yaxi1984@gmail.com)
'''

import os,sys
import WConio as console
import config

v_cmd_history = []
v_cmd_history_current = 0

# append command search path
for v_path in config.cmd_search_path:
    sys.path.append(os.getcwd() + v_path)

console.settitle(config.cli_prompt + ' ' + config.cli_version)
while True:
    b_restart = False
    sys.stdout.write(config.cli_prompt + '>')
    v_cmd_buf = []
    while True:
        v_key = console.getkey()
        if v_key == None:
            continue

        if (len(v_key) == 1) and (v_key >= '\x20') and (v_key < '\x7F'):
            sys.stdout.write(v_key)
            v_cmd_buf.append(v_key)
        else:
            # move cursor left
            if v_key == 'left':
                # do nothing
                pass

            # move cursor right
            elif v_key == 'right':
                # do nothing
                pass

            # move cursor up
            elif v_key == 'up':
                # clear command in screen
                while len(v_cmd_buf) > 0:
                    v_cmd_buf.pop()
                    sys.stdout.write('\b \b')

                if v_cmd_history_current > 0:
                    v_cmd_history_current -= 1
                    v_cmd_buf = list(v_cmd_history[v_cmd_history_current])
                    sys.stdout.write(''.join(v_cmd_buf))

            # move cursor down
            elif v_key == 'down':
                # clear command in screen
                while len(v_cmd_buf) > 0:
                    v_cmd_buf.pop()
                    sys.stdout.write('\b \b')

                if v_cmd_history_current < len(v_cmd_history):
                    v_cmd_buf = list(v_cmd_history[v_cmd_history_current])
                    sys.stdout.write(''.join(v_cmd_buf))
                    v_cmd_history_current += 1

            # backspace
            elif v_key == '\b':
                if len(v_cmd_buf) > 0:
                    v_cmd_buf.pop()
                    sys.stdout.write('\b \b')

            # CTRL+Z/U, clear command buffer
            elif (v_key == '\x15') or (v_key == '\x1A'):
                while len(v_cmd_buf) > 0:
                    v_cmd_buf.pop()
                    sys.stdout.write('\b \b')

            # CTRL+L, clear screen, but keep command
            elif v_key == '\x0C':
                console.clrscr()
                sys.stdout.write(config.cli_prompt + '>' + ''.join(v_cmd_buf).strip())

            # TAB, to auto-complete the input command
            elif v_key == '\x09':
                v_cmd = ''.join(v_cmd_buf).strip()
                v_tmp_list = []
                for v_path in config.cmd_search_path:
                    for v_cmd_file in os.listdir(os.getcwd() + v_path):
                        if (v_cmd_file[:len(config.cmd_file_prefix)] == config.cmd_file_prefix)  \
                                and (v_cmd_file[-3:] == '.py')                                   \
                                and v_cmd in v_cmd_file[len(config.cmd_file_prefix):-3]:
                            v_tmp_list.append(v_cmd_file[len(config.cmd_file_prefix):-3])

                if len(v_tmp_list) == 1:
                    for i in range(len(v_cmd_buf)):
                        sys.stdout.write('\b \b')
                    v_cmd_buf = list(v_tmp_list[0])
                    print v_tmp_list[0],
                elif len(v_tmp_list) == 0:
                    pass
                else:
                    for i in range(len(v_tmp_list)):
                        if i % 4 == 0:
                            print
                            print ' ',
                        print '%-17s' % v_tmp_list[i],
                    print
                    sys.stdout.write(config.cli_prompt + '>' + ''.join(v_cmd_buf).strip())

            # other un-supported un-printable character, do nothing
            else:
                pass

        # if input 'Enter', means a new command is input,
        # quit this loop to deal the command.
        if (v_key == '\r') or (v_key == '\n'):  # Enter
            sys.stdout.write(os.linesep)

            # join command buffer to command string, and remove redundant space.
            v_cmd = ''.join(v_cmd_buf).strip()

            if v_cmd != '':
                # record this command into command history buffer.
                v_cmd_history.append(v_cmd)
                v_cmd_history_current = len(v_cmd_history)
            else:
                b_restart = True

            # break this loop, to deal the command.
            break

    if b_restart:
        continue

    # parse command
    v_tmp_cmd_list = v_cmd.split(' ')
    v_cmd_list = []
    for i in v_tmp_cmd_list:
        if i == '':
            # skip redundant spaces
            continue
        v_cmd_list.append(i)

    # remap command
    if v_cmd_list[0] in config.cmd_remap_table:
        v_cmd_list[0] = config.cmd_remap_table[v_cmd_list[0]]

    # search & execute command
    b_cmd_find = False
    for v_path in config.cmd_search_path:
        for v_cmd_file in os.listdir(os.getcwd() + v_path):
            if v_cmd_file == config.cmd_file_prefix + v_cmd_list[0] + '.py':
                v_cmd_import = __import__(config.cmd_file_prefix + v_cmd_list[0])
                v_cmd_result = v_cmd_import.exec_cmd(v_cmd_list[1:])
                if v_cmd_result == True:
                    print ' Success!'
                elif v_cmd_result == False:
                    print ' Fail!'
                else:
                    # print nothing
                    pass
                del v_cmd_import
                b_cmd_find = True
                break
        if b_cmd_find:
            break
    else:
        # command is not found
        print " Invalid command, type '?' or 'help' to get help!"
