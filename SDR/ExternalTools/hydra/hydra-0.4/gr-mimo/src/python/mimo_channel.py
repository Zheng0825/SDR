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

class MimoChannel(PyHydra.channel):
    TYPE_OPTIONS = ["actual", "effective"]
    def __init__(self, title="Hydra Mimo Channel", **kwargs):
        PyHydra.channel.__init__(self, title=title, **kwargs)

    def make_layout(self):
        bspace = 10
        PyHydra.channel.make_layout(self)

        """ Channel Type Radio Button """
        nbox = wx.BoxSizer(wx.VERTICAL)
        self.channel_type = form.radiobox_field(self, nbox, \
                "Channel Type", value="actual", \
                callback=self.SetChannelType,
                choices = MimoChannel.TYPE_OPTIONS, major_dimension=0)
        self.channel_type.f.SetLabel("")
        self.AddParam('channel_type', self.SetChannelType)
        self.cbox.Add(nbox, 0, wx.EXPAND|wx.TOP, border=bspace)

    def SetChannelType(self, x):
        s, m = str(x).lower(), None
        if s in MimoChannel.TYPE_OPTIONS:
            m = MimoChannel.TYPE_OPTIONS.index(s)+1
        elif isinstance(x,int):
            if (x>1): m = 2
            else:     m = 1
            self.channel_type.set_value(MimoChannel.TYPE_OPTIONS[m-1])
        if (m is not None):
            if hasattr(self.GetParent(), 'phy'):
                if hasattr(self.GetParent().phy, 'rx'):
                    if hasattr(self.GetParent().phy.rx,'set_plot_channel'):
                        self.GetParent().phy.rx.set_plot_channel(m)
