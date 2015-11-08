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
from hydra.PyHydra import window, choicebox_field
from hydra.PyHydra import MPLBase
import wx

# set up backend for matplotlib
from hydra.PyHydra.gui.helper import *

#from matplotlib.axes import Subplot
import numpy


class MimoCSI(window):
    CODEBOOK_CHOICES = ["Grassmanian"]
    def __init__(self, title="Hydra MIMO Limited Feedback CSI", **kwargs):
        self.nchan = 0
        self.plot = None

        self.phyrx = None
        self.subchannel = None
        self.period = None
        self.codebook = None
        self.codebits = None
        self.lowercase_codebook_choices = [s.lower() for s in MimoCSI.CODEBOOK_CHOICES]
        window.__init__(self, title=title, **kwargs)

    def make_layout(self):
        bspace=10
        self.topbox.SetOrientation(wx.HORIZONTAL)
        self.controlbox = wx.BoxSizer(wx.VERTICAL)
        self.AddPlot()
        
        ## Create divider
        self.divider = Divider(self)
        self.divider.SetBackgroundColour(wx.Colour(200,200,200) )
        self.topbox.Add(self.divider, 0, wx.EXPAND|wx.GROW)

        # Get PHY RX
        if hasattr(self.GetParent(), 'phy'):
            if hasattr(self.GetParent().phy, 'rx'):
                self.phyrx = self.GetParent().phy.rx

        """ Subchannel """
        self.set_subchannel = lambda *args: None
        self.subchannel = 0
        if hasattr(self.phyrx, 'lfb_subchannel'):
            self.subchannel = self.phyrx.lfb_subchannel()
        if hasattr(self.phyrx, 'set_lfb_subchannel'):
            self.set_subchannel = self.phyrx.set_lfb_subchannel
        box = wx.BoxSizer(wx.VERTICAL)
        self.make_label("Subchannel", box)
        self.fsubchannel = form.int_field(self, box, value=self.subchannel, \
                                      callback=self.SetSubchannel)
        self.AddParam('subchannel', self.SetSubchannel)
        self.controlbox.Add(box, 0, wx.TOP, border=bspace)

        """ Period """
        self.set_period = lambda *args: None
        self.period = 0
        if hasattr(self.phyrx, 'lfb_period'):
            self.period = self.phyrx.lfb_period()
        if hasattr(self.phyrx, 'set_lfb_period'):
            self.set_period = self.phyrx.set_lfb_period
        box = wx.BoxSizer(wx.VERTICAL)
        self.make_label("Period", box)
        self.fperiod = form.int_field(self, box, value=self.period, \
                                      callback=self.SetPeriod)
        self.AddParam('period', self.SetPeriod)
        self.controlbox.Add(box, 0, wx.TOP|wx.BOTTOM, border=bspace)

        """ Codebook """
        self.set_codebook = lambda *args: None
        self.codebook = 0
        if hasattr(self.phyrx, 'lfb_codebook'):
            self.codebook = self.phyrx.lfb_codebook()
        if hasattr(self.phyrx, 'set_lfb_codebook'):
            self.set_codebook = self.phyrx.set_lfb_codebook
        box = wx.BoxSizer(wx.HORIZONTAL)
        self.fcodebook = choicebox_field(self, box, \
                value=MimoCSI.CODEBOOK_CHOICES[0], callback=self.SetCodebook, \
                choices = MimoCSI.CODEBOOK_CHOICES)
        self.fcodebook.f.SetToolTipString("Set limited feedback codebook")
        self.AddParam('codebook', self.SetCodebook)
        self.controlbox.Add(box, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.TOP|wx.BOTTOM, border=bspace)

        """ Codebook Bits """
        self.set_codebit = lambda *args: None
        self.codebits = 3
        if hasattr(self.phyrx, 'lfb_codebits'):
            self.codebits = self.phyrx.lfb_codebits()
        if hasattr(self.phyrx, 'set_lfb_codebits'):
            self.set_codebits = self.phyrx.set_lfb_codebits
        box = wx.BoxSizer(wx.VERTICAL)
        self.make_label("Codebook Bits", box)
        self.fcodebits = form.int_field(self, box, value=self.codebits, \
                                      callback=self.SetCodebits)
        self.AddParam('codebits', self.SetCodebits)
        self.controlbox.Add(box, 0, wx.TOP, border=bspace)

        self.AddParam('data', self.SetData)
        self.topbox.Add(self.controlbox, 0, wx.ALL, border=bspace)

    def AddPlot(self):
        bspace=10
        ## Make CSIPlotter Component
        vbox = wx.BoxSizer(wx.VERTICAL)
        self.plot = CSIPlot(self, vbox)
        self.topbox.Add(vbox, 1, wx.EXPAND|wx.ALL, border=bspace)
        self.Fit()

    def SetSubchannel(self, n=None):
        c = self.subchannel
        if n is None:
            self.subchannel = self.fsubchannel.get_value()
        else:
            self.subchannel = n
            self.fsubchannel.set_value(n)
        if c != self.subchannel:
            self.set_subchannel(self.subchannel)

    def SetPeriod(self, n=None):
        p = self.period
        if n is None:
            self.period = self.fperiod.get_value()
        else:
            self.period = n
            self.fperiod.set_value(n)
        if p != self.period:
            self.set_period(self.period)

    def SetCodebook(self, c=None):
        b = self.codebook
        cstr = MimoCSI.CODEBOOK_CHOICES[0]
        if c==None:
            cstr = self.fcodebook.get_value()
            self.codebook = self.lowercase_codebook_choices.index(cstr.lower())
        else:
            if str(c).lower() in self.lowercase_codebook_choices:
                self.codebook = self.lowercase_codebook_choices.index(str(c).lower() )
            elif isinstance(c, int):
                if (c<len(MimoCSI.CODEBOOK_CHOICES) ):
                    self.codebook = c
            self.fcodebook.set_value(MimoCSI.CODEBOOK_CHOICES[self.codebook])
        if self.codebook != b:
            self.set_codebook(self.codebook)

    def SetCodebits(self, n=None):
        c = self.codebits
        if n is None:
            self.codebits = self.fcodebits.get_value()
        else:
            self.codebits = n
            self.fcodebits.set_value(n)
        if c != self.codebits:
            self.set_codebits(self.codebits)

    def SetData(self, data, codebook=None, codebits=None):
        if codebook is not None:
            if codebook != self.codebook:
                self.SetCodebook(codebook)
        if codebits is not None:
            if codebits != self.codebits:
                self.SetCodebits(codebits)
        self.plot.hold(False)
        x = numpy.array(range(len(data) ) )
        y = numpy.array(data)+0.0001
        self.plot.draw(x,y)
        self.plot.SetPlotProperties()

    def GetAboutLabel(self):
        return "MIMO Limited Feedback CSI GUI"

    def OnExit(self, evt):
        if 'csi' in dir(self.GetParent() ):
            self.GetParent().csi = None
        window.OnExit(self, evt)

