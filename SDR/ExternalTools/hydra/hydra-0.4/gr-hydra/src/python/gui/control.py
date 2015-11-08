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
from hydra.PyHydra import Callable, mpif_impl
from window import window, Divider
from constellation import constellation
from channel import channel
import wx
import sys

class default_control_setup:
    d_options = {'verbose':0, \
                 'freq':2400e6, 'sample_rate':1.0e6, \
                 'upsample':1, 'tx_gain':4000.0, 'auto_tx_gain':False, \
                 'rx_gain':40.0}

    def get_options():
        dopt = default_control_setup()
        for s in default_control_setup.d_options:
            if s not in dir(dopt):
                setattr(dopt, s, default_control_setup.d_options[s] )
        return dopt
    
    def check_options(options):
        for s in default_control_setup.d_options:
            if s not in dir(options):
                setattr(options, s, default_control_setup.d_options[s] )

    get_options = Callable (get_options)
    check_options = Callable (check_options)

class control(window):
    def __init__(self, options, title="Hydra Control GUI", **kwargs):
        # check for default options before continuing with constructor
        default_control_setup.check_options(options)
        self.verbose = options.verbose
        self.options = options

        self.phy = None
        self.mpif = None
        self.constellation = None
        self.channel = None
        window.__init__(self, title=title, **kwargs)

    def GetAboutLabel(self):
        return "This GUI controls the operation of the physical layer."

    def make_menu(self):
        window.make_menu(self)
        menu = wx.Menu()
        ## Constellation Menu Item
        id = wx.NewId()
        item = menu.Append (id, 'C&onstellation', 'Constellation')
        self.Bind (wx.EVT_MENU, self.AddConstellation, item)
        ## Channel Menu Item
        id = wx.NewId()
        item = menu.Append(id, '&Channel', 'Channel')
        self.Bind(wx.EVT_MENU, self.AddChannel, item)
        ## Demo Panel - (optional)
        id = wx.NewId()
        item = menu.Append(id, '&Demo Panel', 'Demo Panel')
        self.Bind(wx.EVT_MENU, self.AddDemoPanel, item)
        self.mainmenu.Append (menu, "&Plot")

    def AddChannel(self, evt):
        pos = self.GetPositionTuple()
        pos = (pos[0]+800, pos[1]+0)
        self.channel = channel(parent=self, pos=pos)
        self.channel.Show(True)
        print "ADDING CHANNEL GUI ..."

    def AddConstellation(self, evt):
        pos = self.GetPositionTuple()
        pos = (pos[0]+50, pos[1]+400)
        self.constellation = constellation(parent=self, pos=pos)
        self.constellation.Show(True)

    def AddDemoPanel(self, evt):
        errmsg = "%s has no demo panel defined.\n"%(str(self.GetTitle()) )
        sys.stderr.write(errmsg)

    def make_layout(self):
        bspace = 10
        self.topbox.SetOrientation(wx.HORIZONTAL)
        self.SetPosition((50,50))

        #self.spframe = window(self, "My Splitter Window")
        #spwin = wx.SplitterWindow(self.spframe, -1, style=wx.SP_3D)
        #spbox = wx.BoxSizer(wx.HORIZONTAL)
        #f = form.static_text_field(self.spframe, spbox, value="helloworld")
        #win1 = wx.Window(None,-1)
        #win2 = wx.Window(self)
        #assert spwin.IsSplit()
        #assert spwin.SplitVertically(win1, win2)
        #self.spframe.topbox.DeleteWindows()
        #self.spframe.SetSizer(spbox,True)
        #self.spframe.Show(True)

        self.make_control()
        
        ## Create divider
        self.topdivider = Divider(self)
        self.topdivider.SetBackgroundColour(wx.Colour(200,200,200) )
        self.topbox.Add(self.topdivider, 0, wx.EXPAND|wx.GROW)
        
        self.make_debug()
        
        #self.button = form.button_with_callback(self, "MyButton", self.OnClick)
        #self.topbox.Add(self.button, 0, wx.ALIGN_LEFT|wx.ALL, border=bspace)

    def OnClick(self):
        self.SetFreq(self.fc/1.0e3)
        self.SetSampleRate(self.fs*2)
        self.SetUpsample(self.upsample_factor*2)
        self.SetTxGain(self.Gtx-2)
        self.SetRxGain(self.Grx+1)
        self.txinfo.set_value("transmit info %d"%(self.upsample_factor+1) )
        self.rxinfo.set_value("receive info %d"%(self.upsample_factor-1) )

    def make_control(self):
        bspace = 10
        vbox = wx.BoxSizer(wx.VERTICAL)
        
        """ Frequency """
        nbox = wx.BoxSizer(wx.HORIZONTAL)
        self.make_label("Carrier Frequency", nbox, 46)
        self.freq = form.float_field (self, nbox, \
                value=default_control_setup.d_options['freq'], callback=self.SetFreq)
        self.AddParam('freq',self.SetFreq)
        self.SetParam('freq', self.options.freq)
        vbox.AddSpacer((10,10) )
        vbox.Add(nbox, 0, wx.ALIGN_LEFT|wx.BOTTOM|wx.LEFT|wx.RIGHT, border=bspace)
        
        """ Sample Rate """
        nbox = wx.BoxSizer(wx.HORIZONTAL)
        self.make_label("Sample Rate", nbox, 80)
        self.sample_rate = form.float_field (self, nbox, \
                value=self.options.sample_rate, callback=self.SetSampleRate)
        self.AddParam('sample_rate', self.SetSampleRate)
        self.SetParam('sample_rate', self.options.sample_rate)
        vbox.Add(nbox, 0, wx.ALIGN_LEFT|wx.BOTTOM|wx.LEFT|wx.RIGHT, border=bspace)
        
        """ Upsampling Factor """
        nbox = wx.BoxSizer(wx.HORIZONTAL)
        self.make_label("Upsampling Factor", nbox, 43)
        self.upsample = form.int_field (self, nbox, \
                value=self.options.upsample, callback=self.SetUpsample)
        self.AddParam('upsampling_factor', self.SetUpsample)
        self.SetParam('upsampling_factor', self.options.upsample)
        vbox.Add(nbox, 0, wx.ALIGN_LEFT|wx.BOTTOM|wx.LEFT|wx.RIGHT, border=bspace)
        self.upsamplebox = nbox

        """ TxGain """
        box = wx.BoxSizer(wx.HORIZONTAL)
        self.txgain = form.slider_field(self, box, "Transmit Gain",\
                value=self.options.tx_gain, callback=self.SetTxGain,
                min=0, max=10000.0)
        box.AddSpacer((10,10) )
        self.autotxgain = form.checkbox_field(self, box, "auto", \
                value=self.options.auto_tx_gain, callback=self.SetAutoTxGain, \
                weight=0)
        box.GetItem(self.autotxgain.f).SetFlag(wx.ALIGN_LEFT|wx.ALIGN_BOTTOM)
        self.AddParam('auto_tx_gain', self.SetAutoTxGain)
        self.SetParam('auto_tx_gain', self.options.auto_tx_gain)
        self.AddParam('tx_gain', self.SetTxGain)
        self.SetParam('tx_gain', self.options.tx_gain)
        vbox.Add(box, 0, wx.ALIGN_LEFT|wx.BOTTOM|wx.LEFT|wx.RIGHT, border=bspace)
        self.txgainbox = box

        """ RxGain """
        box = wx.BoxSizer(wx.HORIZONTAL)
        self.rxgain = form.slider_field(self, box, "Receive Gain", \
                value=self.options.rx_gain, callback=self.SetRxGain,
                min=0, max=75)
        self.AddParam('rx_gain', self.SetRxGain)
        self.SetParam('rx_gain', self.options.rx_gain)
        vbox.Add(box, 0, wx.ALIGN_LEFT|wx.BOTTOM|wx.LEFT|wx.RIGHT, border=bspace)

        self.topbox.Add(vbox, 0, wx.GROW)
        self.cbox = vbox
        self.Fit()

    def make_debug(self):
        bspace = 20
        nblank = 2
        vbox = wx.BoxSizer(wx.VERTICAL)
        txbox = wx.BoxSizer(wx.VERTICAL)
        rxbox = wx.BoxSizer(wx.VERTICAL)
        self.txinfo = form.static_text_field(self, txbox, \
                "Tx Info\n"+":::::::::::::"*4, "\n"*nblank)
        self.txinfo.f.Wrap(-1)
        self.AddParam('txinfo', self.SetTxInfo)
        self.rxinfo = form.static_text_field(self, rxbox, \
                "Rx Info\n"+":::::::::::::"*4, "\n"*nblank)
        self.AddParam('rxinfo', self.SetRxInfo)
        txbox.SetItemMinSize(self.txinfo.f, (250,150) )
        rxbox.SetItemMinSize(self.rxinfo.f, (250,150) )
        vbox.Add(txbox, 0, wx.ALIGN_LEFT|wx.ALL, border=bspace)
        vbox.AddSpacer( (20,20) )
        vbox.Add(rxbox, 0, wx.ALIGN_LEFT|wx.ALL, border=bspace)
        self.topbox.Add(vbox, wx.GROW|wx.ALL, border=bspace)
        self.dbox = vbox

    def SetTxInfo(self, s):
        self.txinfo.set_value("\n"+s)
        self.Fit()

    def SetRxInfo(self, s):
        self.rxinfo.set_value("\n"+s)
        self.Fit()

    def SetFreq(self,f=None):
        self.fc = default_control_setup.d_options['freq']
        if f==None:
            self.fc = self.freq.get_value()
        else:
            self.fc = f
            self.freq.set_value(f)
        if self.phy:
            self.phy.set_freq(self.fc)

    def SetSampleRate(self,s=None):
        self.fs = default_control_setup.d_options['sample_rate']
        if s==None:
            self.fs = self.sample_rate.get_value()
        else:
            self.fs = s
            self.sample_rate.set_value(s)
        if self.phy:
            self.phy.set_sample_rate(self.fs)

    def SetUpsample(self, M=None):
        self.upsample_factor = default_control_setup.d_options['upsample']
        if M==None:
            self.upsample_factor = self.upsample.get_value()
        else:
            self.upsample_factor = M
            self.upsample.set_value(M)
        if self.phy:
            self.phy.set_upsample(self.upsample_factor)

    def SetTxGain(self,g):
        self.Gtx = g
        self.txgain.set_value(g)
        if hasattr(self.phy, 'tx'):
            if hasattr(self.phy.tx, 'set_gain'):
                self.phy.tx.set_gain(g)

    def SetAutoTxGain(self, v):
        self.autoGtx = v
        self.autotxgain.set_value(v)
        if hasattr(self.mpif, 'tx'):
            self.mpif.tx.set_auto_power(v)

    def SetRxGain(self,g):
        self.Grx = g
        self.rxgain.set_value(g)
        if self.phy:
            self.phy.set_rx_gain(g)

    def SetMPIF(self, m):
        self.mpif = m
        if 'phy' in dir(self.mpif):
            self.phy = self.mpif.phy

    def SetPHY(self, p):
        self.phy = p

    def OnExit(self, event):
        if self.mpif: self.mpif.shutdown()
        elif self.phy:  self.phy.shutdown()
        window.OnExit(self, event)

    def error (self, msg, level=0):
        if self.verbose >= level: sys.stderr.write("CONTROL ERROR: "+str(msg)+"\n")

    """ Add parser options to an OptionParser """
    def add_parser_options (parser):
        if not parser.has_option("-v"):
            parser.add_option ("-v", "--verbose", type="int", \
                    default=default_control_setup.d_options['verbose'], \
                    help="set verbose level of output [default=%default]")
        if not parser.has_option("-f"):
            parser.add_option ("-f", "--freq", type="eng_float", \
                    default=default_control_setup.d_options['freq'], \
                    help="set carrier frequency [default=%default]")
        if not parser.has_option("-s"):
            parser.add_option ("-s", "--sample-rate", type="eng_float", \
                    default=default_control_setup.d_options['sample_rate'], \
                    help="set usrp sample rate [default=%default]")
        if not parser.has_option("-u"):
            parser.add_option ("-u", "--upsample", type="int",
                    default=default_control_setup.d_options['upsample'], \
                    help="set the upsampling factor [default=%default]")
        if not parser.has_option("-g"):
            parser.add_option ("-g", "--tx-gain", type="eng_float", \
                    default=default_control_setup.d_options['tx_gain'], \
                    help="set software transmit gain in dB [default=%default]")
        if not parser.has_option("--auto-tx-gain"):
            parser.add_option("", "--auto-tx-gain", action="store_true", \
                    default=default_control_setup.d_options['auto_tx_gain'],
                    help="enable automatic transmit gain control from MAC/PHY interface [default=%default]")
        if not parser.has_option("-G"):
            parser.add_option ("-G", "--rx-gain", type="eng_float", \
                    default=default_control_setup.d_options['rx_gain'], \
                    help="set RF receive gain in dB [default=%default]")
        mpif_impl.add_parser_options (parser)

    add_parser_options = Callable (add_parser_options)
