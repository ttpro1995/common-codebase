'''
*******************************************************************************
 Desc    : A script used to parse Internal MemoryMap.
 Note    : N/A
 History : 2010/07/14      Panda.Xiong      Create
*******************************************************************************
'''

import sys
import xlrd

class MemoryMap:
    def __init__(self, filename=None):
        # check input filename
        if filename == None:
            print 'Incorrect filename!'
            sys.exit(-1)
        self.filename = filename

        # create an empty dictionary
        self.MemTable = {}

        self.logname = r"~test.log"
        self.log     = file(self.logname, 'w')


    def open(self):
        # open excel file
        self.xlBook = xlrd.open_workbook(self.filename)

        try:
            self.sheet = self.xlBook.sheet_by_name('Internal Memory Map')
        except:
            print 'No "Internal Memory Map" sheet found in "%s" excel file!' % (self.filename)
            sys.exit(-1)

        # launch all memory definition rows
        for row in range(1, self.sheet.nrows):
            # read row data
            row_data = self.sheet.row_values(row)
            row_key  = row_data[0]

            # have reached to end of excel file, exit
            if row_key == 'END':
                break

            # if this row is not empty, record it
            if row_key:
                self.MemTable[row_key] = row_data[1:]


    def close(self):
        'do nothing'
        pass


    def show(self):
        for i in self.MemTable:
            self.log.write('%-35s : %s\n' % (i, str(self.MemTable[i])))



'test lines'
if __name__ == '__main__':
    map = MemoryMap(r'../1620-00100-00_InternalMemoryMap.xls')
    map.open()
    map.show()
    map.close()
