'''
*******************************************************************************
 Desc    : A script used to provide standard MSA Interface.
 Note    : N/A
 History : 2010/07/15      Panda.Xiong      Create
*******************************************************************************
'''

from ctypes import *
import sys
import string
import os
import xlrd
import i2c



class _I2cSecurity:
    'define I2C Security related'

    def _assert(self, message):
        print 'Error: ' + message
        sys.exit(-1)


    def __init__(self):

        # security level
        self.SECURITY_LEVEL_NORMAL   = 0x00
        self.SECURITY_LEVEL_USER_EE  = 0x10
        self.SECURITY_LEVEL_OEM      = 0x15
        self.SECURITY_LEVEL_FACTORY  = 0x20
        self.SECURITY_LEVEL_RD_DEBUG = 0xFF

        # security password
        self.__security_table = {}
        self.__security_table[self.SECURITY_LEVEL_NORMAL]   = 0xFFFFFFFF
        self.__security_table[self.SECURITY_LEVEL_USER_EE]  = 0x00001011
        self.__security_table[self.SECURITY_LEVEL_OEM]      = 0x4F454D20
        self.__security_table[self.SECURITY_LEVEL_FACTORY]  = 0x7E755D4F
        self.__security_table[self.SECURITY_LEVEL_RD_DEBUG] = 0x52444247


    def GetPasswd(self, level):
        'Get password by Security Level'

        if level in self.__security_table:
            return self.__security_table[level]
        else:
            self._assert('Invalid Security Level!')


    def ChangePasswd(self, level, passwd):
        'Change I2C Security Password'

        if level in self.__security_table:
            self.__security_table[level] = passwd
        else:
            self._assert('Invalid Security Level!')



class _AddrInfo:
    'used to store parsed address information'

    def __init__(self, mem_addr=0, mem_page_id=0, mem_offset=0, mem_len=0, mem_bit=0):
        self.mem_addr    = mem_addr
        self.mem_page_id = mem_page_id  # -1 means DIRECT Page
        self.mem_offset  = mem_offset
        self.mem_len     = mem_len      # if mem_bit == -1, this means Byte(s) length; else, means bit length.
        self.mem_bit     = mem_bit      #  -1 means Byte(s)



