#! /usr/bin/env python
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
Dummy mac sends data to MPIF and allows monitoring of received messages on this
interface; useful for debugging.

:author: Ketan Mandke <mandke@ece.utexas.edu>
"""

from gnuradio.eng_option import eng_option
from optparse import OptionParser

from gnuradio import gr
from hydra import mpif, mimo
from hydra.PyHydra import FuncThread

from mimo_mpif import MimoMPIF

import time
from numpy import log10

def send_80211data(mac):
    time.sleep(1.0)
    k=0
    while 1:
        Ptx = 8000**2
        MCS = k%RATE_LIMIT
        k = (k+1)%1000
        #data = "helloworld"*(k%10+10)           # create data to send
        data = "helloworld"*10                 # create data to send
        # create 802.11 data packet
        macdata = mimo.mimo_mac.ieee80211_data(data)
        sbase = ""
        mac.send(macdata, sbase, MCS, Ptx)
        print "\nTXDATA sent, blen=%d, dlen=%d, MCS=%d, power=%.3f"%(len(sbase),len(macdata), MCS, Ptx)
        #mpif.tx.inputQ().insert_tail(gr.message_from_string(s, hydra.mpif.TXDATA) )
        time.sleep(0.50)


def send_data(mac):
    time.sleep(2.0)
    MCS, Ptx, sbase, sdata =  0, 1000, "hello"*10, "helloworld"*20
    mac.send(sbase, MCS, Ptx)
    print "\nTXDATA sent, blen=%d, dlen=%d, MCS=%d, power=%.3f"%(len(sbase), len(sdata), MCS, Ptx)
    time.sleep(2.0)
    MCS, Ptx, sbase, sdata =  0, 1000, "hello"*10, "helloworld"*20
    mac.send(sdata, sbase, MCS, Ptx)
    print "\nTXDATA sent, blen=%d, dlen=%d, MCS=%d, power=%.3f"%(len(sbase), len(sdata), MCS, Ptx)
    L = 100
    for k in range(L):
        time.sleep(0.5)
        MCS, Ptx, sbase, sdata =  k%15, 3000+(100*k), "" , "helloworld"*20
        mac.send(sdata, sbase, MCS, Ptx)
        print "\nTXDATA sent, blen=%d, dlen=%d, MCS=%d, power=%.3f"%(len(sbase), len(sdata), MCS, Ptx)
        time.sleep(0.5)
        MCS, Ptx, sbase, sdata =  k%15, 3000+(100*k), "hello"*10, "helloworld"*20
        mac.send(sdata, sbase, MCS, Ptx)
        print "\nTXDATA sent, blen=%d, dlen=%d, MCS=%d, power=%.3f"%(len(sbase), len(sdata), MCS, Ptx)

def recv_data(mac):
    while 1:
        m = mac.outputQ().delete_head()
        snrdb = -9999.0
        if (m.arg2()>0): snrdb = 10*log10(m.arg2() )
        if m.type()==mpif.RXDATA:
            print "RXDATA recv, ts = %.3f, length = %d,"%(time.time(), m.length() ), \
                    "rate = %d, snr = %.3f dB"%(int(m.arg1() ), snrdb)
        else:
            print "ERROR! INVALID OUTPUT MESSAGE!"

def recv_ctrl(mac):
    while 1:
        m = mac.ctrlQ().delete_head()
        if   m.type()==mpif.CSBUSY:
            print "\nCSBUSY recv, ts = %.3f"%(m.arg1() )
        elif m.type()==mpif.TXDONE:
            print "TXDONE recv, ts = %.3f"%(m.arg1() )
        else:
            print "\nERROR! INVALID CTRL MESSAGE!"

RATE_LIMIT = 16

def main():
    """ Parser Options """
    parser = OptionParser (option_class=eng_option)
    MimoMPIF.add_parser_options (parser)
    if not parser.has_option("--phy-addr"):
        parser.add_option("", "--phy-addr", type="string", default="0.0.0.0", \
                help="set address for PHY [default=%default]")
    (options, args) = parser.parse_args ()
    if len(args)!=0:
        parser.print_help()
        sys.exit(1)

    global RATE_LIMIT
    if   (options.nchannels == 4): RATE_LIMIT = 32
    elif (options.nchannels == 2): RATE_LIMIT = 16
    elif (options.nchannels == 1): RATE_LIMIT = 8

    mac = mimo.mac(options.phy_addr, options.mac_port)
    mac.start()

    #f = FuncThread(send_data, "tx-data", mac)
    f = FuncThread(send_80211data, "tx-data", mac)
    f.setDaemon(1)
    g = FuncThread(recv_data, "rx-data", mac)
    g.setDaemon(1)
    h = FuncThread(recv_ctrl, "rx-ctrl", mac)
    h.setDaemon(1)

    for x in [f,g,h]: x.start()
    #for x in [g,h]: x.start()

    raw_input("Press Enter to quit:\n")
    mac.shutdown()

    p = persist()  # start a persitent thread

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
        while 1:
            pass

if __name__=='__main__':
    main()
