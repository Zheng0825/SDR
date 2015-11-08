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
from hydra import PyHydra, mimo
from hydra.PyHydra import Callable

from mimo_transceiver import MimoTransceiver

class default_mimo_mpif_setup:
    d_options = {'verbose':0, 'debug':1, 'enable_socket':False,
                 'mac_addr':'0.0.0.0', 'mac_port':60000,
                 'mtu':2500, 'rate': -1}

    def get_options():
        dopt = default_mimo_mpif_setup()
        for s in default_mimo_mpif_setup.d_options:
            if s not in dir(dopt):
                setattr(dopt, s, default_mimo_mpif_setup.d_options[s] )
        return dopt
    
    def check_options(options):
        for s in default_mimo_mpif_setup.d_options:
            if s not in dir(options):
                setattr(options, s, default_mimo_mpif_setup.d_options[s] )
        PyHydra.default_mpif_setup.check_options(options)

    get_options = Callable (get_options)
    check_options = Callable (check_options)

class MimoMPIF(PyHydra.mpif_impl):
    def __init__(self, options):
        # check for default options before continuing with constructor
        default_mimo_mpif_setup.check_options(options)
        self.verbose = options.verbose
        self.debug = options.debug

        PyHydra.mpif_impl.__init__(self, options)

        # set remaining options
        self.set_rate(options.rate)

    def make_tx (self, options):
        return mimo.mpiftx(options.enable_socket, options.mac_addr, options.mac_port+1)

    def make_rx (self, options):
        return mimo.mpifrx(options.enable_socket, options.mac_addr, options.mac_port)

    def set_rate(self, r):
        if r < 0:
            self.tx.set_auto_rate(True)
        else:
            self.tx.set_auto_rate(False)
            self.tx.set_rate(r)
    
    """ Add parser options to an OptionParser """
    def add_parser_options (parser):
        if not parser.has_option("-v"):
            parser.add_option ("-v", "--verbose", type="int", \
                    default=default_mimo_mpif_setup.d_options['verbose'], \
                    help="set verbose level of output [default=%default]")
        if not parser.has_option("--debug"):
            parser.add_option ("", "--debug", type="int", \
                    default=default_mimo_mpif_setup.d_options['debug'], \
                    help="set debug level of system [default=%default]")
        if not parser.has_option("--enable-socket"):
            parser.add_option("", "--enable-socket", action="store_true", \
                    default=default_mimo_mpif_setup.d_options['enable_socket'],
                    help="enable socket communication for MAC/PHY interface [default=%default]")
        if not parser.has_option("--mac-addr"):
            parser.add_option("-A", "--mac-addr", type="string", \
                    default=default_mimo_mpif_setup.d_options['mac_addr'],
                    help="set address for MAC [default=%default]")
        if not parser.has_option("--mac-port"):
            parser.add_option ("-P", "--mac-port", type="int", \
                    default=default_mimo_mpif_setup.d_options['mac_port'], \
                    help="set base port used for MAC/PHY interface [default=%default]")
        if not parser.has_option("--mtu"):
            parser.add_option ("", "--mtu", type="int", \
                    default=default_mimo_mpif_setup.d_options['mtu'], \
                    help="set maximum transmission unit default=%default]")
        if not parser.has_option("-R"):
            parser.add_option ("-R", "--rate", type="int", \
                    default=default_mimo_mpif_setup.d_options['rate'], \
                    help="set PHY data rate (use -1 for auto-rate) [default=\"auto\"]")
        MimoTransceiver.add_parser_options (parser)

    add_parser_options = Callable (add_parser_options)

