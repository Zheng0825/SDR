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
Client for connecting to a `TunServer`; contains `TunClient`.

This client can be used to get data to/from a `TunServer` which manages a kernel
tun/tap interface. By default, `TunClient` will setup the client, and transition
to the `CONFIG` state in which it will `close()` the connection. Overload this
state to specify non-trivial execution for subclasses of `TunClient`.

:author: Ketan Mandke <mandke@ece.utexas.edu>
"""

import struct
import socket

import sys
import time

from fsm import FSM
import tunconfig as TUN

class TunClient(FSM):
    """
    Client to connect to `TunServer`.
    
    This class will `CONNECT()` with a corresponding server. After receiving
    setup/configuration information from the `TunServer` in the `SETUP` state, a
    `TunClient` transitions to the `CONFIG` state. By default `CONFIG` will just
    call `close()` and stop executing. Overload this state in subclasses of
    `TunClient` to specify non-trivial execution.

    After completing its execution, subclasses of `TunClient` should call
    `close()` to properly shut down the client's connection to the `TunServer`.
    """
    def __init__(self, name="tun/tap client", options=None, \
                 initstate=None, verbose=None, **kwargs):
        if options is None: options = tunclient_defaults
        self.__clientretry = 0
        self.__options = options
        self.__sock = None
        initstate = self.CONNECT
        FSM.__init__(self, name=name, initstate=initstate, \
                     verbose=options.verbose, **kwargs)

    # Properties
    options = property(lambda self: self.__options)
    sock = property(lambda self: self.__sock)

    def close(self):
        """Close socket connecting to `TunServer`."""
        self.halt()
        if self.sock: return self.sock.close()

    def CONNECT(self, fsm):
        host, port = TUN.SERVHOST, TUN.SERVPORT
        family, stype = socket.AF_INET, socket.SOCK_STREAM
        self.__sock = socket.socket(family, stype)
        while 1:
            try:
                self.sock.connect( (host, port) )
                break
            except socket.error:
                self.__clientretry += 1
                if self.__clientretry>TUN.MAXRETRY: return
                time.sleep(TUN.RECONNECTPAUSE)
        return fsm.goto(self.SETUP)

    def SETUP(self, fsm):
        """SETUP state; read setup/config information from `TunServer."""
        try:
            s = struct.pack('I', 0)
            data = self.sock.recv(len(s))
            if not data: return self.close()
            (dlen,) = struct.unpack('I', data)
            data = ""
            while len(data)<dlen:
                d = self.sock.recv(dlen-len(data) )
                if not d: return self.close()
                data += d
            ctrl = eval(data)
            assert (ctrl, dict)
            self.configure(**ctrl)
            return fsm.goto(self.CONFIG)
        except Exception:
            return self.close()

    def configure(self, **kwargs):
        """
        Process control information received from `TunServer`.

        :param kwargs: Dictionary containing setup information received.

        By default, this method does nothing; overload as needed.
        """
        pass

    def CONFIG(self, fsm):
        """
        CONFIG state; **overload this** to create a custom configuration.

        By default this state will just shut down the socket `sock` by calling
        `close()`.
        """
        self.close()
        return None

class tunclient_defaults(object):
    """Default options used for `TunClient`."""
    verbose = None

    @classmethod
    def add_options(cls, parser):
        """Add options to parser."""
        if not parser.has_option("--verbose"):
            parser.add_option("-v", "--verbose", type="string", \
                    default=cls.verbose, \
                    help="set verbose level of output [default=%default]")
        return parser

