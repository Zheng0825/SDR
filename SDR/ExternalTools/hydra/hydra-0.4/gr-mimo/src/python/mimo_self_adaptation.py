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
import sys
import wx
from wx.lib.filebrowsebutton import FileBrowseButton

from mimo_rate_history import MimoRateHistory

# set up backend for matplotlib
from hydra.PyHydra.gui.helper import *

#from matplotlib.axes import Subplot
import numpy

class MimoSelfAdaptation(window):
    FORMAT = ['b', 'g', 'r', 'c', \
              'b:', 'g:', 'r:', 'c:', \
              'bv-', 'gv-', 'rv-', 'cv-']
    def __init__(self, title="MIMO Self Adaptation", **kwargs):
        self.phyrx = None
        self.rate_history = None
        self.plot = None
        self.data = {}
        self.format = {'per':'b', 'tput':'r'}
        self.newdata = {'per':False, 'tput':False}
        window.__init__(self, title=title, **kwargs)

    def make_menu(self):
        window.make_menu(self)
        menu = wx.Menu()
        ## Menu Item for Adding New Channel
        id = wx.NewId()
        item = menu.Append (id, 'Add Rate &History', 'Add Rate History')
        self.Bind (wx.EVT_MENU, self.AddRateHistory, item)
        self.mainmenu.Append(menu, "&Options")

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

        bspace = 5 

        """ Window Size """
        self.set_window_size = lambda *args: None
        self.window_size = 0
        if hasattr(self.phyrx, 'sa_window_size'):
            window_size = self.phyrx.sa_window_size()
            self.window_size = None
            print "[MIMOSELFADAPTATION] window size from phyrx = ",  self.window_size
        if hasattr(self.phyrx, 'set_sa_window_size'):
            self.set_window_size = self.phyrx.set_sa_window_size
        box = wx.BoxSizer(wx.HORIZONTAL)
        self.make_label("Window Size", box, 42)
        self.fwindow_size = form.int_field(self, box, value=window_size, \
                                      callback=self.SetSAWindowSize)
        self.AddParam('window size', self.SetSAWindowSize)
        self.controlbox.Add(box, 0, wx.ALIGN_LEFT|wx.BOTTOM|wx.LEFT|wx.RIGHT, border=bspace)

        """ Update Interval """
        self.set_update_interval = lambda *args: None
        self.update_interval = 0
        if hasattr(self.phyrx, 'sa_update_interval'):
            self.update_interval = self.phyrx.sa_update_interval()
        if hasattr(self.phyrx, 'set_sa_update_interval'):
            self.set_update_interval = self.phyrx.set_sa_update_interval
        box = wx.BoxSizer(wx.HORIZONTAL)
        self.make_label("Update Interval", box, 27)
        self.fupdate_interval = form.int_field(self, box, value=self.update_interval, \
                                      callback=self.SetUpdateInterval)
        self.AddParam('update interval', self.SetUpdateInterval)
        self.controlbox.Add(box, 0, wx.ALIGN_LEFT|wx.BOTTOM|wx.LEFT|wx.RIGHT, border=bspace)

        """ PER and Tput Format """
        self.fformat = {}
        fcount = 0
        flabel = ["PER Format", "Tput Format"]
        foffset = [50, 47]
        for param in ['per', 'tput']:
            if param not in self.format:
                self.format[param] = MimoSelfAdaptation.FORMAT[fcount]
            box = wx.BoxSizer(wx.HORIZONTAL)
            self.make_label(flabel[fcount], box, foffset[fcount])
            self.fformat[param] = choicebox_field(self, box, \
                        callback=lambda *args: self.SetFormat(param, *args),  \
                        value=self.format[param],   \
                        choices=MimoSelfAdaptation.FORMAT)
            self.fformat[param].f.SetToolTipString("Select Plot Format")
            self.controlbox.Add(box, 0, wx.ALIGN_LEFT|wx.BOTTOM|wx.LEFT|wx.RIGHT, border=bspace)
            fcount += 1

        """ Database parameters """
        self.set_dbsize = lambda *args: None
        self.set_errdbsize = lambda *args: None
        self.dbsize, self.errdbsize = 200, 100
        if hasattr(self.phyrx, 'sa_dbsize'):
            self.dbsize = self.phyrx.sa_dbsize()
        if hasattr(self.phyrx, 'sa_errdbsize'):
            self.errdbsize = self.phyrx.sa_errdbsize()
        if hasattr(self.phyrx, 'set_sa_dbsize'):
            self.set_dbsize = self.phyrx.set_sa_dbsize
        if hasattr(self.phyrx, 'set_sa_errdbsize'):
            self.set_errdbsize = self.phyrx.set_sa_errdbsize
        self.controlbox.AddSpacer( (10,10) )
        hbox = wx.BoxSizer(wx.HORIZONTAL)
        self.make_label("Database Size", hbox, 27)
        self.fdbsize = form.int_field(self, hbox, value=self.dbsize, \
                                      callback=self.SetDbSize)
        self.controlbox.Add(hbox, 0, wx.ALIGN_LEFT|wx.LEFT|wx.RIGHT, border=bspace)
        hbox = wx.BoxSizer(wx.HORIZONTAL)
        self.make_label("Error Size", hbox, 57)
        self.ferrdbsize = form.int_field(self, hbox, value=self.errdbsize, \
                                      callback=self.SetErrDbSize)
        self.controlbox.Add(hbox, 0, wx.ALIGN_LEFT|wx.BOTTOM|wx.LEFT|wx.RIGHT, border=bspace)
        self.AddParam('dbsize', self.SetDbSize)
        self.AddParam('errdbsize', self.SetErrDbSize)
        self.controlbox.AddSpacer( (10,10) )

        """ Reset, Load, and Save Database """
        self.load_database = lambda *args: None
        self.save_database = lambda *args: None
        if hasattr(self.phyrx, 'sa_load_database'):
            self.load_database = self.phyrx.sa_load_database
        if hasattr(self.phyrx, 'sa_save_database'):
            self.save_database = self.phyrx.sa_save_database
        id_reset, id_load, id_save = wx.NewId(), wx.NewId(), wx.NewId()
        self.reset_button = wx.Button(self, id_reset, "Reset")
        self.load_button  = wx.Button(self, id_load, "Load")
        self.save_button  = wx.Button(self, id_save, "Save")
        self.reset_button.SetToolTipString("Reset database from Database and Error Size parameters.")
        self.load_button.SetToolTipString( "Load database from a file.")
        self.save_button.SetToolTipString( "Save database to a file.")
        wx.EVT_BUTTON(self, id_reset, self.ResetDatabase)
        wx.EVT_BUTTON(self, id_load,  self.LoadDatabase)
        wx.EVT_BUTTON(self, id_save,  self.SaveDatabase)
        self.controlbox.Add(self.reset_button, 0, wx.ALIGN_CENTER|wx.LEFT|wx.RIGHT, border=bspace)
        self.controlbox.Add(self.load_button,  0, wx.ALIGN_CENTER|wx.LEFT|wx.RIGHT, border=bspace)
        self.controlbox.Add(self.save_button,  0, wx.ALIGN_CENTER|wx.BOTTOM|wx.LEFT|wx.RIGHT, border=bspace)

        self.AddParam('per', self.SetPER)
        self.AddParam('tput', self.SetThroughput)
        self.AddParam('addper', self.AddPER)
        self.AddParam('addtput', self.AddThroughput)
        self.AddParam('ratehist', self.SetRateHistogram)
        self.topbox.Add(self.controlbox, 0, wx.ALL, border=bspace)
        self.Fit()
        self.SetClientSize(self.topbox.GetSize())
        self.SetParam('window size', window_size)

    def SetFormat(self, key, value):
        if value is None:
            if key in self.fformat:
                self.format[key] = self.fformat[key].get_value()
        else:
            self.format[key] = value
            if key in self.fformat:
                self.fformat[key].set_value(value)
        self.RenewPlot()

    def AddRateHistory(self, evt):
        pos = self.GetPositionTuple()
        pos = (pos[0]+800, pos[1]+0)
        self.rate_history = MimoRateHistory(parent=self, pos=pos)
        self.rate_history.Show(True)
        print "ADDING RATE HISTORY GUI ..."

    def AddPlot(self):
        ## Make SelfAdaptationPlotter Component
        vbox = wx.BoxSizer(wx.VERTICAL)
        self.plot = SelfAdaptationPlot(self, vbox)
        self.topbox.Add(vbox, 1, wx.GROW)
        self.Fit()

    def SetSAWindowSize(self, n=None):
        c = self.window_size
        if n is None:
            self.window_size = self.fwindow_size.get_value()
        else:
            self.window_size = n
            self.fwindow_size.set_value(n)
        if c != self.window_size:
            self.set_window_size(self.window_size)
            if self.plot:
                self.plot.SetSAWindowSize(self.window_size)
                # update plot
                format1, format2 = (), ()
                if 'per' in self.data:  format1 = (self.format['per'], )
                if 'tput' in self.data: format2 = (self.format['tput'], )
                self.plot.plot1([0],[0], *format1)
                self.plot.plot2([0],[0], *format2)
                self.plot.SetPlotProperties()

    def SetUpdateInterval(self, n=None):
        p = self.update_interval
        if n is None:
            self.update_interval = self.fupdate_interval.get_value()
        else:
            self.update_interval = n
            self.fupdate_interval.set_value(n)
        if p != self.update_interval:
            self.set_update_interval(self.update_interval)

    def SetPER(self, data, ndata):
        update=True
        if 'per' not in self.data: update=False
        self.newdata['per'] = True
        assert len(data) == ndata, "[MIMO_SELF_ADAPTATION]: PER data does not match reported size!"
        self.data['per'] = numpy.array(data)
        self.RenewPlot(update=update)

    def SetThroughput(self, data, ndata):
        update=True
        if 'tput' not in self.data: update=False
        self.newdata['tput'] = True
        assert len(data) == ndata, "[MIMO_SELF_ADAPTATION]: Throughput data does not match reported size!"
        self.data['tput'] = numpy.array(data)
        self.RenewPlot(update=update)

    def AddPER(self, data, ndata):
        self.newdata['per'] = True
        if 'per' in self.data:
            x = self.data['per'].tolist()
        else:
            x = [0 for a in range(ndata)]
        assert len(x) == ndata, "[MIMO_SELF_ADAPTATION]: PER data does not match reported size!"
        if len(x)>0: x.pop(0)
        x.append(data)
        if 'per' in self.data:
            self.data['per'] = numpy.array(x)
            self.UpdatePlot()
        else:
            self.data['per'] = numpy.array(x)
            self.RenewPlot()

    def AddThroughput(self, data, ndata):
        self.newdata['tput'] = True
        if 'tput' in self.data:
            x = self.data['tput'].tolist()
        else:
            x = [0 for a in range(ndata)]
        assert len(x) == ndata, "[MIMO_SELF_ADAPTATION]: Throughput data does not match reported size!"
        if len(x)>0: x.pop(0)
        x.append(data)
        if 'tput' in self.data:
            self.data['tput'] = numpy.array(x)
            self.UpdatePlot()
        else:
            self.data['tput'] = numpy.array(x)
            self.RenewPlot()

    def SetRateHistogram(self, *args):
        if self.rate_history:
            self.rate_history.SetParam('data', *args)

    def SetDbSize(self, n=None):
        dbsize = self.dbsize
        errdbsize = self.errdbsize
        if n is None:
            self.dbsize = int(self.fdbsize.get_value())
        else:
            self.dbsize = int(n)
            self.fdbsize.set_value(self.dbsize)
        if (self.dbsize>self.errdbsize) and (self.dbsize!=dbsize):
            self.set_dbsize(self.dbsize)

    def SetErrDbSize(self, n=None):
        dbsize = self.dbsize
        errdbsize = self.errdbsize
        if n is None:
            self.errdbsize = int(self.ferrdbsize.get_value())
        else:
            self.errdbsize = int(n)
            self.ferrdbsize.set_value(self.errdbsize)
        if (self.dbsize>self.errdbsize) and (self.errdbsize!=errdbsize):
            self.set_errdbsize(self.errdbsize)

    def ResetDatabase(self, event=None):
        if (self.dbsize>self.errdbsize):
            self.set_dbsize(self.dbsize)
            self.set_errdbsize(self.errdbsize)

    def LoadDatabase(self, event):
        dbgui = SelfAdaptationLoadDatabase(parent=self)
        dbgui.Show(True)

    def FinishLoadDatabase(self, filename):
        self.load_database(filename)
        if hasattr(self.phyrx, 'sa_dbsize'):
            self.dbsize = self.phyrx.sa_dbsize()
        if hasattr(self.phyrx, 'sa_errdbsize'):
            self.errdbsize = self.phyrx.sa_errdbsize()
        self.set_dbsize(self.dbsize)
        self.set_errdbsize(self.errdbsize)

    def SaveDatabase(self, event):
        dbgui = SelfAdaptationSaveDatabase(parent=self)
        dbgui.Show(True)

    def FinishSaveDatabase(self, filename):
        self.save_database(filename)

    def UpdatePlot(self):
        isready = True
        assert ('per' in self.newdata) and ('tput' in self.newdata), \
                "[MIMO_SELF_ADAPTATION] Could not find proper keys in data dictionary!"
        if (self.newdata['per'] != self.newdata['tput']): isready = False
        if not isready: return
        L1 = self.plot.lines[0][0]
        L2 = self.plot.lines[1][0]
        xdata1 = numpy.array(range(len(self.data['per'])) )
        xdata2 = numpy.array(range(len(self.data['tput'])) )
        L1.set_ydata(self.data['per'])
        L1.set_xdata(xdata1)
        L2.set_ydata(self.data['tput'])
        L2.set_xdata(xdata2)
        self.plot.Draw()

    def RenewPlot(self, update=False):
        isready = True
        assert ('per' in self.newdata) and ('tput' in self.newdata), \
                "[MIMO_SELF_ADAPTATION] Could not find proper keys in data dictionary!"
        if (self.newdata['per'] != self.newdata['tput']): isready = False
        if not isready: return
        # renew or update plot
        for p in self.data:
            line_idx=0
            plot_method = self.plot.plot1
            if p=='tput':
                line_idx=1
                plot_method = self.plot.plot2
            x = numpy.array(range(len(self.data[p])))
            format = 'b'
            if p in self.format: format = self.format[p]
            if update:
                L = self.plot.lines[line_idx][0]
                L.set_xdata(x)
                L.set_ydata(self.data[p])
            else:
                #plot_method([0],[0])
                plot_method(x, self.data[p], format)
        if update: self.plot.Draw()
        else: self.plot.SetPlotProperties()

    def GetAboutLabel(self):
        return "MIMO Self Adaptation GUI"

    def OnExit(self, evt):
        if 'self_adapt' in dir(self.GetParent() ):
            self.GetParent().self_adapt = None
        window.OnExit(self, evt)

