#!/usr/bin/env python

''' WxPython App '''

import wx

class MenuBar(wx.MenuBar):
    def __init__(self):
        menuBar  = wx.MenuBar()

        # Append Menu for File Menu
        fileMenu = wx.Menu()
        fileMenu.Append(wx.NewId(), "&Open",  "")
        fileMenu.Append(wx.NewId(), "&Save",  "")
        fileMenu.AppendSeparator()
        fileMenu.Append(wx.NewId(), "&Exit",  "")
        menuBar.Append(fileMenu, "&File")

        # Append Menu for Edit Menu
        editMenu = wx.Menu()
        editMenu.Append(wx.NewId(), "&Copy",  "")
        editMenu.Append(wx.NewId(), "&Cut",   "")
        editMenu.Append(wx.NewId(), "&Paste", "")
        editMenu.AppendSeparator()
        editMenu.Append(wx.NewId(), "&Options", "")
        menuBar.Append(editMenu, "&Edit")

        self.SetMenuBar(menuBar)


class Frame(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, parent=None, id=-1, title='WxPython App', pos=(150, 150),
                        size=(500,500),
                        style=wx.DEFAULT_FRAME_STYLE ^ (wx.RESIZE_BORDER | wx.MAXIMIZE_BOX))

        # create Panel
        self._create_Panel()

        # create MenuBar
        self._create_MenuBar()

        # create StatusBar
        self._create_StatusBar()

        # create Buttons
        self._create_Button()

        self.Bind(wx.EVT_CLOSE, self.OnCloseWindow)


    def _create_Panel(self):
        self._panel  = wx.Panel(self)
        self._panel.SetBackgroundColour('White')


    def _create_MenuBar(self):
        self._menuBar  = wx.MenuBar()

        # Append Menu for File Menu
        self._fileMenu = wx.Menu()
        self._fileMenu.Append(wx.NewId(), "&Open",  "")
        self._fileMenu.Append(wx.NewId(), "&Save",  "")
        self._fileMenu.AppendSeparator()
        self._fileMenu.Append(wx.NewId(), "&Exit",  "")
        self._menuBar.Append(self._fileMenu, "&File")

        # Append Menu for Edit Menu
        self._editMenu = wx.Menu()
        self._editMenu.Append(wx.NewId(), "&Copy",  "")
        self._editMenu.Append(wx.NewId(), "&Cut",   "")
        self._editMenu.Append(wx.NewId(), "&Paste", "")
        self._editMenu.AppendSeparator()
        self._editMenu.Append(wx.NewId(), "&Options", "")
        self._menuBar.Append(self._editMenu, "&Edit")

        self.SetMenuBar(self._menuBar)


    def _create_StatusBar(self):
        self._statusBar = self.CreateStatusBar()

    def _create_Button(self):
        self._buttonText   = wx.Button(self._panel, label="Enter Text", pos=(200,380), size=(80,30))
        self._buttonChoice = wx.Button(self._panel, label="Select",     pos=(300,380), size=(80,30))
        self._buttonQuit   = wx.Button(self._panel, label="Quit",       pos=(400,380), size=(80,30))

        self.Bind(wx.EVT_BUTTON, self.OnEnterText, self._buttonText)
        self.Bind(wx.EVT_BUTTON, self.OnChoice,    self._buttonChoice)
        self.Bind(wx.EVT_BUTTON, self.OnQuit,      self._buttonQuit)



# Event dealing
    def OnEnterText(self, event):
        text = wx.TextEntryDialog(self, "Please Enter Your Name:", 'Your Name')
        if text.ShowModal() == wx.ID_OK:
            name = text.GetValue()
            msgBox = wx.MessageDialog(self, 'Hello, '+name, 'Welcome', wx.OK)
            msgBox.ShowModal()

    def OnChoice(self, event):
        dlg = wx.SingleChoiceDialog(self, 'Please select your age:',
                                    'Your Age',
                                    ['<20', '20-40', '40-60', '60-80', '>80'])
        if dlg.ShowModal() == wx.ID_OK:
            response = dlg.GetStringSelection()
            msgBox = wx.MessageDialog(self, 'You select: '+response, 'Information', wx.OK)
            msgBox.ShowModal()

    def OnQuit(self, event):
        msgBox = wx.MessageDialog(self, 'Are you sure want to exit?',
                                  'Question', wx.YES_NO | wx.ICON_QUESTION)
        if msgBox.ShowModal() == wx.ID_YES:
            self.Close(True)


    def OnCloseWindow(self, event):
        self.Destroy()


class App(wx.App):
    def OnInit(self):
        self.frame = Frame()
        self.frame.Show()
        self.SetTopWindow(self.frame)
        return True

def main():
    app = App()
    app.MainLoop()

if __name__ == '__main__':
    main()
