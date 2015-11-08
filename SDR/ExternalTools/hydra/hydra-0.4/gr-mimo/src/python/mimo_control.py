#!/usr/bin/env python
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
from gnuradio import gr
from hydra import PyHydra, mimo
from hydra.PyHydra import Callable
import wx

from mimo_mpif import MimoMPIF
from mimo_channel import MimoChannel
from mimo_csi import MimoCSI
from mimo_adapt import MimoAdapt
from mimo_self_adaptation import MimoSelfAdaptation

class default_mimo_control_setup:
    d_options = {'verbose':0, \
                 'freq':2400e6, 'sample_rate':1.0e6, \
                 'upsample':1, 'tx_gain':60.0, 'auto_tx_gain':False, \
                 'rx_gain':0.0, 'enable_beamforming':False}

    def get_options():
        dopt = default_mimo_control_setup()
        for s in default_mimo_control_setup.d_options:
            if s not in dir(dopt):
                setattr(dopt, s, default_mimo_control_setup.d_options[s] )
        return dopt
    
    def check_options(options):
        for s in default_mimo_control_setup.d_options:
            if s not in dir(options):
                setattr(options, s, default_mimo_control_setup.d_options[s] )
        PyHydra.default_control_setup.check_options(options)

    get_options = Callable (get_options)
    check_options = Callable (check_options)

