#!  /usr/bin/env python
# 
# Copyright (C) 2009  The University of Texas at Austin.
# 
# This file is part of Hydra: A wireless multihop testbed.
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 

from gnuradio.wxgui import form
import wx

class window(wx.Frame):
    wxapp = None  ## stores a wx.App object
    def __init__(self, parent=None, title="Hydra GUI Window", nstatus=2, **kwargs):
        if window.wxapp is None: window.wxapp = wx.App(0)
        wx.Frame.__init__(self, parent, -1, title, **kwargs)
        self.param = {}
        self.CreateStatusBar(nstatus)
        self.make_menu()
        
        self.topbox = wx.BoxSizer(wx.HORIZONTAL)
        #self.topbox.SetOrientation(wx.VERTICAL)    # change orientation
        self.SetSizer(self.topbox)

        self.make_layout()

        self.SetAutoLayout(True)
        self.Fit()

    def AddParam(self, name, callback, *data):
        self.param[name] = Param(self, callback, *data)

    def RemoveParam(self, name):
        if name in self.param: del self.param['name']

    def SetParam(self, name, *data):
        if name in self.param: self.param[name].set_value(*data)

    def GetParam(self, name):
        if name not in self.param: return None
        return self.param[name].get_value()

    def make_menu(self):
        self.mainmenu = wx.MenuBar()
        menu = wx.Menu()
        item = menu.Append (200, 'E&xit', 'Exit')
        self.Bind (wx.EVT_MENU, self.OnExit, item)
        item = menu.Prepend (201, '&About', 'About')
        self.Bind (wx.EVT_MENU, self.OnAbout, item)
        self.mainmenu.Append (menu, "&File")
        self.SetMenuBar (self.mainmenu)

        self.Bind (wx.EVT_CLOSE, self.OnExit)

    def make_label(self, label, box, border=10, alignment=wx.HORIZONTAL):
        nbox = wx.BoxSizer(alignment)
        f = form.static_text_field(self, nbox, value=label)
        box.Add(nbox, 0, wx.ALIGN_LEFT|wx.ALIGN_CENTER_VERTICAL|wx.RIGHT, border=border)
        return f

    def make_layout(self):
        self.button = form.button_with_callback(self, "MyButton", self.OnClick)
        self.topbox.Add(self.button, 0, wx.ALL, border=20)

    """ ==============================
             Callback Functions
        ============================== """
    def OnAbout(self, event):
        dlg = wx.MessageDialog(self, self.GetAboutLabel(), \
                               "About %s"%(self.GetTitle() ), wx.OK | wx.ICON_INFORMATION)
        dlg.ShowModal()
        dlg.Destroy()

    def GetAboutLabel(self):
        return "This GUI sets up a convenient API for wx.Frame."

    def OnExit(self, event):
        self.Destroy()

    def OnClick(self):
        if 'count' not in dir(self): self.count = 0
        else: self.count += 1
        self.button.SetLabel("MyButton %d"%(self.count) )
        self.Fit()

""" Mutable Parameter for window class """
class Param:
    def __init__(self, win, callback, *data):
        self.callback = lambda evt: callback(*evt.data)
        self.win = win
        self.data = data
        self.type = wx.NewEventType()
        win.Connect(-1, -1, self.type, self.callback)
    def set_value(self, *data):
        self.data = data
        evt = DataEvent(self.type, *data)
        wx.PostEvent(self.win, evt)
        del evt
    def get_value(self):
        return self.data

class DataEvent(wx.PyEvent):
    def __init__(self, type, *data):
        wx.PyEvent.__init__(self)
        self.SetEventType(type)
        self.data = data
    def Clone(self):
        self.__class__(self.GetId() )

"""
class choicebox_field:
    Creates a wx.ChoiceBox using the conventions in gnuradio.wxgui.form.

    The set and get methods inherited from form.field pass and return strings. 
"""

class choicebox_field(form.field):
    def __init__(self, parent=None, sizer=None, label=None, value=None,
                 converter=form.identity_converter(), callback=None, choices=[],
                 weight=1, tipstring=None):
        
        new_id = wx.NewId()
        w = wx.Choice(parent, new_id , choices = choices)
        if tipstring: w.SetToolTipString(tipstring)
        self.f = self._pair_with_label(w, parent=parent, sizer=sizer, label=label, weight=weight)
        if callback:
            wx.EVT_CHOICE (parent, new_id, lambda evt: callback(evt.GetString() ) )
        form.field.__init__(self, converter, value)
    
    def _get_prim_value(self):
        return self.f.GetStringSelection()

    def _set_prim_value(self, v):
        self.f.SetStringSelection(str(v))

    def set_choices (self, choices):
        self.f.Clear()
        self.f.Append([str(x) for x in choices])

    def get_choices (self):
        return [str(s) for s in self.f.GetStrings()]

class Divider(wx.Panel):
    def __init__(self, *args, **kwargs):
        if 'size' not in kwargs: kwargs['size'] = (0.5,0.5)
        #if 'style' in kwargs: kwargs['style'] |= wx.FULL_REPAINT_ON_RESIZE
        #else: kwargs['style'] = wx.FULL_REPAINT_ON_RESIZE
        wx.Panel.__init__(self, *args, **kwargs)
