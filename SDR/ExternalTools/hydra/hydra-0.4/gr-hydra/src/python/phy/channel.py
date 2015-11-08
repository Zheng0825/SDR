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
from gnuradio import eng_notation
from optparse import OptionParser

from hydra import phy as phylib
from hydra.PyHydra import FuncThread, Callable, RadioRx, RadioTx
from address import Address, AddrServer, AddrClient
import sys
import time

class default_phy_channel_setup:
    d_options = {'verbose':0, 'debug':1, 'disable_gui':False, \
                 'fake_rf':False, 'nchannels':1, \
                 'ifname':None, \
                 'addrserver_addr':'0.0.0.0', \
                 'addrserver_port':50999, \
                 'channel_addr':'0.0.0.0', \
                 'channel_port':51000, \
                 'enable_channel':False}

    def get_options():
        dopt = default_phy_channel_setup()
        for s in default_phy_channel_setup.d_options:
            if s not in dir(dopt):
                setattr(dopt, s, default_phy_channel_setup.d_options[s] )
        return dopt
    
    def check_options(options):
        for s in default_phy_channel_setup.d_options:
            if s not in dir(options):
                setattr(options, s, default_phy_channel_setup.d_options[s] )

    get_options = Callable (get_options)
    check_options = Callable (check_options)

class Channel(object):
    def __init__(self, model, options):
        # check for default options before continuing with constructor
        default_phy_channel_setup.check_options(options)
        self.verbose = options.verbose
        self.options = options

        # instantiate blocks
        self.model = model
        self.impl = phylib.phy_channel("phy_channel", model,    \
                                       options.addrserver_addr, \
                                       options.addrserver_port)
        self.addr, self.port = None, None
        self.addrclient = None

        # set options
        self._connected = 0
        self.radioconnected = lambda: (self._connected==3)
        self.fake_rf = options.fake_rf
        self.nchannels = options.nchannels
        self.model.set_ntx(self.nchannels)
        self.model.set_nrx(self.nchannels)
        self.impl.set_debug(options.debug)

    def set_debug(self, d):
        self.impl.set_debug(d)
        self.model.set_debug(d)

    def connect_phy(self, p):
        if hasattr(p, 'txpath'): self.connect_radiotx(p.txpath)
        if hasattr(p, 'rxpath'): self.connect_radiorx(p.rxpath)

    def connect_radiorx(self, r):
        if self.fake_rf and isinstance(r, RadioRx):
            self._connected |= 1
            self.impl.set_rxQ(r.src.inputQ())
        else:
            sys.stderr.write("[CHANNEL]: Cannot connect non-RadioRx!\n")

    def connect_radiotx(self, t): 
        if self.fake_rf and isinstance(t, RadioTx):
            self._connected |= 2
            self.impl.set_txQ(t.sink.outputQ())
        else:
            sys.stderr.write("[CHANNEL]: Cannot connect non-RadioTx!\n")

    def __getattr__(self, key):
        if hasattr(self.impl, key):
            return getattr(self.impl, key)
        else:
            return object.__getattr__(self, key)

    def __del__(self, *args):
        self.stop()

    def stop(self):
        self.impl.stop()

    def start(self, addr="0.0.0.0", port=None):
        # Check if radio connected
        destaddr, error = None, False
        assert(self.radioconnected() ), "RadioRx and RadioTx have not been connected to Channel!"
        # Start server
        if port is None: port = self.options.channel_port
        self.impl.start(addr, port)
        # Get server address
        # wait for server and client to start running
        while (not self.impl.server_running() ): time.sleep(0.5)
        while (not self.impl.client_running() ): time.sleep(0.5)
        # should probably update this with correct IP address for matching
        self.addr = self.impl.recv_addr()
        self.port = self.impl.recv_port()
        print "[CHANNEL]: server and client are running, recv_addr =", (self.addr, self.port)
        # Connect to address server and register
        self.addrclient = AddrClient(self.options)
        regaddr, regport = Address.get_ip_address(ifname=self.options.ifname, ntoa=True), self.port
        if (self.addrclient.connect()<0):
            sys.stderr.write("[CHANNEL]: Could not connect with address server!\n")
            error = True
        elif (self.addrclient.register(regaddr, regport)<0):
            sys.stderr.write("[CHANNEL]: Could not connect with address server!\n")
            error = True
        else:
            destaddr = self.addrclient.get_dest_addr()
            if (destaddr < 0):
                sys.stderr.write("[CHANNEL]: Error getting destination address\n")
                error = True
        sys.stderr.write("[CHANNEL]: Destination address = %s\n"%(str(destaddr) ) )
        self.addrclient.close()
        # connect to remote destination
        if not error:
            sys.stderr.write("[CHANNEL]: Connecting  channel client to destination\n")
            self.impl.connect_client(destaddr[0], destaddr[1])
        else:
            self.stop()
            return -1

    """ Add parser options to an OptionParser """
    def add_parser_options (parser):
        if not parser.has_option("-v"):
            parser.add_option ("-v", "--verbose", type="int", \
                    default=default_phy_channel_setup.d_options['verbose'], \
                    help="set verbose level of output [default=%default]")
        if not parser.has_option("--debug"):
            parser.add_option ("", "--debug", type="int", \
                    default=default_phy_channel_setup.d_options['debug'], \
                    help="set debug level of system [default=%default]")
        if not parser.has_option("-n"):
            parser.add_option ("-n", "--nchannels", type="int", \
                    default=default_phy_channel_setup.d_options['nchannels'], \
                    help="set number of channels (or antennas) on USRP board [default=%default]")
        if not parser.has_option("--fake-rf"):
            parser.add_option("", "--fake-rf", action="store_true", \
                    default=default_phy_channel_setup.d_options['fake_rf'],
                    help="enable \"fake\" RF for emulation [default=%default]")
        if not parser.has_option("--ifname"):
            parser.add_option("", "--ifname", type="string", \
                    default=default_phy_channel_setup.d_options['ifname'],
                    help="specify ethernet interface\'s name (e.g. eth0) [default=%default]")
        if not parser.has_option("--addrserver-addr"):
            parser.add_option("", "--addrserver-addr", type="string", \
                    default=default_phy_channel_setup.d_options['addrserver_addr'],
                    help="set address for Address Server [default=%default]")
        if not parser.has_option("--addrserver-port"):
            parser.add_option ("", "--addrserver-port", type="int", \
                    default=default_phy_channel_setup.d_options['addrserver_port'], \
                    help="specify port used by Address Server [default=%default]")
        if not parser.has_option("--channel-addr"):
            parser.add_option("", "--channel-addr", type="string", \
                    default=default_phy_channel_setup.d_options['channel_addr'],
                    help="set address for listening channel server [default=%default]")
        if not parser.has_option("--channel-port"):
            parser.add_option ("", "--channel-port", type="int", \
                    default=default_phy_channel_setup.d_options['channel_port'], \
                    help="specify port used by listening channel server [default=%default]")
        if not parser.has_option("--disable-gui"):
            parser.add_option("", "--disable-gui", action="store_true", \
                    default=default_phy_channel_setup.d_options['disable_gui'],  \
                    help="disable GUI operation [default=%default]")
        if not parser.has_option("--enable-channel"):
            parser.add_option("", "--enable-channel", action="store_true", \
                    default=default_phy_channel_setup.d_options['enable_channel'],  \
                    help="enable operation of Channel [default=%default]")
        AddrClient.add_parser_options (parser)
        AddrServer.add_parser_options (parser)

    add_parser_options = Callable (add_parser_options)
