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

from gnuradio import gr

from hydra import PyHydra, gui, mimo
from hydra.PyHydra import Callable, GuiHelper

from mimo_transceiver import MimoTransceiver
from mimo_mpif import MimoMPIF
from mimo_control import MimoControl

import sys
import struct

class default_mimo_manager_setup:
    d_options = {'verbose':0,'debug':1}

    def get_options():
        dopt = default_mimo_manager_setup()
        for s in default_mimo_manager_setup.d_options:
            if s not in dir(dopt):
                setattr(dopt, s, default_mimo_manager_setup.d_options[s] )
        return dopt
    
    def check_options(options):
        for s in default_mimo_manager_setup.d_options:
            if s not in dir(options):
                setattr(options, s, default_mimo_manager_setup.d_options[s] )
        PyHydra.default_manager_setup.check_options(options)

    get_options = Callable (get_options)
    check_options = Callable (check_options)

class MimoManager(PyHydra.manager):
    def __init__(self, options):
        # check for default options before continuing with constructor
        default_mimo_manager_setup.check_options(options)
        self.verbose = options.verbose
        self.debug = options.debug

        # counter to manage debug over/under runs
        self.nunderrun = 0
        self.noverrun  = 0

        PyHydra.manager.__init__(self, options)

    def make_control(self, options):
        return MimoControl(options)

    def throttle_input(self):
        updatenow = 0
        vcsi, vchan, vadapt = 1, 1, 1
        delta = 1
        # update overrun/underrun count
        if (self.debugQ.count() > mimo.MAX_DEBUG_LENGTH):
            self.noverrun += 1
            self.nunderrun = 0
        else:
            self.nunderrun += 1
            self.noverrun   = 0
        # check for excessive underrun/overflows
        if (self.noverrun > 3):
            updatenow = 1
            self.nunderrun = 0
            #if self.control.csi:
            #    vcsi = self.control.csi.period + delta
            if self.control.channel:
                vchan = self.control.channel.chanUprate + delta
            if self.control.self_adapt:
                vadapt = self.control.self_adapt.update_interval + delta
        elif (self.nunderrun > 100):
            updatenow = -1
            #if self.control.csi:
            #    vcsi = max(1, self.control.csi.period-1)
            if self.control.channel:
                vchan = max(1, self.control.channel.chanUprate-1)
            if self.control.self_adapt:
                vadapt = max(1, self.control.self_adapt.update_interval-1)
        # update parameters as needed
        if updatenow:
            #if self.control.csi:
            #    self.control.csi.SetParam('period', vcsi)
            if self.control.channel:
                self.control.channel.SetParam('chanrate', vchan)
            if self.control.self_adapt:
                self.control.self_adapt.SetParam('update interval',vadapt)
        return updatenow

    def classify(self, m):
        if (self.throttle_input()>0) and (self.verbose>9):
            sys.stderr.write("Throttle input, Overrun!!\n")
        elif (self.verbose>9):
            sys.stderr.write("No throttle, debugQ = %d\n"%(self.debugQ.count()))
        if m.type() == gui.RSVD_0:
            """ CSI DATA """
            codebook = int(m.arg1() )
            codebits = int(m.arg2() )
            if self.control.csi:
                disp = True
                s = m.to_string()
                if hasattr(self.control, 'monitor_mac_addr'):
                    if self.control.monitor_mac_addr():
                        disp = self.control.cmp_mac_addr(s[0:mimo.mac_monitor.MAC_ADDR_SIZE])
                        s = s[mimo.mac_monitor.MAC_ADDR_SIZE:]
                if disp and (codebits > 0):
                    self.control.csi.SetParam('data', \
                            GuiHelper.to_ivector(s), codebook, codebits)
                    #print "Limited Feedback CSI data sent, len =", m.length()
                else:
                    self.control.csi.SetParam('codebits', codebits)
        elif m.type() == gui.CHANNEL:
            """ CHANNEL DATA """
            if self.control.channel:
                disp = True
                s = m.to_string()
                if hasattr(self.control, 'monitor_mac_addr'):
                    if self.control.monitor_mac_addr():
                        disp = self.control.cmp_mac_addr(s[0:mimo.mac_monitor.MAC_ADDR_SIZE])
                        s = s[mimo.mac_monitor.MAC_ADDR_SIZE:]
                if disp:
                    PyHydra.manager.classify(self, \
                            gr.message_from_string(s, m.type(), m.arg1(), m.arg2() ) )
                    #print "Channel with addr"
        elif m.type() == gui.RSVD_1:
            """ MAC ADDRESS """
            if self.control:
                s = m.to_string()
                self.control.SetParam('local mac address', s[0:mimo.mac_monitor.MAC_ADDR_SIZE]);
        elif m.type() == gui.RSVD_2:
            """ PER DATA """
            if self.control:
                if self.control.self_adapt:
                    s = m.to_string()
                    ndata = int(m.arg1())
                    v = struct.unpack('<'+'f'*ndata, s)
                    #sys.stderr.write("[MIMO_MANAGER]: got PER data = %s\n"%(str(v) ) )
                    self.control.self_adapt.SetParam('per', v, ndata)
        elif m.type() == gui.RSVD_3:
            """ THROUGHPUT DATA """
            if self.control:
                if self.control.self_adapt:
                    s = m.to_string()
                    ndata = int(m.arg1())
                    v = struct.unpack('<'+'f'*ndata, s)
                    #sys.stderr.write("[MIMO_MANAGER]: got TPUT data = %s\n"%(str(v)) )
                    self.control.self_adapt.SetParam('tput', v, ndata)
        elif m.type() == gui.RSVD_4:
            """ RATE HISTOGRAM DATA """
            if self.control:
                if self.control.self_adapt:
                    s = m.to_string()
                    nrates = int(m.arg1())
                    v = struct.unpack('<'+'i'*nrates, s)
                    #sys.stderr.write("[MIMO_MANAGER]: got RATEHISTOGRAM data, len = %d\n"%len(v))
                    self.control.self_adapt.SetParam('ratehist', v, nrates)
        elif m.type() == gui.RSVD_5:
            """ ADD PER DATA """
            if self.control:
                if self.control.self_adapt:
                    x = float(m.arg1() )
                    ndata = int(m.arg2())
                    self.control.self_adapt.SetParam('addper', x, ndata)
        elif m.type() == gui.RSVD_6:
            """ ADD TPUT DATA """
            if self.control:
                if self.control.self_adapt:
                    x = float(m.arg1() )
                    ndata = int(m.arg2())
                    self.control.self_adapt.SetParam('addtput', x, ndata)
        else:
            PyHydra.manager.classify(self, m)

    """ Add parser options to an OptionParser """
    def add_parser_options (parser):
        if not parser.has_option("-v"):
            parser.add_option ("-v", "--verbose", type="int", \
                    default=default_mimo_manager_setup.d_options['verbose'], \
                    help="set verbose level of output [default=%default]")
        if not parser.has_option("--debug"):
            parser.add_option ("", "--debug", type="int", \
                    default=default_mimo_manager_setup.d_options['debug'], \
                    help="set debug level of system [default=%default]")
        PyHydra.manager.add_parser_options (parser)
        MimoControl.add_parser_options (parser)

    add_parser_options = Callable (add_parser_options)
