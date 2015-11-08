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
from window import window, choicebox_field
import wx

from helper import *
from mplbase import MPLBase

#from matplotlib.axes import Subplot
import numpy


class constellation(window):
    def __init__(self, title="Hydra Constellation", **kwargs):
        self.nplot = 0
        self.plot = []  # contains pointer to plot
        window.__init__(self, title=title, **kwargs)

    def make_menu(self):
        window.make_menu(self)

    def make_layout(self):
        self.topbox.SetOrientation(wx.HORIZONTAL)
        self.button = form.button_with_callback(self, "Add Plot", self.AddPlot)
        self.topbox.Add(self.button, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, border=20)
        self.AddPlot()
        self.AddParam('format', self.SetFormat)
        self.AddParam('color',  self.SetColor)
        self.AddParam('shape',  self.SetShape)
        self.AddParam('data',   self.SetData)
        self.Fit()

    def AddPlot(self):
        pid = self.nplot
        
        ## Make Plotter Component
        vbox = wx.BoxSizer(wx.VERTICAL)
        self.plot.append(Plotter(self, vbox, SID=self.nplot) )
        
        self.nplot += 1
        self.topbox.Add(vbox, 1, wx.GROW)
        self.Fit()

    def SetData(self, SID, data):
        for p in self.plot:
            if (p.SID == SID): p.SetData(SID, data)

    def SetSID(self, PID, id):
        if PID<self.nplot: self.plot[PID].SetSID(id)

    def SetFormat(self, PID, f):
        if PID<self.nplot: self.plot[PID].SetFormat(f)

    def SetColor(self, PID, c):
        if PID<self.nplot: self.plot[PID].SetColor(c)

    def SetShape(self, PID, s):
        if PID<self.nplot: self.plot[PID].SetShape(s)

    def GetAboutLabel(self):
        return "Constellation plotting GUI"

    def OnExit(self, evt):
        if 'constellation' in dir(self.GetParent() ):
            self.GetParent().constellation = None
        window.OnExit(self, evt)

class Plotter(MPLBase):
    shape_choices = ['+', 'x', '.', 'v']
    color_choices = ['b', 'r', 'g', 'c']
    # Set any of the below parameters to None to use default values
    axis_range = [-1.7, 1.7, -1.7, 1.7]
    axis_xticks = numpy.arange(-1.0, 1.01, 1.0)
    axis_yticks = numpy.arange(-1.0, 1.01, 1.0)
    marker_size = 7.5
    marker_width = 1.0
    def __init__(self, win, sizer, SID=0, color=None, shape=None):
        MPLBase.__init__(self)
        self.win = win
        self.sizer = sizer
        
        # configure parameters
        self.SID = SID
        self.color = color
        self.shape = shape
        self.data = numpy.array([complex(0,0)])
        if color==None:
            k = self.SID%len(Plotter.color_choices)
            self.color = Plotter.color_choices[k]
        if shape==None:
            self.shape = Plotter.shape_choices[0]
        self.format = self.color + self.shape
        
        # create fig, canvas, toolbar
        figsize, dpi = (3.2,3.2), 100
        self.make_plot(self.win, figsize=figsize, dpi=dpi)

        # create subplot with data
        self.make_subplot()
        self.toolbar.update()

        # compose components
        vbox = wx.BoxSizer(wx.VERTICAL)
        vbox.Add(self.canvas, 1, wx.LEFT|wx.TOP|wx.GROW)
        # Best to allow the toolbar to resize!
        vbox.Add(self.toolbar, 0, wx.GROW)

        # create common configuration buttons
        self._make_plot_common(vbox)
        self.sizer.Add(vbox, 1, wx.GROW)

    def make_subplot(self):
        self.axes = self.fig.add_subplot(111)
        self.axes.set_position([0.15,0.14,0.78,0.78])
        x = numpy.array([numpy.complex(0,0)] )
        self.lines = self.axes.plot(x.real, x.imag, self.format)
        self.SetPlotProperties()
        self.data = x

    def _make_plot_common(self, sizer):
        hbox  = wx.BoxSizer(wx.HORIZONTAL)
        #idbox = wx.BoxSizer(wx.HORIZONTAL)
        #cbox  = wx.BoxSizer(wx.HORIZONTAL)
        #sbox  = wx.BoxSizer(wx.HORIZONTAL)
        ## Add int box for Stream ID
        self.sidfield = form.int_field(self.win, hbox, "SID", self.SID, self.SetSID)
        self.sidfield.f.SetToolTipString("Select Spatial Stream")
        ## Add choice box for color
        self.cfield = choicebox_field(self.win, hbox, callback = self.SetColor, \
                                 value = self.color, choices = Plotter.color_choices)
        self.cfield.f.SetToolTipString("Select Marker Color")
        ## Add choice box for shape
        self.sfield = choicebox_field(self.win, hbox, callback = self.SetShape, \
                                 value = self.shape, choices = Plotter.shape_choices)
        self.sfield.f.SetToolTipString("Select Marker Shape")
        self.SetColor(self.color)
        self.SetShape(self.shape)
        sizer.Add(hbox, 0, wx.ALIGN_LEFT|wx.LEFT, border=10)

    def SetData(self, SID, data):
        if self.SID == SID:
            self.data = data
            x = numpy.array(self.data)
            self.lines[0].set_data(x.real, x.imag)
            self.SetShape(self.shape)
            self.SetColor(self.color)

    def SetSID(self, id=None):
        if id==None:
            self.SID = self.sidfield.get_value()
        else:
            self.SID = id
            self.sidfield.set_value(self.SID)
        k = self.SID%len(Plotter.color_choices)
        self.SetColor(Plotter.color_choices[k])
        self.data = numpy.array([complex(0,0)])
        self.SetData(self.SID, self.data)

    def SetFormat(self, f):
        # re-plot with new format
        self.format = f
        self.axes.hold(False)
        x = numpy.array(self.data)
        self.lines = self.axes.plot(x.real, x.imag, self.format)
        if len(f)>0:
            if f[0] in Plotter.color_choices: self.SetColor(f[0])
        if len(f)>1:
            if f[1] in Plotter.shape_choices: self.SetShape(f[1])
        self.SetPlotProperties()
        self.Draw()

    def SetColor(self, c=None):
        if c==None:
            self.color = self.cfield.get_value()
        else:
            self.color = c
            if c in Plotter.color_choices: self.cfield.set_value(c)
        self.lines[0].set_color(self.color)
        self.format = self.color + self.shape
        self.Draw()

    def SetShape(self, s=None):
        if s==None:
            self.shape = self.sfield.get_value()
        else:
            self.shape = s
            if s in Plotter.shape_choices: self.sfield.set_value(s)
        self.lines[0].set_marker(self.shape)
        if Plotter.marker_width: self.lines[0].set_markeredgewidth(Plotter.marker_width)
        if Plotter.marker_size:  self.lines[0].set_markersize(Plotter.marker_size)
        self.format = self.color + self.shape
        self.Draw()

    def SetPlotProperties(self):
        self.axes.axis(Plotter.axis_range)
        self.axes.grid(True)
        if Plotter.axis_range is not None:  self.axes.axis(Plotter.axis_range)
        if Plotter.axis_xticks is not None: self.axes.set_xticks(Plotter.axis_xticks)
        if Plotter.axis_yticks is not None: self.axes.set_yticks(Plotter.axis_yticks)
        self.axes.set_xlabel('I')
        self.axes.set_ylabel('Q', rotation=0)