class SelfAdaptationPlot(MPLBase):
    def __init__(self, win, sizer):
        MPLBase.__init__(self)
        self.win = win
        self.sizer = sizer
        self.lines = []
        self.pktidx = range(100)
        self.yrange = [0,1]

        # create fig, canvas, toolbar
        figsize, dpi = (5,4.5), 100
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
        self.axes1 = self.fig.add_subplot(211)
        self.axes2 = self.fig.add_subplot(212)
        self.axes1.set_position([0.13,0.60,0.82,0.38])
        self.axes2.set_position([0.13,0.10,0.82,0.38])
        self.hold(False)
        x = numpy.array(self.pktidx)
        self.plot1(x,x*0.0, 'b')
        self.plot2(x,x*0.0, 'b')
        self.SetPlotProperties()

    def hold(self, *args):
        self.axes1.hold(*args)
        self.axes2.hold(*args)
        if not self.axes1.ishold():
            self.lines = []

    def plot(self, *args, **kwargs):
        self.lines.append(self.axes.plot(*args, **kwargs) )

    def draw(self, *args, **kwargs):
        self.plot(*args, **kwargs)

    def plot1(self, *args, **kwargs):
        newplot = self.axes1.plot(*args, **kwargs)
        if len(self.lines)>0: self.lines[0] = newplot
        else: self.lines.append(newplot)

    def plot2(self, *args, **kwargs):
        self.fig.add_subplot(212)
        newplot = self.axes2.plot(*args, **kwargs)
        if len(self.lines)>1: self.lines[1] = newplot
        else: self.lines.append(newplot)

    def SetSAWindowSize(self, w):
        sys.stderr.write("[SELFADAPTATIONPLOT] SetSAWindowSize(%s) called\n"%(str(w)))
        self.pktidx = numpy.array(range(w))
        self.SetPlotProperties()

    def SetPlotProperties(self):
        x, y = self.pktidx, [0.0,1.0]
        self.axes1.grid(True)
        self.axes1.set_xlim(x[0], x[len(x)-1] )
        self.axes1.set_ylim(y[0]-0.05, y[1]+0.05)
        self.axes1.set_xlabel('Relative RX Packet Index')
        self.axes1.set_ylabel('Packet Error Rate')
        
        x, y = self.pktidx, [0,135.0]
        self.axes2.grid(True)
        self.axes2.set_xlim(x[0], x[len(x)-1] )
        self.axes2.set_ylim(y[0]-0.05, y[1]+0.5)
        self.axes2.set_xlabel('Relative RX Packet Index')
        self.axes2.set_ylabel('Throughput (Mbps)')
        self.Draw()

