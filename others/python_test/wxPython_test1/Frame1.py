#Boa:Frame:Frame1

import wx

def create(parent):
    return Frame1(parent)

[wxID_FRAME1, wxID_FRAME1BUTTON1, wxID_FRAME1TXTBUF, 
] = [wx.NewId() for _init_ctrls in range(3)]

class Frame1(wx.Frame):
    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wx.Frame.__init__(self, id=wxID_FRAME1, name='', parent=prnt,
              pos=wx.Point(304, 269), size=wx.Size(400, 250),
              style=wx.DEFAULT_FRAME_STYLE, title='Frame1')
        self.SetClientSize(wx.Size(392, 214))

        self.txtBuf = wx.TextCtrl(id=wxID_FRAME1TXTBUF, name='txtBuf',
              parent=self, pos=wx.Point(160, 48), size=wx.Size(100, 22),
              style=0, value='')

        self.button1 = wx.Button(id=wxID_FRAME1BUTTON1, label='button1',
              name='button1', parent=self, pos=wx.Point(176, 96),
              size=wx.Size(75, 24), style=0)
        self.button1.Bind(wx.EVT_BUTTON, self.OnButton1Button,
              id=wxID_FRAME1BUTTON1)

    def __init__(self, parent):
        self._init_ctrls(parent)

    def OnButton1Button(self, event):
        self.txtBuf.Value = "Hello, world!"