class Divider(wx.Panel):
    def __init__(self, *args, **kwargs):
        if 'size' not in kwargs: kwargs['size'] = (0.5,0.5)
        #if 'style' in kwargs: kwargs['style'] |= wx.FULL_REPAINT_ON_RESIZE
        #else: kwargs['style'] = wx.FULL_REPAINT_ON_RESIZE
        wx.Panel.__init__(self, *args, **kwargs)

class CSIPlot(MPLBase):
    def __init__(self, win, sizer):
        MPLBase.__init__(self)
        self.win = win
        self.sizer = sizer
        self.fig = None
        self.lines = []

        # create fig, canvas, toolbar
        figsize, dpi = (4,3.5), 100
        self.make_plot(self.win, figsize=figsize, dpi=dpi)

        # create subplot with data
        self.make_subplot()
        self.toolbar.update()

        # compose components
        vbox = wx.BoxSizer(wx.VERTICAL)
        vbox.Add(self.canvas, 1, wx.LEFT|wx.TOP|wx.GROW)
        # Best to allow the toolbar to resize!
        vbox.Add(self.toolbar, 0, wx.GROW)
        self.sizer.Add(vbox, 1, wx.GROW)

    def make_subplot(self):
        self.axes = self.fig.add_subplot(111)
        self.axes.set_position([0.13,0.13,0.82,0.80])
        self.hold(False)
        x = numpy.array(range(8))
        self.bar(x, x*0.0)
        self.SetPlotProperties()

    def hold(self, *args):
        self.axes.hold(*args)
        if not self.axes.ishold():
            self.lines = []

    def bar(self, *args, **kwargs):
        self.axes.bar(*args, **kwargs)

    def draw(self, *args, **kwargs):
        self.bar(*args, **kwargs)

    def SetPlotProperties(self):
        self.axes.grid(True)
        #x, y = self.xvals, self.yrange
        #self.axes.set_xlim(x[0], x[len(x)-1] )
        #self.axes.set_ylim(y[0], y[1])
        self.axes.set_xlabel('Code Indices')
        self.Draw()