##  @class SelfAdaptationLoadDatabase
#   @brief GUI for loading database from a file.
class SelfAdaptationLoadDatabase(window):
    def __init__(self, title="Load Self Adaptation Database", **kwargs):
        window.__init__(self, title=title, **kwargs)
        self.Centre()

    def make_menu(self):
        self.mainmenu = wx.MenuBar()
        self.SetMenuBar (self.mainmenu)

        self.Bind (wx.EVT_CLOSE, self.OnExit)

    def make_layout(self):
        box = wx.BoxSizer(wx.VERTICAL)
        # add file browser
        self.filebrowser = FileBrowseButton(self, labelText="", \
                              dialogTitle="Load Database File")
        box.Add(self.filebrowser, 0, wx.ALIGN_LEFT|wx.EXPAND|wx.ALL, border=10)

        # add ok and cancel buttons
        hbox = wx.BoxSizer(wx.HORIZONTAL)
        id_ok, id_cancel = wx.NewId(), wx.NewId()
        self.okbutton = wx.Button(self, wx.ID_OK)
        self.cancelbutton = wx.Button(self, wx.ID_CANCEL)
        self.okbutton.SetId(id_ok)
        self.cancelbutton.SetId(id_cancel)
        self.Bind(wx.EVT_BUTTON, self.OnDone, id=id_ok)
        self.Bind(wx.EVT_BUTTON, self.OnCancel, id=id_cancel)
        hbox.Add(self.okbutton, wx.ALIGN_LEFT|wx.RIGHT, border=5)
        hbox.Add(self.cancelbutton, border=0)
        # add to vertical box
        box.AddSpacer((400, 5) )
        box.Add(hbox, 0, wx.ALIGN_RIGHT|wx.ALL, border=10)

        # add to topbox
        self.topbox.Add(box, 0, wx.ALL, border=10)

    def OnDone(self, event=None):
        ok, f = True, None
        filename = self.filebrowser.GetValue()
        if filename is None: filename = ""
        else: filename = str(filename)
        try:
            f = file(filename, 'r')
        except IOError:
            ok = False
        if f: f.close()
        if ok and hasattr(self.GetParent(), 'FinishLoadDatabase'):
            self.GetParent().FinishLoadDatabase(filename)
            self.OnExit(event)
        else:
            dlg = wx.MessageDialog(self, "File could not be found", \
                                   "Load Error", wx.OK|wx.ICON_INFORMATION)
            dlg.ShowModal()
            dlg.Destroy()

    def OnCancel(self, event=None):
        self.OnExit(event)

