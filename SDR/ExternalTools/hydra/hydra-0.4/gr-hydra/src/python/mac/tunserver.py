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
Server for tunnel interface; contains `TunServer`.

This server opens a tunnel interface and waits for a connection from a
`TunClient`. All data received from the tunnel interface is forwarded to the
client, and all data received from the client will be written to the tunnel
interface.

Running `TunServer`
===================

This file can be run as a standalone program to run the `TunServer`. To do so,
run this python program with root permissions (**required to access the tunnel
device**).

:author: Ketan Mandke <mandke@ece.utexas.edu>
"""

import os
import struct
import socket

from optparse import OptionParser
import sys

from fsm import FSM
import tunconfig as TUN

# Parameters taken from socket definitions in c++
TUNSETIFF   = 0x400454CA
IF_NAMESIZE = 16
IFHWADDRLEN = 6
IFF_TUN     = 0x0001
IFF_TAP     = 0x0002
IFF_NO_PI   = 0x1000

def _ifconfig(s="", verbose=False):
    """Internal method to call /sbin/ifconfig with arguments."""
    IFCONFIG_EXEC = "/sbin/ifconfig"
    if verbose>TUN.VERBOSE:
        cfg_exec = "%s -v %s"%(IFCONFIG_EXEC, s)
        sys.stderr.write(cfg_exec+"\n")
        return os.system(cfg_exec)
    else:
        return os.system("%s %s 2>/dev/null"%(IFCONFIG_EXEC, s) )

def _read_config_file(config, fconf):
    """Internal method to read configuration file fconf into `config`."""
    if fconf is not None:
        f = file(fconf, "r")
        d, s = {}, f.readline()
        while s:
            try:
                d = eval(s)
                assert isinstance(d, dict)
                s = f.readline()
            except Exception:
                d, s = {}, f.readline()
                continue
            # translate common aliases
            trans = {'ip': ('ip', 'IP'), 'mac':('mac', 'MAC', 'eth', 'ETH') }
            for key in trans:
                for val in trans[key]:
                    if val in d:
                        setattr(config, key, d[val])
                        del d[val]
            # copy remaining values
            for val in d:
                setattr(config, val.replace("-", "_"), d[val])
        f.close()

class TunServer(FSM):
    """
    Tunnel interface server.

    Open tunnel interface and listen for connection with `TunClient`.
    """
    def __init__(self, name="tun/tap server", options=None, \
                 initstate=None, verbose=None, **kwargs):
        """
        Constructor.

        :param name:      Name of server.
        :param options:   Configuration options (see `tunserver_defaults`).
        :param initstate: Ignored by TunServer.
        :param verbose:   Passed to `FSM` constructor.
        :param kwargs:    Paseed to `FSM` constructor.
        """
        if options is None: options = tunserver_defaults
        self.verbose   = options.verbose
        self.__options = options
        self.__tun = None
        self.__recv = None
        self.__sock = None
        self.__conn = None
        self.__client = None
        # call FSM constructor
        initstate = self.CONFIG
        FSM.__init__(self, name=name, initstate=initstate, \
                           verbose=self.verbose, **kwargs)

    # properties
    options    = property(lambda self: self.__options)
    ip         = property(lambda self: self.options.ip)
    mac        = property(lambda self: self.options.mac)
    netmask    = property(lambda self: self.options.netmask)
    tun_device = property(lambda self: self.options.tun_device)
    dev_name   = property(lambda self: self.options.dev_name)
    mtu        = property(lambda self: self.options.mtu)
    # set up during CONFIG
    tun        = property(lambda self: self.__tun)
    recv       = property(lambda self: self.__recv)
    sock       = property(lambda self: self.__sock)
    conn       = property(lambda self: self.__conn)
    client     = property(lambda self: self.__client)
    connected  = property(lambda self: (self.conn is not None) )
    running    = property(lambda self: (self.state == self.SERVICE) )

    def close(self):
        """Shutdown socket of server; also closes any open connections."""
        self.halt()
        if self.sock: self.sock.close()
        _ifconfig("%s down"%(self.dev_name) )

    def CONFIG(self, fsm):
        """
        CONFIG state; configure server.

        In this state, `TunServer` does the following:
            * Open tun/tap interface.
            * Associate tun/tap interface with mac/ip addresses based on
              `options`.
            * Transition to `BIND` to continue execution.
        """
        from fcntl import ioctl
        # open tunnel device
        tun = os.open(self.tun_device, os.O_RDWR)
        # call ioctl to setup tun/tap options for device
        mode = IFF_TAP | IFF_NO_PI
        _dev_name = self.dev_name + "%d"
        ifr_exec = "ifr = struct.pack(\"%dsH\", _dev_name, mode)"%(IF_NAMESIZE)
        exec(ifr_exec)
        ifs = ioctl(tun, TUNSETIFF, ifr)
        ifname = ifs[:IF_NAMESIZE].strip("\x00")

        # associate ifname with ip address
        ip_addr  = self.ip
        netmask  = self.netmask
        eth_addr = self.mac
        mtu      = self.mtu
        _ifconfig("%s %s arp mtu %d "%(ifname, ip_addr, mtu) + \
                 "netmask %s up"%(netmask), self.verbose)
        _ifconfig("%s down"%(ifname), self.verbose)
        if eth_addr is not None:
            if _ifconfig("%s hw ether %s"%(ifname, eth_addr), self.verbose):
                sys.stderr.write("[TUNSERVER]: Cannot continue operation " + \
                                 "with invalid MAC address %s!\n"%(eth_addr) + \
                                 "             Check ifconfig output below " + \
                                 "for valid MAC address.\n\n")
                _ifconfig("%s"%(ifname) )
                self.__options.mac = None
                return self.close()
        _ifconfig("%s up"%(ifname), self.verbose)
    
        # update properties
        self.__options.dev_name = ifname
        self.__tun = tun
        if self.verbose>TUN.VERBOSE:
            sys.stderr.write("opened tunnel interface: (%s, %s)\n"%(ifname, ip_addr) )

        return fsm.goto(self.BIND)

    def BIND(self, fsm):
        """
        BIND state; create TCP socket and bind it to address specified by
        (`TUN.SERVHOST`, `TUN.SERVPORT`).

        This state also spawns a thread to listen to monitor the tunnel
        interface (i.e. `recv`), which executes the `RECV` state. After that,
        the server transitions to `LISTEN`.
        """
        host, port = TUN.SERVHOST, TUN.SERVPORT
        family, stype = socket.AF_INET, socket.SOCK_STREAM
        self.__sock = socket.socket(family, stype)
        try:
            self.sock.bind((host, port) )
        except socket.error, e:
            sys.stderr.write("[TUNSERVER]: Cannot bind to socket (%s,%s)!\n" + \
                             "             Address already in use!\n")
            return self.close()

        # spawn monitor
        self.__recv = FSM(name=self.getName()+".recv", verbose=self.verbose)
        self.recv.goto(self.RECV)
        self.recv.start()

        return fsm.goto(self.LISTEN)

    def RECV(self, fsm):
        """RECV state; listen to tun/tap interface and forward received data to
        `TunClient` (if one is connected and running)."""
        pkt = os.read(self.tun, self.mtu)
        if not pkt: return self.close()
        # if server is connected and running, then forward to client
        if self.connected and self.running: self.conn.send(pkt)
        return fsm.goto(self.RECV)

    def LISTEN(self, fsm):
        """LISTEN state; listen and accept connection from a `TunClient`; then
        transition to `SETUP` state."""
        try:
            self.sock.listen(1)
            self.__conn, self.__client = self.sock.accept()
            return fsm.goto(self.SETUP)
        except Exception:
            self.close()

    def SETUP(self, fsm):
        """SETUP state; pass setup message to `TunClient` to pass control
        information to client before transitioning to `SERVICE`."""
        ctrl = {'ip':self.ip, 'mac':self.mac, 'mtu':self.mtu}
        s = str(ctrl)
        d = struct.pack('I', len(s) ) + s
        self.conn.send(d)
        return fsm.goto(self.SERVICE)

    def SERVICE(self, fsm):
        """SERVICE state; listen for data from `TunClient` and forward to
        tun/tap interface."""
        while 1:
            try: data = self.conn.recv(self.mtu)
            except Exception: break
            if not data: break
            os.write(self.tun, data)
            if fsm.verbose>TUN.VERBOSE: fsm.debug("WROTE %d bytes"%(len(data)) )
        self.conn.close()
        self.__conn, self.__client = None, None
        return fsm.goto(self.LISTEN)

class tunserver_defaults(object):
    """Default options used for `TunServer`."""
    verbose = None
    ip      = "192.168.255.1"
    netmask = "255.255.255.0"
    mac     = "0A:48:D5:09:F5:A9"
    # other valid mac addresses
    #mac = "FF:0A:C1:E8:DB:02"
    #mac = "FA:2A:EC:D4:A7:FC"
    tun_device = "/dev/net/tun"
    dev_name   = "tuntap"
    mtu        = 1500

    @classmethod
    def add_options(cls, parser):
        """Add options to parser."""
        if not parser.has_option("--verbose"):
            parser.add_option("-v", "--verbose", type="string", \
                    default=cls.verbose, \
                    help="set verbose level of output [default=%default]")
        if not parser.has_option("--ip"):
            parser.add_option("", "--ip", type="string", default=cls.ip, \
                    help="IP address for tun/tap interface [default=%default]")
        if not parser.has_option("--mac"):
            parser.add_option("", "--mac", type="string", default=cls.mac, \
                    help="MAC/Ethernet address for tun/tap interface [default=%default]")
        if not parser.has_option("--netmask"):
            parser.add_option("", "--netmask", type="string", \
                    default=cls.netmask, \
                    help="Mask for IP subnet [default=%default]")
        if not parser.has_option("--tun-device"):
            parser.add_option("", "--tun-device", type="string", \
                    default=cls.tun_device, \
                    help="Device for tun/tap interface [default=%default]")
        if not parser.has_option("--dev-name"):
            parser.add_option("", "--dev-name", type="string", \
                    default=cls.dev_name, \
                    help="Reference name for tun/tap device [default=%default]")
        if not parser.has_option("--mtu"):
            parser.add_option("", "--mtu", type="int", default=cls.mtu, \
                    help="Maximum transmission unit received from tunnel interface [default=%default]")
        return parser

def tunserver_mainloop():
    """Run a `TunServer` using configuration options passed through the command
    line or a config file."""
    parser = OptionParser()
    parser.usage = "Usage: %prog [OPTIONS] [CONFIG_FILE]"
    tunserver_defaults.add_options(parser)
    (options, args) = parser.parse_args()
    if len(args)>1:
        parser.print_help()
        sys.exit()

    fconf = None
    # Input configuration file
    if len(args)>0: fconf = args[0]
    _read_config_file(options, fconf)

    # attempt to create TunServer
    ts = TunServer(options=options)
    ts.start()

    print "Press (ctrl-c) to quit:"
    try:
        while 1: raw_input("")
    except KeyboardInterrupt:
        pass

if __name__=="__main__":
    tunserver_mainloop()
