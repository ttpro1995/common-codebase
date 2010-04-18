#!/bin/env python

''' This is the first basic WxPython Application '''

import wx

if __name__ == '__main__':
    app   = wx.PySimpleApp()
    frame = wx.Frame(parent=None, title='First App')
    frame.Show(True)
    app.MainLoop()
