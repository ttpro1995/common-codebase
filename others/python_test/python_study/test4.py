#!/usr/bin/env python

import wx
import sys

class Frame(wx.Frame):

    def __init__(self, parent, id, title):
        print "Frame __init__"
        wx.Frame.__init__(self, parent, id, title)

class App(wx.App):

    def __init__(self, redirect=True, filename=None):
        wx.App.__init__(self, redirect, filename)

    def OnInit(self):
        print "OnInit"
        self.frame = Frame(parent=None, id=-1, title='Startup')
        self.frame.Show()
        self.SetTopWindow(self.frame)
        print sys.stderr, "Set Top Window Done!"
        return True

    def OnExit(self):
        print "OnExit"

if __name__ == '__main__':
    app = App(True, '~log.txt')
    print "before MainLoop"
    app.MainLoop()
