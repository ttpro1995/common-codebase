#!/bin/env python

''' This is the second basic WxPython Application '''

import wx

class MyFrame(wx.Frame):
    pass

if __name__ == '__main__':
    app   = wx.PySimpleApp()
    frame = MyFrame(parent=None, title='Second App')
    frame.Show(True)
    app.MainLoop()
