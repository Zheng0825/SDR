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
from hydra import PyHydra
import wx

class MimoAdapt(PyHydra.window):
    def __init__(self, title="MIMO Adaptation Controls", **kwargs):
        PyHydra.window.__init__(self, title=title, **kwargs)

    def GetSNRPenalty(self):
        p = 0.0
        if hasattr(self.GetParent(), 'phy'):
            if hasattr(self.GetParent().phy, 'rx'):
                if hasattr(self.GetParent().phy.rx, 'snr_penalty'):
                    p = self.GetParent().phy.rx.snr_penalty()
        return p

    def GetConditionPenalty(self):
        p = 1.0
        if hasattr(self.GetParent(), 'phy'):
            if hasattr(self.GetParent().phy, 'rx'):
                if hasattr(self.GetParent().phy.rx, 'condition_penalty'):
                    p = self.GetParent().phy.rx.condition_penalty()
        return p

    def SetSNRPenalty(self, p=None):
        self.psnr = 0.0
        if p is None:
            self.psnr = self.fsnr.get_value()
        else:
            self.psnr = p
            self.fsnr.set_value(p)
        if hasattr(self.GetParent(), 'phy'):
            if hasattr(self.GetParent().phy, 'rx'):
                if hasattr(self.GetParent().phy.rx, 'set_snr_penalty'):
                    self.GetParent().phy.rx.set_snr_penalty(self.psnr)

    def SetConditionPenalty(self, p=None):
        self.pcond = 1.0
        if p is None:
            self.pcond = min(1.0, max(0, self.fcond.get_value()) )
            self.fcond.set_value(self.pcond)
        else:
            self.pcond = min(1.0, max(0, p) )
            self.fcond.set_value(self.pcond)
        if hasattr(self.GetParent(), 'phy'):
            if hasattr(self.GetParent().phy, 'rx'):
                if hasattr(self.GetParent().phy.rx, 'set_condition_penalty'):
                    self.GetParent().phy.rx.set_condition_penalty(self.pcond)

    def make_layout(self):
        bspace = 10
        self.topbox.SetOrientation(wx.VERTICAL)
        vbox = wx.BoxSizer(wx.VERTICAL)

        """ SNR Penalty """
        box = wx.BoxSizer(wx.HORIZONTAL)
        self.make_label("SNR Penalty (dB)", box, 81)
        p = self.GetSNRPenalty()
        print "SNR Penalty =", p
        self.fsnr = form.float_field(self, box, value=p, \
                                callback=self.SetSNRPenalty)
        self.AddParam('snr penalty', self.SetSNRPenalty)
        vbox.Add(box, 0, wx.ALIGN_LEFT|wx.BOTTOM, border=bspace)

        """ Condition Penalty """
        box = wx.BoxSizer(wx.HORIZONTAL)
        self.make_label("Condition Penalty (0.0-1.0)", box, 20)
        p = self.GetConditionPenalty()
        print "Condition Penalty =", p
        self.fcond = form.float_field(self, box, value=p, \
                                callback=self.SetConditionPenalty)
        self.AddParam('condition penalty', self.SetConditionPenalty)
        vbox.Add(box, 0, wx.ALIGN_LEFT)
        
        self.topbox.Add(vbox, 0, wx.ALIGN_LEFT|wx.ALL, border=2*bspace)
        self.Fit()

    def GetAboutLabel(self):
        return "This GUI can be used to control parameters used in MIMO Adaptation."

    def OnExit(self, evt):
        if 'adapt' in dir(self.GetParent() ):
            self.GetParent().adapt = None
        PyHydra.window.OnExit(self, evt)
