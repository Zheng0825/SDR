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
from hydra.PyHydra import window, choicebox_field, Divider
from hydra.PyHydra import MPLBase
import wx
import sys

# set up backend for matplotlib
from hydra.PyHydra.gui.helper import *

#from matplotlib.axes import Subplot
import numpy


class MimoRateHistory(window):
    def __init__(self, title="MIMO Rate History", **kwargs):
        self.plot = None

        self.phyrx = None
        window.__init__(self, title=title, **kwargs)

    def make_layout(self):
        bspace=10
        self.topbox.SetOrientation(wx.VERTICAL)
        self.AddPlot()
        
        self.AddParam('data',   self.SetData)

    def AddPlot(self):
        ## Make ChannelPlotter Component
        vbox = wx.BoxSizer(wx.VERTICAL)
        self.plot = RateHistoryPlot(self, vbox)
        self.topbox.Add(vbox, 1, wx.GROW)
        self.Fit()

    def SetData(self, data, nrates):
        if len(data) != nrates: return
        self.data = numpy.array(data)
        self.UpdatePlot()

    def UpdatePlot(self):
        nrates = len(self.data)
        x = numpy.array(range(nrates) )
        y = numpy.array(self.data)
        self.plot.draw(x, y)
        self.plot.SetPlotProperties()

    def GetAboutLabel(self):
        return "MIMO Rate History GUI"

    def OnExit(self, evt):
        if 'rate_history' in dir(self.GetParent() ):
            self.GetParent().rate_history = None
        window.OnExit(self, evt)

class RateHistoryPlot(MPLBase):
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
        """ Initialize graph """
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
        self.axes.set_xlim(0, 16 )
        #self.axes.set_ylim(y[0], y[1])
        self.axes.set_xlabel('Modulation Coding Scheme Index')
        self.Draw()
