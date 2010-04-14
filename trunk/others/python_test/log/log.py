
# log API

import time
import datetime


version = '0.01'
log_file_name = r'~test.log'

# append log with LF
def append_line(s):
    log_file = open(log_file_name, 'a')
    s = str(datetime.datetime.today()) + ' : ' + s
    log_file.write(s)
    log_file.write("\n")
    log_file.close()

# test procedure
def auto_test():
    print ' Auto Testing ....',
    append_line('-------------------------------------')
    append_line('test start ... ')
    append_line('  time baseline')
    time.sleep(1)
    append_line('  delayed 1s after previous line')
    time.sleep(0.5)
    append_line("  delayed 0.5s after previous line")
    append_line('test end.')
    append_line('-------------------------------------')
    raw_input(' Done. Press "Enter" key to exit ...')


if __name__ == '__main__':
    auto_test()