class _MemoryMap:

    def _assert(self, message):
        print 'Error: ' + message
        sys.exit(-1)


    def __init__(self, filename, write_delay, debug_enable=False):

        # check input filename
        if filename == None:
            self._assert('Incorrect filename!')

        # record input parameter
        self.__filename     = filename
        self.__debug_enable = debug_enable

        # create an empty dictionary
        self.__memtable = {}

        if self.__debug_enable:
            self.__logname = r'~memmap.log'
            self.__log     = file(self.__logname, 'w')

        # load I2C Library
        self.__i2c = i2c.I2C(write_delay)

        if len(self.__i2c.vAvaliableMasterTable) == 0:
            self._assert('No Avaliable I2C Master!')
        else:
            print 'All I2C Master:'
            for i in self.GetAvaliableI2cMaster():
                print '  -> %s' % i,
                if i == self.GetCurrentI2cMaster():
                    print '(Active)'
                else:
                    print
            print

        # open Internal MemoryMap
        self.__open()


    def GetAvaliableI2cMaster(self):
        'Get all Avaliable I2C Master Name'

        return self.__i2c.vAvaliableMasterTable.keys()


    def GetCurrentI2cMaster(self):
        'Get Current I2C Master Name'

        return self.__i2c._CurrentI2cMaster.sName


    def SelectI2cMaster(self, vMasterName):
        'Select I2C Master by name'

        return self.__i2c.SelectMaster(vMasterName)


    def __parse_address(self, s_addr):
        'parse address string'

        valid_addr_string_table = ['0','1','2','3','4','5','6','7','8','9',
                                   'A','B','C','D','E','F',
                                   '[',']','.','-']

        # check the input address string is valid or not
        for i in range(len(s_addr)):
            if not s_addr[i] in valid_addr_string_table:
                self._assert('Invalid Address(1): %s' % s_addr)

        addr_info = _AddrInfo()

        # split input address string
        ss_parse = s_addr.split('.')

        # get memory address & page id
        if (len(ss_parse) != 2) and (len(ss_parse) != 3):
            self._assert('Invalid Address(2): %s' % s_addr)

        if ss_parse[0].find('[') == -1:
            # DIRECT Page
            addr_info.mem_addr    = string.atoi(ss_parse[0], 16)
            addr_info.mem_page_id = -1
        else:
            # Extended Page
            dev_end_index  = ss_parse[0].index('[')
            page_end_index = ss_parse[0].index(']')
            addr_info.mem_addr    = string.atoi(ss_parse[0][0:dev_end_index], 16)
            addr_info.mem_page_id = string.atoi(ss_parse[0][dev_end_index+1:page_end_index], 16)

        # get memory offset & length
        if ss_parse[1].find('-') == -1:
            # memory length is 1 Byte
            addr_info.mem_offset = string.atoi(ss_parse[1], 16)
            addr_info.mem_len    = 1
        else:
            # memory length is over 1 Byte
            if len(ss_parse) == 3:
                self._assert('Invalid Address(3): %s' % s_addr)
            ss_offset = ss_parse[1].split('-')
            addr_info.mem_offset = string.atoi(ss_offset[0], 16)
            addr_info.mem_len    = string.atoi(ss_offset[1], 16) - addr_info.mem_offset + 1

        # get memory bit
        if len(ss_parse) == 2:
            # memory is Byte(s)
            addr_info.mem_bit = -1
        else:
            # memory is bit
            ss_bit = ss_parse[2].split('-')
            addr_info.mem_bit = string.atoi(ss_bit[0], 16)
            if len(ss_bit) == 1:
                addr_info.mem_len = 1
            else:
                addr_info.mem_len = string.atoi(ss_bit[1], 16) - addr_info.mem_bit
                if addr_info.mem_len > 8:
                    self._assert('Invalid Address(4): %s' % s_addr)

        return addr_info


    def __record(self, row_key, row_data):
        'record Memory Map'

        # check this memory is recorded or not
        if row_key in self.__memtable:
            self._assert('Duplicate Memory definition: "%s"' % row_key)

        # parse address & record this memory
        mem_info = []
        addr_info = self.__parse_address(row_data[0].upper())
        mem_info.append(addr_info)
        mem_info.append(row_data[2])  # memory description
        self.__memtable[row_key] = mem_info

        if self.__debug_enable:
            # debug record log
            s_addr = 'addr:%02X, page_id:%02X, offset:%02X, len:%02X, bit:%-2X' % \
                    (addr_info.mem_addr,
                    addr_info.mem_page_id,
                    addr_info.mem_offset,
                    addr_info.mem_len,
                    addr_info.mem_bit)
            self.__log.write('%-35s :  %-50s  %s\n' % (row_key, s_addr, row_data[2]))


    def __open(self):

        # open excel file
        try:
            self.__xlBook = xlrd.open_workbook(self.__filename)
            self.__sheet = self.__xlBook.sheet_by_name('Internal Memory Map')
        except:
            self._assert('Open Excel File "%s" Fail!' % self.__filename)

        # launch all memory definitions
        for row in range(1, self.__sheet.nrows):
            # read memory data
            mem_data = self.__sheet.row_values(row)
            mem_name = mem_data[0].strip()

            # have reached to the end of excel file, exit
            if mem_name == 'END':
                break

            # if this memory is not empty, record it
            if mem_name:
                self.__record(mem_name, mem_data[1:])


    def CONFIG_SET(self, mem_name, mem_value):

        if not mem_name in self.__memtable:
            self._assert('Memory "%s" is not found in MemoryMap!' % mem_name)

        addr_info = self.__memtable[mem_name][0]

        # create write buffer
        write_buf = []
        if type(mem_value) == type('abcd'):         # string type
            if addr_info.mem_bit != -1:
                # bit memory
                self._assert('Do not know how to write string to bit Memory "%s"!' % mem_name)
            if len(mem_value) != addr_info.mem_len:
                self._assert('The length of Memory "%s" doesnot match input string "%s"!' % (mem_name, mem_value))
            for i in range(addr_info.mem_len):
                write_buf.append(ord(mem_value[i]))
        elif type(mem_value) == type(0x12345678):   # integer type
            if addr_info.mem_bit != -1:
                # bit memory
                if mem_value >= 0xFF:
                    self._assert('A bit Memory "%s" can only be within 1 byte!' % mem_name)
                if (mem_value & (0xFF << addr_info.mem_len)) != 0:
                    self._assert('Input Memory Data too long for bit Memory "%s"! %X' % mem_name)
            else:
                # Byte(s) memory
                if (mem_value >> addr_info.mem_len*8) != 0:
                    self._assert('Input Memory "%s" data "0x%X" length too long!' % (mem_name, mem_value))
            for i in range(addr_info.mem_len):
                tmp_value = (mem_value>>((addr_info.mem_len-i-1)*8)) & 0xFF
                write_buf.append(tmp_value)

        # select page
        if addr_info.mem_page_id != -1:
            (vResult, aBuf) = self.__i2c.RandomRead(addr_info.mem_addr, 0x7F, 1)
            if vResult:
                if aBuf[0] != addr_info.mem_page_id:
                    aBuf[0] = addr_info.mem_page_id
                    if not self.__i2c.RandomWrite(addr_info.mem_addr, 0x7F, aBuf):
                        self._assert('Change Page ID to 0x%02X Fail!' % addr_info.mem_page_id)
            else:
                self._assert('Read Current Selected Page ID Fail!')

        # write memory
        if addr_info.mem_bit == -1:     # Byte(s) Memory
            if not self.__i2c.RandomWrite(addr_info.mem_addr, addr_info.mem_offset, write_buf):
                self._assert('Write Memory "%s" Fail!' % mem_name)
        else:   # bit memory
            # read byte memory, which contains this bit memory
            (vResult, aBuf) = self.__i2c.RandomRead(addr_info.mem_addr, addr_info.mem_offset, 1)
            if not vResult:
                self._assert('Read bit Memory "%s" Fail!' % mem_name)

            # modify memory
            vMask = (~(0xFF << addr_info.mem_len) & 0xFF) << addr_info.mem_bit  # mask for bit memory
            aBuf[0] &= ~vMask
            aBuf[0] |= ((write_buf[0] << addr_info.mem_bit) & vMask)

            # write the modified memory
            if not self.__i2c.RandomWrite(addr_info.mem_addr, addr_info.mem_offset, aBuf):
                self._assert('Write bit Memory "%s" Fail!' % mem_name)


    def CONFIG_GET(self, mem_name):

        if not mem_name in self.__memtable:
            self._assert('Memory "%s" is not found in MemoryMap!' % mem_name)

        addr_info = self.__memtable[mem_name][0]

        # select page
        if addr_info.mem_page_id != -1:
            (vResult, aBuf) = self.__i2c.RandomRead(addr_info.mem_addr, 0x7F, 1)
            if vResult:
                if aBuf[0] != addr_info.mem_page_id:
                    aBuf[0] = addr_info.mem_page_id
                    if not self.__i2c.RandomWrite(addr_info.mem_addr, 0x7F, aBuf):
                        self._assert('Change Page ID to 0x%02X Fail!' % addr_info.mem_page_id)
            else:
                self._assert('Read Current Selected Page ID Fail!')

        # read memory
        if addr_info.mem_bit == -1:     # Byte(s) Memory
            (vResult, aReadBuf) = self.__i2c.RandomRead(addr_info.mem_addr, addr_info.mem_offset, addr_info.mem_len)
            if vResult:
                if addr_info.mem_len == 1:
                    return aReadBuf[0]
                elif addr_info.mem_len == 2:
                    return ((aReadBuf[0] << 8) | aReadBuf[1])
                elif addr_info.mem_len == 4:
                    return ((aReadBuf[0] << 24) | (aReadBuf[1] << 16) | (aReadBuf[2] << 8) | (aReadBuf[3] << 0))
                else:
                    return aReadBuf
            else:
                self._assert('Read Memory "%s" Fail!' % mem_name)
        else:   # bit memory
            # read byte memory, which contains this bit memory
            (vResult, aBuf) = self.__i2c.RandomRead(addr_info.mem_addr, addr_info.mem_offset, 1)
            if not vResult:
                self._assert('Read bit Memory "%s" Fail!' % mem_name)

            # get & return bit memory value
            vMask = (~(0xFF << addr_info.mem_len) & 0xFF) << addr_info.mem_bit  # mask for bit memory
            aBuf[0] &= vMask
            return (aBuf[0] >> addr_info.mem_bit)



