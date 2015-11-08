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

from gnuradio.eng_option import eng_option
from optparse import OptionParser

from gnuradio import gr
from hydra import PyHydra
import hydra

from manager import manager
import wx
import sys
import time
import numpy

def bar(m, f):
    x = numpy.array([complex(1,1), complex(1,-1), complex(-1,-1), complex(-1,1)])
    time.sleep(3.0)
    f.SetParam('txinfo', "helloworld")
    time.sleep(3.0)
    f.SetParam('rxinfo', "hello1")
    f.SetParam('upsampling_factor', 20)
    f.SetParam('rx_gain', 30)
    time.sleep(3.0)
    f.SetParam('freq', 400e6)
    time.sleep(2.0)
    if f.constellation:
        f.constellation.SetParam('data', 0, x)
    time.sleep(2.0)
    if f.constellation:
        f.constellation.SetParam('data', 1, x*numpy.exp(complex(0,numpy.pi/4) ) )
    if f.constellation: f.constellation.SetParam('format', 0, 'b-')
    time.sleep(2.0)
    v = (complex(1,1), complex(0,0), complex(-1,-1) )
    s = hydra.gui.helper.to_string_complex(v)
    msg = gr.message_from_string(s, hydra.gui.CONSTELLATION)
    hydra.phy.phyglobal.debugQ().insert_tail(msg)
    time.sleep(4.0)
    #m.shutdown()

def startgui(f,a):
    f.Show(True)
    a.MainLoop()

def main():
    """ Parser Options """
    parser = OptionParser (option_class=eng_option)
    manager.add_parser_options (parser)
    (options, args) = parser.parse_args ()
    if len(args)!=0:
        parser.print_help()
        sys.exit(1)

    phy   = PyHydra.transceiver(options)
    mpif  = PyHydra.mpif_impl(options)
    mpif.set_transceiver(phy)

    g = PyHydra.FuncThread(phy.run)
    g.start()
    mpif.start()

    mgr = manager(options)
    mgr.SetMPIF(mpif)
    
    h = PyHydra.FuncThread(bar, "bar", mgr, mgr.control)
    h.setDaemon(1)
    h.start()
    mgr.run()
    #ctrl.Show(True)
    #app.MainLoop()
    
    ## shutdown unconnected parts
    #mpif.shutdown()
    #phy.shutdown()

if __name__=='__main__':
    main()
