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
from hydra.PyHydra import Callable
from window import window, Divider
import wx
import sys
import time

class AddrGUI(window):
    def __init__(self, client, title="Address Client GUI", **kwargs):
        self.client = client
        self.addrlist = []
        window.__init__(self, title=title, **kwargs)

    def make_layout(self):
        bspace = 10
        self.topbox.SetOrientation(wx.HORIZONTAL)
        self.SetPosition((200,100))
        vbox = wx.BoxSizer(wx.VERTICAL)
        """ Title """
        hbox = wx.BoxSizer(wx.VERTICAL)
        self.addrtitle = form.static_text_field(self, hbox)
        self.AddParam('title', self.SetAddrTitle)
        self.SetParam('title', "Address List : "+str(self.client.local) )
        vbox.Add(hbox, 0, wx.ALIGN_LEFT|wx.ALL, border=0)

        """ AddressList """
        hbox = wx.BoxSizer(wx.VERTICAL)
        self.refreshbox = form.button_with_callback(self, "Refresh", self.RefreshList)
        hbox.Add(self.refreshbox, wx.ALIGN_LEFT|wx.ALL, border=0)
        #self.optbox = form.static_text_field(self, hbox)
        self.optbox = wx.BoxSizer(wx.VERTICAL)
        hbox.Add(self.optbox, 0, wx.ALIGN_TOP|wx.ALIGN_BOTTOM, border=10)
        self.AddParam('refresh', self.RefreshList)
        self.SetParam('refresh')
        vbox.Add(hbox, 0, wx.ALIGN_LEFT|wx.ALL, border=0)
        vbox.AddSpacer( (10,10) )

        self.topbox.Add(vbox, wx.GROW|wx.ALL, border=bspace)

    def SetAddrTitle(self, s):
        banner = "==============================\n"
        self.addrtitle.set_value(banner+str(s)+'\n'+banner)
        self.Fit()

    def RefreshList(self, *args):
        self.addrlist = self.client.server_request()
        # clear optbox
        children = self.optbox.GetChildren()
        for k in range(len(children)):
            children[k].Show(False)
            self.optbox.Remove(0)
        # create new boxes
        aspace = 10
        for k in range(len(self.addrlist) ):
            newopt = self.CreateOpt(self.addrlist[k])
            self.optbox.Add(newopt, wx.ALIGN_LEFT|wx.ALL, border=0)
        self.Fit()

    def CreateOpt(self, addr):
        aspace = 10
        newopt = form.button_with_callback(self, \
                        " "*aspace + "%s"%(str(addr) ) + " "*aspace, \
                        lambda *args: self.SetDest(addr) )
        return newopt

    def SetDest(self, dest=""):
        print "TRYING TO SET DEST =", dest
        if dest in self.addrlist:
            self.client.put_destaddr(dest)
            time.sleep(1.0)
            self.OnExit(None)

    def GetAboutLabel(self):
        return "This GUI sets up an AddrClient connection to an AddrServer."

    def OnExit(self, event):
        if self.client:
            if hasattr(self.client, 'put_destaddr'): self.client.put_destaddr()
        self.Destroy()
