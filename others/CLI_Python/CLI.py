
import os,sys
import WConio as console
import config

v_cmd_history = []
v_cmd_history_current = 0

# append command search path
for v_path in config.cmd_search_path:
    sys.path.append(os.getcwd() + v_path)

console.settitle(config.cli_prompt)
while True:
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
                if len(v_cmd_buf) > 0:
                    sys.stdout.write('\b')

            # move cursor right
            elif v_key == 'right':
                if len(v_cmd_buf) > 0:
                    sys.stdout.write('\b')

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

            # other un-supported un-printable character, do nothing
            else:
                pass

        # if input 'Enter', means a new command is input,
        # quit this loop to deal the command.
        if (v_key == '\r') or (v_key == '\n'):  # enter
            sys.stdout.write(os.linesep)

            # join command buffer to command string, and remove redundant space.
            v_cmd = ''.join(v_cmd_buf).strip()
            if v_cmd != '':
                # record this command into command history buffer.
                v_cmd_history.append(v_cmd)
                v_cmd_history_current = len(v_cmd_history)

                # break this loop, to deal the command.
                break
            else:
                sys.stdout.write(config.cli_prompt + '>')

    # parse command
    v_cmd_list = v_cmd.split(' ')

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
