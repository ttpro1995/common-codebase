
import wx

class App(wx.App):
    def OnInit(self):
        frame = wx.Frame(parent=None, title='WxPython Test App')
        frame.Show()
        return True

app = App()
app.MainLoop()
