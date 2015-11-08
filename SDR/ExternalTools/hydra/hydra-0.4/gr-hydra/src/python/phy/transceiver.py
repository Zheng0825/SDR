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

from gnuradio import gr
from hydra import rf, phy
from hydra.PyHydra import RadioRx, RadioTx, Callable
import sys

class default_phy_setup:
    d_options = {'verbose':0, 'debug':1, \
                 'nchannels':1, 'upsample':1, \
                 'tx_gain':4000.0, 'rx_gain': 40.0, \
                 'inband':False}

    def get_options():
        dopt = default_phy_setup()
        for s in default_phy_setup.d_options:
            if s not in dir(dopt):
                setattr(dopt, s, default_phy_setup.d_options[s] )
        return dopt
    
    def check_options(options):
        for s in default_phy_setup.d_options:
            if s not in dir(options):
                setattr(options, s, default_phy_setup.d_options[s] )

    get_options = Callable (get_options)
    check_options = Callable (check_options)

class transceiver (gr.top_block):
    
    def __init__(self, options):
        gr.top_block.__init__(self)
        # check for default options before calling constructor
        default_phy_setup.check_options(options)
        self.verbose = options.verbose
        self.debug = options.debug

        # instantiate blocks
        self.tx = self.make_tx(options)
        self.rx = self.make_rx(options)
        # create runtime environment for mblocks
        self.rt = None
        if options.inband:
            self.rt = rf.runtime(True, True)
        self.txpath, self.rxpath = None, None
        self.txpath = RadioTx(options)
        if self.rx: self.rxpath = RadioRx(options)

        if (self.tx==None): self.warning("tx is not defined.")
        if (self.rx==None): self.warning("rx is not defined.")

        # connect hier_block2's to RadioRx and RadioTx
        if self.rx: self.connect(self.rxpath, self.rx)
        if self.tx: self.connect(self.txpath)

        # set options
        if hasattr(self.tx, 'set_debug'): self.tx.set_debug(options.debug)
        if hasattr(self.rx, 'set_debug'): self.rx.set_debug(options.debug)
        self.set_nchannels(options.nchannels)
        self.sample_rate = options.sample_rate
        self.set_upsample(options.upsample)
        self.set_tx_gain(options.tx_gain)
        self.set_rx_gain(options.rx_gain)

        # connect other relevant parameters
        if self.tx: self.tx.set_outputQ(self.txpath.pad.inputQ() )

    def make_tx(self, options):
        return phy.tx()

    def make_rx(self, options):
        return phy.rx()

    def stop(self):
        if self.txpath: self.txpath.shutdown()
        else: return
        if self.rxpath: self.rxpath.shutdown()
        else: return
        if not self.txpath.fake_rf: gr.top_block.stop(self)
        self.txpath = None
        self.rxpath = None

    def shutdown(self):
        print "phy.shutdown called ..."
        self.stop()

    def set_nchannels (self, n):
        self.nchannels = n
        if self.tx: self.tx.set_ntx(n)
        if self.rx: self.rx.set_nrx(n)

    def set_upsample (self, m):
        self.upsample = default_phy_setup.d_options['upsample']
        if self.tx:
            self.tx.set_upsampling_factor(m)
            self.upsample = self.tx.upsampling_factor()
        if (self.verbose>4): sys.stderr.write("upsample factor = %d\n"%(self.upsample) )
        self.set_sample_rate(self.sample_rate)

    def set_tx_gain (self, g):
        self.tx_gain = default_phy_setup.d_options['tx_gain']
        if self.tx:
            self.tx.set_gain(g)
            self.tx_gain = self.tx.gain()
        if (self.verbose>4): sys.stderr.write("tx gain = %s\n"%(int(self.tx_gain)) )

    def set_rx_gain (self, gdb):
        self.rx_gain = default_phy_setup.d_options['rx_gain']
        if self.rxpath:
            self.rxpath.set_rx_gain(gdb)
            self.rx_gain = self.rxpath.rx_gain
        if (self.verbose>4): sys.stderr.write("rx gain = %d dB\n"%(int(self.rx_gain)) )

    """ Other control methods for controlling rf from transceiver """
    def set_freq(self, f):
        self.rxpath.set_freq(f)
        self.txpath.set_freq(f)
        self.freq = self.rxpath.freq

    def set_sample_rate(self, r):
        self.txpath.set_sample_rate(r*self.upsample)
        self.rxpath.set_sample_rate(r)
        if (self.verbose>4): sys.stderr.write("Tx sample rate = %.2fM\n"%(self.txpath.sample_rate/1.0e6) )
        if (self.verbose>4): sys.stderr.write("Rx sample rate = %.2fM\n"%(self.rxpath.sample_rate/1.0e6) )
        self.sample_rate = self.rxpath.sample_rate

    def set_tx_mode(self, m):
        if hasattr(self.txpath.pad, 'set_continuous'):
            self.txpath.pad.set_continuous(m)

    """ 
    Transmit/receive functions are depricated. They should not be used in
    conjunction with MAC/PHY interface objects defined in mpif. When operating
    with mpif objects, you will need to pass the transmitter (self.tx) and the
    receive output queue (self.rx.outputQ() ) to the configuration functions
    mpif_tx.set_transmitter() and mpif_rx.set_inputQ().
    """
    def transmit(self, s):
        if self.tx: self.tx.transmit (s)

    def receive(self):
        ##  DEPRICATED: do not use!
        #   check the global control queue phy.phyglobal.ctrlQ() instead
        return self.rx.outputQ().delete_head()

    def warning (self, msg, level=0):
        if self.verbose >= level: sys.stderr.write("PHY warning: "+str(msg)+"\n")

    def error (self, msg, level=0):
        if self.verbose >= level: sys.stderr.write("PHY ERROR: "+str(msg)+"\n")
    
    """ Add parser options to an OptionParser """
    def add_parser_options (parser):
        if not parser.has_option("-v"):
            parser.add_option ("-v", "--verbose", type="int", \
                    default=default_phy_setup.d_options['verbose'], \
                    help="set verbose level of output [default=%default]")
        if not parser.has_option("--debug"):
            parser.add_option ("", "--debug", type="int", \
                    default=default_phy_setup.d_options['debug'], \
                    help="set debug level of system [default=%default]")
        if not parser.has_option("-n"):
            parser.add_option ("-n", "--nchannels", type="int", \
                    default=default_phy_setup.d_options['nchannels'], \
                    help="set number of channels (or antennas) on USRP board [default=%default]")
        if not parser.has_option("-G"):
            parser.add_option ("-G", "--rx-gain", type="eng_float", \
                    default=default_phy_setup.d_options['rx_gain'], \
                    help="set usrp receive gain in dB [default=%default]")
        if not parser.has_option("-g"):
            parser.add_option ("-g", "--tx-gain", type="eng_float", \
                    default=default_phy_setup.d_options['tx_gain'], \
                    help="set software transmit gain in dB [default=%default]")
        if not parser.has_option("-u"):
            parser.add_option ("-u", "--upsample", type="int",
                    default=default_phy_setup.d_options['upsample'], \
                    help="set the upsampling factor [default=%default]")
        if not parser.has_option("--inband"):
            parser.add_option("", "--inband", action="store_true", \
                    default=default_phy_setup.d_options['inband'],
                    help="enable inband signalling in RF [default=%default]")
        RadioRx.add_parser_options(parser)
        RadioTx.add_parser_options(parser)

    add_parser_options = Callable (add_parser_options)
