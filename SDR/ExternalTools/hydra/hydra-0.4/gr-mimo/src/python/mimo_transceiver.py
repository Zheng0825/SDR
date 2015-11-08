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

"""
Setup a MIMO PHY protocol object.

This will inherit from PyHydra.transceiver and will contain mimo.tx() and
mimo.rx() elements.
"""

from gnuradio import gr
from hydra import mimo, phy, PyHydra
from hydra.PyHydra import Callable

class default_mimo_phy_setup:
    d_options = {'verbose':0, 'debug':1, \
                 'nchannels':2, 'pd_threshold':70, \
                 'codebits': 3}

    def get_options():
        dopt = default_mimo_phy_setup()
        for s in default_mimo_phy_setup.d_options:
            if s not in dir(dopt):
                setattr(dopt, s, default_mimo_phy_setup.d_options[s] )
        return dopt
    
    def check_options(options):
        for s in default_mimo_phy_setup.d_options:
            if s not in dir(options):
                setattr(options, s, default_mimo_phy_setup.d_options[s] )
        PyHydra.default_phy_setup.check_options(options)

    get_options = Callable (get_options)
    check_options = Callable (check_options)

class MimoTransceiver(PyHydra.transceiver):
    def __init__(self, options):
        # check for default options before calling constructor
        default_mimo_phy_setup.check_options(options)
        self.verbose = options.verbose
        self.debug = options.debug

        PyHydra.transceiver.__init__(self, options)

        # set additional options
        self.set_thresh(options.pd_threshold/100.0)
        if hasattr(self.rx, "set_lfb_codebits"):
            self.rx.set_lfb_codebits(options.codebits)
        

    def make_tx(self, options):
        return mimo.tx()

    def make_rx(self, options):
        return mimo.rx()

    def set_thresh(self, t):
        t = max(min(t,1.00), 0.0)
        if hasattr(self.rx, 'packet_detector'):
            self.rx.packet_detector().set_thresh(t)

    """ Add parser options to an OptionParser """
    def add_parser_options (parser):
        if not parser.has_option("-v"):
            parser.add_option ("-v", "--verbose", type="int", \
                    default=default_mimo_phy_setup.d_options['verbose'], \
                    help="set verbose level of output [default=%default]")
        if not parser.has_option("--debug"):
            parser.add_option ("", "--debug", type="int", \
                    default=default_mimo_phy_setup.d_options['debug'], \
                    help="set debug level of system [default=%default]")
        if not parser.has_option("-n"):
            parser.add_option ("-n", "--nchannels", type="int", \
                    default=default_mimo_phy_setup.d_options['nchannels'], \
                    help="set number of channels (or antennas) on USRP board [default=%default]")
        if not parser.has_option("-T"):
            parser.add_option ("-T", "--pd-threshold", type="eng_float", \
                    default=default_mimo_phy_setup.d_options['pd_threshold'], \
                    help="set packet detector threshold (percentage %) [default=%default]")
        if not parser.has_option(""):
            parser.add_option ("", "--codebits", type="int", \
                    default=default_mimo_phy_setup.d_options['codebits'], \
                    help="set number of bits used in limited feedback [default=%default]")
        PyHydra.transceiver.add_parser_options(parser)

    add_parser_options = Callable (add_parser_options)
