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
from window import window, choicebox_field, Divider
import wx

from helper import *
from mplbase import MPLBase

#from matplotlib.axes import Subplot
import numpy
import sys

class channel(window):
    def __init__(self, title="Hydra Channel", **kwargs):
        self.nchan = 0
        self.plot = None
        self.nsub = 64
        self.ymin, self.ymax = -60, 10
        self.chan = []  # contains pointer to plot
        self.data = {}
        self.chanUprate = 2
        window.__init__(self, title=title, **kwargs)

    def make_menu(self):
        window.make_menu(self)
        menu = wx.Menu()
        ## Menu Item for Adding New Channel
        id = wx.NewId()
        item = menu.Append (id, '&Add Channel', 'Add Channel')
        self.Bind (wx.EVT_MENU, self.AddChannel, item)
        self.mainmenu.Append(menu, "&Options")

    def make_layout(self):
        bspace=10
        self.topbox.SetOrientation(wx.VERTICAL)
        self.bottombox = wx.BoxSizer(wx.HORIZONTAL)
        self.AddPlot()
        
        ## Controls for plotting go in bottombox
        self.topbox.Add(self.bottombox, 0, wx.EXPAND)
        # Create box for ChannelControl objects
        self.dbox = wx.BoxSizer(wx.VERTICAL)
        banner = wx.BoxSizer(wx.HORIZONTAL)
        banner.AddSpacer((40,10) )
        self.make_label("Nrx", banner, 57)
        self.make_label("Ntx", banner, 80)
        self.make_label("Format", banner, 10)
        self.dbox.Add(banner, 0, wx.EXPAND)
        self.bottombox.Add(self.dbox, 0, wx.EXPAND|wx.ALL, border=bspace)
        self.AddChannel()
        
        ## Create divider
        self.divider = Divider(self)
        self.divider.SetBackgroundColour(wx.Colour(200,200,200) )
        self.bottombox.Add(self.divider, 0, wx.EXPAND|wx.EXPAND)

        ## Create additional plotting controls
        self.cbox = wx.BoxSizer(wx.VERTICAL)
        nbox = wx.BoxSizer(wx.HORIZONTAL)
        self.make_label("Ymax(dB)", nbox, 44)
        self.fymax = form.float_field(self, nbox, value=self.ymax, \
                                      callback=self.SetYMax)
        self.cbox.Add(nbox, 0, wx.EXPAND|wx.TOP, border=bspace)
        nbox = wx.BoxSizer(wx.HORIZONTAL)
        self.make_label("Ymin(dB)", nbox, 49)
        self.fymin = form.float_field(self, nbox, value=self.ymin, \
                                      callback=self.SetYMin)
        self.cbox.Add(nbox, 0, wx.EXPAND|wx.TOP, border=bspace)
        nbox = wx.BoxSizer(wx.HORIZONTAL)
        self.make_label("Channel Period", nbox, 13)
        self.fchanuprate = form.int_field(self, nbox, value=self.chanUprate, \
                                      callback=self.SetChanUpRate)
        self.cbox.Add(nbox, 0, wx.EXPAND|wx.TOP, border=bspace)
        self.bottombox.Add(self.cbox, 0, wx.EXPAND|wx.ALL, border=bspace)
        self.AddParam('data',   self.SetData)
        self.AddParam('nsub',   self.SetNSubchannels)
        self.AddParam('chanrate',   self.SetChanUpRate)
        self.SetParam('chanrate', self.chanUprate)

        # Additional parameters for control
        #self.AddParam('updateplot', self.UpdatePlot)
        self.AddParam('updateplot', self.TinyUpdate)
        self.AddParam('cleardata', self.ClearData)
        self.Fit()

    def AddPlot(self):
        ## Make ChannelPlotter Component
        vbox = wx.BoxSizer(wx.VERTICAL)
        self.plot = ChannelPlot(self, vbox)
        self.topbox.Add(vbox, 1, wx.EXPAND)
        self.Fit()

    def AddChannel(self, evt=None):
        f = ChannelControl.FORMAT[len(self.chan)]
        self.chan.append(ChannelControl(self, self.dbox, format=f) )
        self.UpdatePlot()
        sz = self.GetClientSize()
        self.SetClientSize(wx.Size(sz[0] + 10, sz[1]+10) )
        self.Fit()

    def RemoveChannel(self, c):
        if c in self.chan:
            c.Remove()
            self.chan.remove(c)
        self.UpdatePlot()
        self.Fit()

    def SetNSubchannels(self, n):
        self.nsub = n
        self.plot.SetNSubchannels(n)

    def SetYMax(self, y=None):
        self.ymax = 10.0
        if y is None:
            self.ymax = self.fymax.get_value()
        else:
            self.ymax = y
            self.fymax.set_value(y)
        self.plot.SetYRange(ymax=self.ymax)
        self.UpdatePlot()

    def SetYMin(self, y=None):
        self.ymin = -60.0
        if y is None:
            self.ymin = self.fymin.get_value()
        else:
            self.ymin = y
            self.fymin.set_value(y)
        self.plot.SetYRange(ymin=self.ymin)
        self.UpdatePlot()

    def SetChanUpRate(self, n=None):
        self.chanUprate = 2
        if n is None:
            self.chanUprate = self.fchanuprate.get_value()
        else:
            self.chanUprate = n
            self.fchanuprate.set_value(n)
        p = self.GetParent()
        if hasattr(p, 'phy'):
            if hasattr(p.phy, 'rx'):
                if hasattr(p.phy.rx, 'set_chan_rate'):
                    p.phy.rx.set_chan_rate(self.chanUprate);

    def SetData(self, data, nrx=0, ntx=0, update=False):
        if len(data) != self.nsub:
            self.SetNSubchannels(len(data) )
        x = numpy.array(data)
        k = numpy.where(x<self.ymin)
        if (self.ymax<numpy.max(x) ):
            self.SetYMax(numpy.max(x)+10.0 )
        x[k] = self.ymin
        self.data[(nrx,ntx)] = x
        if update:
            self.TinyUpdate()
            #self.UpdatePlot()

    def ClearData(self):
        self.data = {}

    def TinyUpdate(self):
        nplots = len(self.plot.lines)
        onplots = numpy.array([1 for c in self.chan \
                    if (c.display and ((c.nrx, c.ntx) in self.data) )] )
        if int(numpy.sum(onplots)) != nplots:
            return self.UpdatePlot()
        pindex = 0
        x = numpy.array(self.plot.subcarriers)
        for c in self.chan:
            if (c.nrx, c.ntx) in self.data:
                data = self.data[(c.nrx, c.ntx)]
                if (len(data)==self.nsub) and c.display:
                    line = self.plot.lines[pindex][0]
                    line.set_ydata(data[x%self.nsub])
                    line.set_xdata(x)
        self.plot.Draw()

    def UpdatePlot(self):
        self.plot.hold(False)
        self.plot.draw([0],[0])
        x = numpy.array(self.plot.subcarriers)
        for c in self.chan:
            if (c.nrx, c.ntx) in self.data:
                data = self.data[(c.nrx, c.ntx)]
                if (len(data) == self.nsub) and c.display:
                    self.plot.hold(True)
                    self.plot.draw(x, data[x%self.nsub], c.format)
        self.plot.SetPlotProperties()

    def SetSID(self, PID, id):
        if PID<self.nplot: self.plot[PID].SetSID(id)

    def SetFormat(self, PID, f):
        if PID<self.nplot: self.plot[PID].SetFormat(f)

    def SetColor(self, PID, c):
        if PID<self.nplot: self.plot[PID].SetColor(c)

    def SetShape(self, PID, s):
        if PID<self.nplot: self.plot[PID].SetShape(s)

    def GetAboutLabel(self):
        return "Channel plotting GUI"

    def OnExit(self, evt):
        if 'channel' in dir(self.GetParent() ):
            self.GetParent().channel = None
        window.OnExit(self, evt)

