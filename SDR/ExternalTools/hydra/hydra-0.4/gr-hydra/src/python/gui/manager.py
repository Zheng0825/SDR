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

from hydra import phy, gui
from hydra.PyHydra import Callable, FuncThread
from gnuradio import gr
from control import control
from window import window
from helper import GuiHelper
import wx
import time
import sys

class default_manager_setup:
    d_options = {'verbose':0,'debug':1,'disable_gui':False}

    def get_options():
        dopt = default_manager_setup()
        for s in default_manager_setup.d_options:
            if s not in dir(dopt):
                setattr(dopt, s, default_manager_setup.d_options[s] )
        return dopt
    
    def check_options(options):
        for s in default_manager_setup.d_options:
            if s not in dir(options):
                setattr(options, s, default_manager_setup.d_options[s] )

    get_options = Callable (get_options)
    check_options = Callable (check_options)

class manager(object):
    def __init__(self, options):
        # check for default options before continuing with constructor
        default_manager_setup.check_options(options)
        self.verbose = options.verbose
        self.debug = options.debug
        self.disable = options.disable_gui
        self.isalive = False

        ## create components
        self.debugQ = phy.phyglobal.debugQ()
        self.control = None
        self.mpif, self.phy = None, None # default values
        if not self.disable:
            if window.wxapp is None: window.wxapp = wx.App(0)
            self.control = self.make_control(options)

    def make_control(self, options):
        return control(options)

    def SetMPIF(self, m):
        if self.control: self.control.SetMPIF(m)
        else: self.mpif = m

    def SetPHY(self, p):
        if self.control: self.control.SetPHY(p)
        else: self.phy = p

    def GetPHY(self):
        if hasattr(self.control, 'phy'): return self.control.phy
        else: return self.phy

    def monitor(self):
        while 1:
            m = self.debugQ.delete_head()
            if m.type() == gui.SHUTDOWN: break
            if not self.disable: self.classify(m)
        self.close()
        
    def close(self):
        if self.control: self.control.Close()
        self.control = None
        self.isalive = False

    ## overload this function to change its behavior
    def classify(self, m):
        if   m.type() == gui.TXINFO:
            self.control.SetParam('txinfo', m.to_string() )
        elif m.type() == gui.RXINFO:
            self.control.SetParam('rxinfo', m.to_string() )
        elif m.type() == gui.CONSTELLATION:
            sid = int(m.arg1() )
            if self.control.constellation:
                self.control.constellation.SetParam('data', sid, \
                        GuiHelper.to_cvector(m) )
        elif m.type() == gui.CHANNEL:
            nrx = int(m.arg1() )
            ntx = int(m.arg2() )
            if self.control.channel:
                if (nrx<0):
                    if (ntx<0):
                      #sys.stderr.write("[manager]: UpdatePlot called\n")
                      self.control.channel.SetParam('updateplot')
                      #self.control.channel.UpdatePlot()
                    else:
                      #sys.stderr.write("[manager]: ClearData called\n")
                      self.control.channel.SetParam('cleardata')
                      #self.control.channel.ClearData()
                else:
                    self.control.channel.SetParam('data', \
                            GuiHelper.to_vector(m), nrx, ntx)

    def shutdown(self):
        m = gr.message_from_string("shutdown", gui.SHUTDOWN)
        self.debugQ.insert_tail(m)

    def start_wxApp(self):
        if window.wxapp is not None:
            if not window.wxapp.IsMainLoopRunning():
                window.wxapp.MainLoop()
            else:
                while (window.wxapp.IsMainLoopRunning() ): time.sleep(2)

    ##  Non-Blocking
    def start(self, startgui=True):
        self.isalive = True
        f = FuncThread(self.monitor)
        f.setDaemon(1)
        g = None
        if not self.disable:
            if window.wxapp is None: window.wxapp = wx.App(0)
            g = FuncThread(self.start_wxApp)
            g.setDaemon(1)
            if self.control: self.control.Show(True)
            if startgui: g.start()
        f.start()
        return g

    ##  Stop all threads
    def stop(self):
        self.shutdown()

    ##  Make sure destructor calls stop
    def __del__(self, *args):
        self.stop()
        time.sleep(1)
        window.wxapp = None

    ##  Blocking call
    def run(self):
        g = self.start()
        if not self.disable:
            if g is not None: g.join()
        else:
            print "Press Ctrl-c to quit"
            try:
                while self.isalive: time.sleep(3)
            except KeyboardInterrupt:
                pass
        print "exiting manager ..."
        self.stop()

    def error (self, msg, level=0):
        if self.verbose >= level: sys.stderr.write("MANAGER ERROR: "+str(msg)+"\n")

    """ Add parser options to an OptionParser """
    def add_parser_options (parser):
        if not parser.has_option("-v"):
            parser.add_option ("-v", "--verbose", type="int", \
                    default=default_manager_setup.d_options['verbose'], \
                    help="set verbose level of output [default=%default]")
        if not parser.has_option("--debug"):
            parser.add_option ("", "--debug", type="int", \
                    default=default_manager_setup.d_options['debug'], \
                    help="set debug level of system [default=%default]")
        if not parser.has_option("--disable-gui"):
            parser.add_option("", "--disable-gui", action="store_true", \
                    default=default_manager_setup.d_options['disable_gui'],
                    help="disable GUI operation [default=%default]")
        control.add_parser_options(parser)

    add_parser_options = Callable (add_parser_options)
