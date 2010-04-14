
import wx

dlg = wx.MessageDialog(None, 'Is this the coolest thing ever!',
                      'MessageDialog', wx.YES_NO | wx.ICON_QUESTION)
result = dlg.ShowModal()
dlg.Destroy()