class ChannelPlot(MPLBase):
    def __init__(self, win, sizer):
        MPLBase.__init__(self)
        self.win = win
        self.sizer = sizer
        self.lines = []
        self.subcarriers = range(-64/2, 64/2)
        self.yrange = [-60,10]

        # create fig, canvas, toolbar
        figsize, dpi = (5,4.5), 100
        self.make_plot(self.win, figsize=figsize, dpi=dpi)

        # create subplot with data
        self.make_subplot()
        self.toolbar.update()

        # compose components
        vbox = wx.BoxSizer(wx.VERTICAL)
        vbox.Add(self.canvas, 1, wx.LEFT|wx.TOP|wx.EXPAND)
        # Best to allow the toolbar to resize!
        vbox.Add(self.toolbar, 0, wx.EXPAND)
        self.sizer.Add(vbox, 1, wx.EXPAND)

    def make_subplot(self):
        self.axes = self.fig.add_subplot(111)
        self.axes.set_position([0.13,0.13,0.82,0.80])
        self.hold(False)
        x = numpy.array(self.subcarriers)
        self.plot(x, x*0.0, 'b')
        self.SetPlotProperties()

    def hold(self, *args):
        self.axes.hold(*args)
        if not self.axes.ishold():
            self.lines = []

    def plot(self, *args, **kwargs):
        self.lines.append(self.axes.plot(*args, **kwargs) )

    def draw(self, *args, **kwargs):
        self.plot(*args, **kwargs)

    def SetNSubchannels(self, n):
        self.subcarriers = numpy.array(range(-n/2, n/2) )
        self.SetPlotProperties()

    def SetYRange(self, ymin=None, ymax=None):
        if ymin is not None: self.yrange[0] = ymin
        if ymax is not None: self.yrange[1] = ymax
        self.SetPlotProperties()

    def SetPlotProperties(self):
        self.axes.grid(True)
        x, y = self.subcarriers, self.yrange
        self.axes.set_xlim(x[0], x[len(x)-1] )
        self.axes.set_ylim(y[0], y[1])
        self.axes.set_xlabel('Subcarrier Index')
        self.axes.set_ylabel('Channel Gain (dB)')
        self.Draw()

