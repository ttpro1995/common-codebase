
import time
import datetime


class LOG:
    def __init__(self, file_name):
        self.file_name = file_name
        self.log = file(self.file_name, 'w')

    def record(self, msg):
        'append log with LF'
        s = str(datetime.datetime.today()) + ' : ' + msg + '\n'
        self.log.write(s)

    def flush(self):
        self.log.flush()


# auto-test scripts
if __name__ == '__main__':
    log = LOG(r'~test.log')
    print ' Auto Testing ....',
    log.record('-------------------------------------')
    log.record('test start ... ')
    log.record('  time baseline')
    time.sleep(1)
    log.record('  delayed 1s after previous line')
    time.sleep(0.5)
    log.record("  delayed 0.5s after previous line")
    log.record('test end.')
    log.record('-------------------------------------')
    raw_input('Done. \n Press "Enter" key to exit ...')
