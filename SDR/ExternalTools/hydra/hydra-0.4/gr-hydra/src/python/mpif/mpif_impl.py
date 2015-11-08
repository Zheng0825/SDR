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

from gnuradio import gr
from hydra import mpif
from hydra.PyHydra import Callable, transceiver

class default_mpif_setup:
    d_options = {'verbose':0, 'debug':1, 'enable_socket':False,
                 'mac_addr':'0.0.0.0', 'mac_port':60000, 'auto_tx_gain':False,
                 'mtu':2500}

    def get_options():
        dopt = default_mpif_setup()
        for s in default_mpif_setup.d_options:
            if s not in dir(dopt):
                setattr(dopt, s, default_mpif_setup.d_options[s] )
        return dopt
    
    def check_options(options):
        for s in default_mpif_setup.d_options:
            if s not in dir(options):
                setattr(options, s, default_mpif_setup.d_options[s] )

    get_options = Callable (get_options)
    check_options = Callable (check_options)

class mpif_impl(object):
    def __init__(self, options):
        # check for default options before continuing with constructor
        default_mpif_setup.check_options(options)
        self.verbose = options.verbose
        self.debug = options.debug

        # instantiate mpif threads and blocks
        self.tx = self.make_tx(options)
        self.rx = self.make_rx(options)
        self.phy = None

        # set additional options
        if hasattr(self.tx, 'set_debug'): self.tx.set_debug(options.debug)
        if hasattr(self.rx, 'set_debug'): self.rx.set_debug(options.debug)
        if hasattr(self.tx, 'set_auto_power'):
            self.tx.set_auto_power(options.auto_tx_gain)

        # connect components as needed
        if self.rx and self.tx:
            if 'set_mpiftx' in dir(self.rx): self.rx.set_mpiftx(self.tx)

    def make_tx (self, options):
        return mpif.tx(options.enable_socket, options.mac_addr, options.mac_port+1)

    def make_rx (self, options):
        return mpif.rx(options.enable_socket, options.mac_addr, options.mac_port)

    def set_transceiver(self, t):
        self.set_phy(t)
    
    def set_phy(self, p):
        self.phy = p
        if self.phy:
            if self.tx and self.phy.tx: self.tx.set_transmitter(self.phy.tx)
            if self.rx and self.phy.rx: self.rx.set_inputQ(self.phy.rx.outputQ() )

    def start(self):
        if self.tx: self.tx.start()
        if self.rx: self.rx.start()

    def stop(self):
        if self.tx: self.tx.stop()
        if self.rx: self.rx.stop()

    def shutdown(self):
        print "mpif.shutdown called ...."
        self.stop()
        if self.phy: self.phy.shutdown()

    def error (self, msg, level=0):
        if self.verbose >= level: sys.stderr.write("MPIF ERROR: "+str(msg)+"\n")
    
    """ Add parser options to an OptionParser """
    def add_parser_options (parser):
        if not parser.has_option("-v"):
            parser.add_option ("-v", "--verbose", type="int", \
                    default=default_mpif_setup.d_options['verbose'], \
                    help="set verbose level of output [default=%default]")
        if not parser.has_option("--debug"):
            parser.add_option ("", "--debug", type="int", \
                    default=default_mpif_setup.d_options['debug'], \
                    help="set debug level of system [default=%default]")
        if not parser.has_option("--enable-socket"):
            parser.add_option("", "--enable-socket", action="store_true", \
                    default=default_mpif_setup.d_options['enable_socket'],
                    help="enable socket communication for MAC/PHY interface [default=%default]")
        if not parser.has_option("--mac-addr"):
            parser.add_option("-A", "--mac-addr", type="string", \
                    default=default_mpif_setup.d_options['mac_addr'],
                    help="set address for MAC [default=%default]")
        if not parser.has_option("--mac-port"):
            parser.add_option ("-P", "--mac-port", type="int", \
                    default=default_mpif_setup.d_options['mac_port'], \
                    help="set base port used for MAC/PHY interface [default=%default]")
        if not parser.has_option("--mtu"):
            parser.add_option ("", "--mtu", type="int", \
                    default=default_mpif_setup.d_options['mtu'], \
                    help="set maximum transmission unit default=%default]")
        if not parser.has_option("--auto-tx-gain"):
            parser.add_option("", "--auto-tx-gain", action="store_true", \
                    default=default_mpif_setup.d_options['auto_tx_gain'],
                    help="enable automatic transmit gain control from MAC/PHY interface [default=%default]")
        transceiver.add_parser_options (parser)

    add_parser_options = Callable (add_parser_options)
