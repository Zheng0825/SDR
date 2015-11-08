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

"""
from pylab import *
"""

from gnuradio.eng_option import eng_option
from optparse import OptionParser

from gnuradio import gr
from hydra.PyHydra import FuncThread, GuiHelper
from hydra.PyHydra import Channel
import hydra

from mimo_manager import MimoManager
from mimo_transceiver import MimoTransceiver
from mimo_mpif import MimoMPIF
import sys
import time
import numpy
import struct

def startgui(f,a):
    f.Show(True)
    a.MainLoop()

def bar(mpif):
    time.sleep(5.0)
    sys.stderr.write("bar running ...\n")
    """
    ## packet 1
    MCS = 5
    data = "helloworld"*10
    v = hydra.mimo.txvector()
    v.set_length(len(data) )
    v.set_power(8000.0)
    v.set_rate(MCS)
    h = v.to_string()
    s = h+data
    mpif.tx.inputQ().insert_tail(gr.message_from_string(s, hydra.mpif.TXDATA) )
    ## packets 2 to L
    L = 100
    #for k in range(L):
    """
    k=0
    m_monitor = hydra.mimo.mac_monitor()
    while 1:
        k = (k+1)%1000
        time.sleep(1.0)
        MCS = k%RATE_LIMIT
        #data = "helloworld"*(k%10+10)           # create data to send
        data = "helloworld"*10                 # create data to send
        # create 802.11 data packet
        macdata = hydra.mimo.mimo_mac.ieee80211_data(data)
        v = hydra.mimo.txvector()
        v.set_length(len(macdata) )
        v.set_power(8000.0)
        v.set_rate(MCS)
        h = v.to_string()
        s = h+macdata
        mpif.tx.inputQ().insert_tail(gr.message_from_string(s, hydra.mpif.TXDATA) )

"""
TxData = ()
def txplot(src):
    global TxData
    m = src.delete_head()
    s = m.to_string()
    Ntx = int(m.arg1() )
    M = gr.sizeof_gr_complex
    Nsamp = int(m.length() ) / (M*Ntx)
    x = [() for k in range(Ntx) ]
    for k in range(Nsamp):
        for j in range(Ntx):
            xr, xi = struct.unpack('<ff', s[(k*Ntx + j)*M: (k*Ntx+j+1)*M])
            x[j] += (complex(xr,xi), )
    TxData = numpy.array(x[0], numpy.complex64)
"""

RATE_LIMIT = 16
def main():
    """ Parser Options """
    parser = OptionParser (option_class=eng_option)
    MimoManager.add_parser_options (parser)
    Channel.add_parser_options (parser)
    (options, args) = parser.parse_args ()
    if len(args)!=0:
        parser.print_help()
        sys.exit(1)

    # overwrite fake_rf when --enable-channel is used
    if options.enable_channel: options.fake_rf = True

    mgr = MimoManager(options)
    phy   = MimoTransceiver(options)
    phy.rx.packet_detector().set_debug(options.debug)
    phy.rx.packet_detector().set_window(64)
    phy.rx.packet_detector().set_delay(16)
    #phy.rx.packet_detector().set_thresh(0.70)
    phy.rx.packet_detector().set_nchannels(options.nchannels)
    """
    """
    sys.stderr.write("phy.rx.packet_detector().window()    = %s\n"%(phy.rx.packet_detector().window()) )
    sys.stderr.write("phy.rx.packet_detector().delay()     = %s\n"%(phy.rx.packet_detector().delay()) )
    sys.stderr.write("phy.rx.packet_detector().nchannels() = %s\n"%(phy.rx.packet_detector().nchannels()) )
    #phy.rx.set_max_buffer_load(0.0)
    sys.stderr.write("phy.rx.nrx() = %s\n"%(phy.rx.nrx()) )
    sys.stderr.write("phy.tx.nrx() = %s\n"%(phy.tx.ntx()) )
    sys.stderr.write("phy.rx.max_buffer_size() = %s\n"%(phy.rx.max_buffer_size()) )
    phy.txpath.pad.set_npre(2)
    phy.txpath.pad.set_npost(2)
    mpif  = MimoMPIF(options)
    mpif.set_transceiver(phy)

    global RATE_LIMIT
    if   (options.nchannels == 4): RATE_LIMIT = 32
    elif (options.nchannels == 2): RATE_LIMIT = 16
    elif (options.nchannels == 1): RATE_LIMIT = 8

    mpif.start()
    phy.start()

    mgr.SetMPIF(mpif)

    ch = None
    if (options.fake_rf and options.enable_channel):
        Nodb = 10
        chmodel = hydra.phy.channel_model()
        chmodel.set_noise_power_db(Nodb)
        ch = Channel(chmodel, options)
        ch.connect_phy(phy)
        ch.start()
    
    if not options.enable_socket:
        f = FuncThread(bar, "bar", mpif)
        f.setDaemon(1)
        f.start()
    elif mpif.tx.socket_enable():
        sys.stderr.write("MPIF TXPORT = %s\n"%(mpif.tx.port()) )
        sys.stderr.write("MPIF RXPORT = %s\n"%(mpif.rx.port()) )
    
    #mgr.run()
    g = mgr.start()
    if options.disable_gui:
        sys.stderr.write("Press Ctrl-c to quit\n")
        try:
            while 1: time.sleep(4)
        except KeyboardInterrupt:
            sys.stderr.write("exiting ...\n")
    else:
        if g is not None: g.join()
    mgr.stop()
    
    """
    if (len(TxData)>0):
        plot(numpy.abs(TxData) )
        grid()
        show()
    """
    
    if (ch is not None):
      sys.stderr.write("CALLING Channel.stop()\n")
      ch.stop()
    sys.stderr.write("CALLING mpif.shutdown()\n")
    mpif.shutdown()
    for dd in range(8):
        time.sleep(0.5)
        sys.stderr.write(".")
    sys.stderr.write("\n")


    p = persist()  # start a persitent thread
    
    ## shutdown unconnected parts
    #mpif.shutdown()
    #phy.shutdown()

    #del phy # delete remaining handles for USRP
    #del mpif
    #del mgr


""" 
Class persist:
    Start a persistant thread that will allow other
    threads to close properly.
"""
class persist(object):
    def __init__(self):
        p = FuncThread(self.run)
        p.setDaemon(1)
        p.start()
    def run(self):
        while 1: raw_input("")

if __name__=='__main__':
    main()
