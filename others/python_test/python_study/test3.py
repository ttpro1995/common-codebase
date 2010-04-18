#!/bin/env python

''' This is the 3rd basic WxPython Application '''

import wx

class MyFrame(wx.Frame):
    pass

class MyApp(wx.PySimpleApp):
    def OnInit(self):
        self.frame = MyFrame(parent=None, title='App 3')
        self.frame.Show(True)
        self.SetTopWindow(self.frame)
        return True

def main():
    app   = MyApp()
    app.MainLoop()

if __name__ == '__main__':
    main()