class MSA(_I2cSecurity, _MemoryMap):
    'MSA Relelated'

    def __init__(self, filename=None, write_delay=0.2, debug_enable=False):
        'Default I2C write delay is 200ms'
        _MemoryMap.__init__(self, filename, write_delay, debug_enable) # init MemoryMap
        _I2cSecurity.__init__(self)                                    # I2C Security class


    def EnterSecurity(self, level):
        if self.CONFIG_GET('PWD_Entry') == level:
            return

        self.CONFIG_SET('PWD_Entry', self.GetPasswd(level))
        vReadSecurity = self.CONFIG_GET('PWD_Entry')
        if vReadSecurity != level:
            self._assert('Enter Security Level %X Fail! Read:%X, PWD:%X' % (level, vReadSecurity, self.GetPasswd(level)))




'auto-test scripts'
if __name__ == '__main__':
    print '  ----  Start  Testing       ----  '

    map = MSA(r'1620-00100-00_InternalMemoryMap.xls', debug_enable=True)
    print '  ----  MSA Init Done        ----  '

    map.EnterSecurity(map.SECURITY_LEVEL_FACTORY)

    map.CONFIG_SET('RT_SW_TXDIS', 1)
    print '          ->  Get "RT_SW_TXDIS" bit memory: %X' % map.CONFIG_GET('RT_SW_TXDIS')

    map.CONFIG_SET('Debug_System_Ctrl_Polling_Dis', 1)
    print '          ->  Get "Debug_System_Ctrl_Polling_Dis" bit memory: %X' % map.CONFIG_GET('Debug_System_Ctrl_Polling_Dis')

    map.CONFIG_SET('Page_Select', 0x81)
    print '          ->  Get "Page_Select" memory: %02X' % map.CONFIG_GET('Page_Select')

    map.CONFIG_SET('Debug_Cmd_Param1', 0xABCD)
    print '          ->  Get "Debug_Cmd_Param1" memory: %04X' % map.CONFIG_GET('Debug_Cmd_Param1')

    print '  ----  Test Done            ----  '