class MimoControl(PyHydra.control):
    MCS_CHOICES = [str(k) for k in range(16)] + ["Auto"]
    ENABLE_CHOICES = ["off", "on"]
    BROADCAST_ADDR = '\xFF'*mimo.mac_monitor.MAC_ADDR_SIZE
    NULL_ADDR = '\x00'*mimo.mac_monitor.MAC_ADDR_SIZE
    def __init__(self, options, title="Hydra MIMO Control GUI", **kwargs):
        # check for default options before continuing with constructor
        default_mimo_control_setup.check_options(options)
        self.verbose = options.verbose
        self.options = options

        # Update MCS_CHOICES if needed
        if hasattr(self.options, 'nchannels'):
            nchan = self.options.nchannels
            MimoControl.MCS_CHOICES = [str(k) for k in range(8*nchan)] + ["Auto"]

        self.csi = None
        self.adapt = None
        self.self_adapt = None
        self.monitor_addr = MimoControl.BROADCAST_ADDR
        self.local_addr = MimoControl.NULL_ADDR
        PyHydra.control.__init__(self, options, title=title, **kwargs)
    
    def GetAboutLabel(self):
        return "This GUI controls the operation of the MIMO physical layer."

    def __setattr__(self, key, val):
        if   key=='constellation':
            m = False
            if val is not None: m = True
            if hasattr(self, 'phy'):
                if hasattr(self.phy, 'rx'):
                    if hasattr(self.phy.rx, 'set_plot_constellation'):
                        self.phy.rx.set_plot_constellation(m)
        elif key=='channel':
            m = False
            if val is not None: m = True
            if hasattr(self, 'phy'):
                if hasattr(self.phy, 'rx'):
                    if hasattr(self.phy.rx, 'set_plot_channel'):
                        self.phy.rx.set_plot_channel(m)
        elif key=='csi':
            m = False
            if val is not None: m = True
            if hasattr(self, 'phy'):
                if hasattr(self.phy, 'rx'):
                    if hasattr(self.phy.rx, 'set_plot_limited_feedback'):
                        self.phy.rx.set_plot_limited_feedback(m)
        elif key=='self_adapt':
            m = False
            if val is not None: m = True
            if hasattr(self, 'phy'):
                if hasattr(self.phy, 'rx'):
                    if hasattr(self.phy.rx, 'set_plot_self_adaptation'):
                        self.phy.rx.set_plot_self_adaptation(m)
        PyHydra.control.__setattr__(self, key, val)

    def to_address(self,s):
        slist = s.split(":")
        addr = ""
        for k in range(len(slist) ):
            addr += chr(int(slist[k], 16) )
        if len(addr) == mimo.mac_monitor.MAC_ADDR_SIZE:
            return addr
        else:
            return MimoControl.BROADCAST_ADDR

    def cmp_mac_addr(self, s):
        if self.monitor_mac_addr():
            print "mac addr =", self.to_address_string(s[0:mimo.mac_monitor.MAC_ADDR_SIZE])
            print "monitor addr =", self.to_address_string(self.monitor_addr)
            if self.monitor_addr == MimoControl.BROADCAST_ADDR:
                return True
            else:
                addr = s[0:mimo.mac_monitor.MAC_ADDR_SIZE]
                return (addr == self.monitor_addr)

    def monitor_mac_addr(self):
        if hasattr(self.phy, 'rx'):
            if hasattr(self.phy.rx, 'monitor_mac_addr'):
                return self.phy.rx.monitor_mac_addr()

    def to_address_string(self, addr):
        paddr = ""
        for k in range(len(addr) ):
            paddr += "%02X"%ord(addr[k])
            if (k+1)<len(addr): paddr +=":"
        return paddr

    def make_menu(self):
        PyHydra.control.make_menu(self)
        k = self.mainmenu.FindMenu("Plot")
        menu = self.mainmenu.GetMenu(k)
        ## Limited Feedback CSI
        id = wx.NewId()
        N = menu.GetMenuItemCount()
        item = menu.Insert(N-1, id, '&Limited Feedback CSI', 'Limited Feedback CSI')
        self.Bind(wx.EVT_MENU, self.AddCSI, item)
        if mimo.SELF_ADAPTATION_ENABLED:
            ## Self Adaptation
            id = wx.NewId()
            N = menu.GetMenuItemCount()
            item = menu.Insert(N-1, id, 'Self &Adaptation', 'Self Adaptation')
            self.Bind(wx.EVT_MENU, self.AddSelfAdaptation, item)
        else:
            ## Options Menu -> Adaptation
            id = wx.NewId()
            menu = wx.Menu()
            item = menu.Append(id, '&MIMO Adaptation')
            self.Bind(wx.EVT_MENU, self.AddAdapt, item)
            self.mainmenu.Append(menu, '&Options')

    def AddLocalMacAddress(self):
        bspace = 10
        """ MAC Local Address """
        nbox = wx.BoxSizer(wx.HORIZONTAL)
        f = self.make_label("Local MAC Address: ", nbox)
        nbox.SetItemMinSize(f.f, (150,20) )
        self.flocal_addr = self.make_label("--:--:--:--:--:--", nbox)
        nbox.SetItemMinSize(self.flocal_addr.f, (150,20) )
        self.AddParam("local mac address", self.SetLocalMacAddr)
        self.cbox.InsertSpacer(0,(10,20))
        self.cbox.Insert(1, nbox, 0, wx.ALIGN_LEFT|wx.BOTTOM|wx.LEFT, border=bspace)

        ## Create divider
        divider = PyHydra.Divider(self)
        divider.SetBackgroundColour(wx.Colour(200,200,200) )
        self.cbox.InsertSpacer(2,(10,20))
        self.cbox.Insert(2, divider, 0, wx.EXPAND|wx.GROW)

    def make_control(self):
        bspace = 10
        PyHydra.control.make_control(self)

        self.AddLocalMacAddress()
        
        """ Create MCS choicebox_field """
        box = wx.BoxSizer(wx.HORIZONTAL)
        self.make_label("MCS", box, 35)
        self.mcsfield = PyHydra.choicebox_field(self, box, \
                        value="Auto", callback = self.SetMCS, \
                        choices = MimoControl.MCS_CHOICES)
        self.mcsfield.f.SetToolTipString("Select Modulation and Coding Scheme")
        self.AddParam('rate', self.SetMCS)
        self.AddParam('mcs', self.SetMCS)
        if self.options.rate<0: self.SetParam('mcs', "Auto")
        else: self.SetParam('mcs', self.options.rate)
        self.cbox.Add (box, 0, wx.ALIGN_LEFT|wx.ALL, border = bspace)

        ## Create divider
        divider = PyHydra.Divider(self)
        divider.SetBackgroundColour(wx.Colour(200,200,200) )
        self.cbox.AddSpacer((10,10))
        self.cbox.Add(divider, 0, wx.EXPAND|wx.GROW)
        self.cbox.AddSpacer((10,10))
        
        """ Packet Detector Threshold """
        box = wx.BoxSizer(wx.VERTICAL)
        self.pd_threshold = form.slider_field(self, box,  \
                "Packet Detector Threshold (%)", value=self.options.pd_threshold,
                callback=self.SetPacketDetectorThreshold, min=20, max=100)
        self.AddParam('pd_threshold', self.SetPacketDetectorThreshold)
        self.SetParam('pd_threshold', self.options.pd_threshold)
        self.cbox.Add(box, 0, wx.ALIGN_LEFT|wx.ALL, border=bspace)

        """ Enable Beamforming """
        box = wx.BoxSizer(wx.HORIZONTAL)
        self.make_label("Enable Beamforming: ", box, 20)
        self.enable_beamforming = form.radiobox_field(self, box, label="", \
                value = MimoControl.ENABLE_CHOICES[self.options.enable_beamforming], \
                callback=self.SetEnableBeamforming,     \
                choices = MimoControl.ENABLE_CHOICES, major_dimension=0)
        #box.Hide(1, recursive=True), box.Remove(1) # Remove default form label
        self.enable_beamforming.f.SetLabel("")
        self.AddParam('enable_beamforming', self.SetEnableBeamforming)
        self.SetParam('enable_beamforming', self.options.enable_beamforming)
        self.cbox.Add(box, 0, wx.ALIGN_LEFT|wx.ALL, border=bspace)

        """ MAC Monitor Address """
        vbox = wx.BoxSizer(wx.VERTICAL)
        self.monitor = MimoControl.BROADCAST_ADDR
        f = form.static_text_field(self, vbox, value="Monitoring MAC Address: ")
        vbox.AddSpacer((10,10) )
        box = wx.BoxSizer(wx.HORIZONTAL)
        self.fmonitor = form.text_field(self, box, \
                value="FF:"*(mimo.mac_monitor.MAC_ADDR_SIZE-1)+"FF", \
                callback=self.SetMonitor, weight=1)
        self.AddParam('mac_monitor', self.SetMonitor)
        box.AddSpacer((10,10) )
        self.enable_monitor = form.checkbox_field(self, box, "monitor?", \
                value=False, callback=self.SetEnableMonitor, \
                weight=0)
        box.AddSpacer((100,10) )
        vbox.Add(box, 0, wx.EXPAND)
        self.cbox.Add(vbox, 1, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL, border=bspace)
        self.Fit()

    def SetLocalMacAddr(self, s):
        if s == MimoControl.NULL_ADDR: return
        self.flocal_addr.set_value(str(self.to_address_string(s))+"\n")
        self.Fit()

    def AddChannel(self, evt):
        pos = self.GetPositionTuple()
        pos = (pos[0]+800, pos[1]+0)
        self.channel = MimoChannel(parent=self, pos=pos)
        self.channel.Show(True)
        self.channel.SetChannelType(1)
        print "ADDING MIMO CHANNEL GUI ..."

    def AddCSI(self, evt):
        pos = self.GetPositionTuple()
        pos = (pos[0]+500, pos[1]+25)
        self.csi = MimoCSI(parent=self, pos=pos)
        self.csi.Show(True)
        self.csi.SetSubchannel(4)
        print "ADDING MIMO LIMITED FEEDBACK CSI GUI ..."

    def AddSelfAdaptation(self, evt):
        if mimo.SELF_ADAPTATION_ENABLED:
            pos = self.GetPositionTuple()
            pos = (pos[0]+500, pos[1]+25)
            self.self_adapt = MimoSelfAdaptation(parent=self, pos=pos)
            self.self_adapt.Show(True)
            print "ADDING MIMO SELF ADAPTATION GUI ..."
        else:
            print "CANNOT ADD MIMO SELF ADATATION GUI !!!"

    def AddAdapt(self, evt):
        pos = self.GetPositionTuple()
        pos = (pos[0]+100, pos[1]+100)
        self.adapt = MimoAdapt(parent=self, pos=pos)
        self.adapt.Show(True)
        print "ADDING MIMO ADAPTATION CONTROL"

    def SetMCS(self, r=None):
        if r==None:
            self.mcs = self.mcsfield.get_value()
        else:
            self.mcs = str(r)
            if self.mcs in MimoControl.MCS_CHOICES: self.mcsfield.set_value(self.mcs) 
            if self.mcs.lower() == "auto": self.mcsfield.set_value("Auto")
        print "SetMCS called ... rate =", self.mcs
        if hasattr(self.mpif,'tx'):
            if self.mcs.lower().strip() == "auto":
                self.mpif.tx.set_auto_rate(True)
            else:
                try:    n = int(self.mcs)
                except: n = 0
                self.mpif.tx.set_auto_rate(False)
                self.mpif.tx.set_rate(n)

    def SetPacketDetectorThreshold(self, t):
        if t > 1.0: t = t/100.0
        t = max(min(t, 1.00), 0.0)
        self.thresh = t
        if hasattr(self.phy, 'set_thresh'):
            self.phy.set_thresh(t)

    def SetEnableBeamforming(self, x):
        s, m = str(x).lower(), None
        if s in MimoControl.ENABLE_CHOICES:
            m = MimoControl.ENABLE_CHOICES.index(s)
        elif isinstance(x,int):
            m = (x>0)
            self.enable_beamforming.set_value(MimoControl.ENABLE_CHOICES[m])
        if (m is not None):
            if hasattr(self.phy, 'tx'):
                if hasattr(self.phy.tx,'set_enable_beamforming'):
                    print "enable beamforming =", m
                    self.phy.tx.set_enable_beamforming(m)
            if hasattr(self.phy, 'rx'):
                if hasattr(self.phy.rx, 'set_enable_csi_feedback'):
                    print "enable csi feedback =", m
                    self.phy.rx.set_enable_csi_feedback(m)

    def SetMonitor(self, s=None):
        addr = MimoControl.BROADCAST_ADDR
        if s==None:
            addr = self.to_address(self.fmonitor.get_value())
            print "mac addr to monitor =", self.to_address_string(addr)
        else:
            self.monitor = self.to_address(s)
            self.fmonitor.set_value(s)
        self.monitor_addr = addr
        self.fmonitor.f.Fit()

    def SetEnableMonitor(self, v):
        if hasattr(self.phy, 'rx'):
            if hasattr(self.phy.rx, 'set_monitor_mac_addr'):
                self.phy.rx.set_monitor_mac_addr(v)


    def AddDemoPanel(self, evt):
        # Launch Constellation Plot
        self.AddConstellation(evt)
        if self.constellation:
            self.constellation.AddPlot()
        # Launch Channel Plot
        self.AddChannel(evt)
        chanSetup = [ {'nrx':0, 'ntx':1}, {'nrx':1, 'ntx':0}, {'nrx':1, 'ntx':1}]
        if self.channel:
            for k in range(len(chanSetup)):
                self.channel.AddChannel()
                self.channel.chan[k+1].SetNRx(chanSetup[k]['nrx'])
                self.channel.chan[k+1].SetNTx(chanSetup[k]['ntx'])
        #self.AddAdapt(evt)
        if self.adapt:
            self.adapt.SetParam('snr penalty', 7)
            self.adapt.SetParam('condition penalty', 0.5)

    """ Add parser options to an OptionParser """
    def add_parser_options (parser):
        if not parser.has_option("-v"):
            parser.add_option ("-v", "--verbose", type="int", \
                    default=default_mimo_control_setup.d_options['verbose'], \
                    help="set verbose level of output [default=%default]")
        if not parser.has_option("--enable-beamforming"):
            parser.add_option("", "--enable-beamforming", action="store_true", \
                    default=default_mimo_control_setup.d_options['enable_beamforming'],
                    help="enable transmit beamforming in the PHY [default=%default]")
        PyHydra.control.add_parser_options (parser)
        MimoMPIF.add_parser_options (parser)

    add_parser_options = Callable (add_parser_options)