class ChannelControl(object):
    _COUNT = 0
    FORMAT = ['b', 'g', 'r', 'c', \
              'b:', 'g:', 'r:', 'c:', \
              'bv-', 'gv-', 'rv-', 'cv-']
    def __init__(self, win, sizer, nrx=0, ntx=0, format='b'):
        self.id = ChannelControl._COUNT
        ChannelControl._COUNT += 1
        self.win = win
        self.sizer = sizer
        
        ## Channel data parameters
        self.nrx, self.ntx = 0, 0
        self.format = format
        self.display = True

        self.box = wx.BoxSizer(wx.HORIZONTAL)
        self.win.make_label("H", self.box, 5)
        self.fnrx = form.int_field (self.win, self.box, value=self.nrx, \
                                    callback=self.SetNRx)
        self.fntx = form.int_field (self.win, self.box, value=self.ntx, \
                                    callback=self.SetNTx)
        self.box.AddSpacer((30,10))
        self.fformat = form.text_field(self.win, self.box, value=self.format, \
                                       callback=self.SetFormat)
        self.box.AddSpacer((20,10))
        self.fdisplay = form.checkbox_field(self.win, self.box, label="display?", \
                             value=self.display, callback=self.SetDisplay)
        self.box.AddSpacer((10,10))
        id = wx.NewId()
        self.button = wx.Button(self.win, id=id, label='x', style = wx.BU_EXACTFIT)
        wx.EVT_BUTTON(self.win, id, lambda evt: self.win.RemoveChannel(self) )
        self.box.Add(self.button, 0, wx.ALIGN_RIGHT|wx.LEFT, border=10)
        self.sizer.Add(self.box, 0)

    def Remove(self):
        self.sizer.Hide(self.box, recursive=True)
        self.sizer.Remove(self.box)

    def SetNRx(self, n=None):
        self.nrx = 0
        if n is None:
            self.nrx = self.fnrx.get_value()
        else:
            self.nrx = n
            self.fnrx.set_value(n)
        self.win.UpdatePlot()

    def SetNTx(self, n=None):
        self.ntx = 0
        if n is None:
            self.ntx = self.fntx.get_value()
        else:
            self.ntx = n
            self.fntx.set_value(n)
        self.win.UpdatePlot()

    def SetFormat(self, f=None):
        self.format = 'b'
        if f is None:
            self.format = self.fformat.get_value()
        else:
            self.format = f
            self.fformat.set_value(f)
        self.win.UpdatePlot()

    def SetDisplay(self, d=None):
        self.display = True
        if d is None:
            self.display = self.fdisplay.get_value()
        else:
            self.display = d
            self.fdisplay.set_value(d)
        self.win.UpdatePlot()