##  @class SelfAdaptationSaveDatabase
#   @brief GUI for saving database to a file.
class SelfAdaptationSaveDatabase(window):
    def __init__(self, title="Save Self Adaptation Database", **kwargs):
        window.__init__(self, title=title, **kwargs)
        self.Centre()

    def make_menu(self):
        self.mainmenu = wx.MenuBar()
        self.SetMenuBar (self.mainmenu)

        self.Bind (wx.EVT_CLOSE, self.OnExit)

    def make_layout(self):
        box = wx.BoxSizer(wx.VERTICAL)
        # add file browser
        self.filebrowser = FileBrowseButton(self, labelText="", \
                              dialogTitle="Save Database File")
        box.Add(self.filebrowser, 0, wx.ALIGN_LEFT|wx.EXPAND|wx.ALL, border=10)

        # add ok and cancel buttons
        hbox = wx.BoxSizer(wx.HORIZONTAL)
        id_ok, id_cancel = wx.NewId(), wx.NewId()
        self.okbutton = wx.Button(self, wx.ID_OK)
        self.cancelbutton = wx.Button(self, wx.ID_CANCEL)
        self.okbutton.SetId(id_ok)
        self.cancelbutton.SetId(id_cancel)
        self.Bind(wx.EVT_BUTTON, self.OnDone, id=id_ok)
        self.Bind(wx.EVT_BUTTON, self.OnCancel, id=id_cancel)
        hbox.Add(self.okbutton, wx.ALIGN_LEFT|wx.RIGHT, border=5)
        hbox.Add(self.cancelbutton, border=0)
        # add to vertical box
        box.AddSpacer((400, 5) )
        box.Add(hbox, 0, wx.ALIGN_RIGHT|wx.ALL, border=10)

        # add to topbox
        self.topbox.Add(box, 0, wx.ALL, border=10)

    def OnDone(self, event=None):
        filename = self.filebrowser.GetValue()
        if filename is None: filename = ""
        else: filename = str(filename)
        if (len(filename)>0) and hasattr(self.GetParent(), 'FinishSaveDatabase'):
            self.GetParent().FinishSaveDatabase(filename)
            self.OnExit(event)
        else:
            dlg = wx.MessageDialog(self, "Invalid file name.", \
                                   "Save Error", wx.OK|wx.ICON_INFORMATION)
            dlg.ShowModal()
            dlg.Destroy()

    def OnCancel(self, event=None):
        self.OnExit(event)
