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
Contains constants and shared data used by `TunServer` and `TunClient`.

:author: Ketan Mandke <mandke@ece.utexas.edu>

:var VERBOSE:
    Constant enumeration to control verbose thresholds in this file.
  
    Setting the verbose level of a `TunServer` or `TunClient` below this
    threshold will cause the corresponding output in this file to be silenced.

:var SERVPORT:
    TCP port that `TunServer` listens to for connections from `TunClient`.

:var SERVHOST:
    Address that `TunServer` is listening on for `TunClient`.

:var MAXRETRY:
    Maximum number of times `TunClient` should try to connect to `TunServer`
    before giving up.

:var RECONNECTPAUSE:
    Time to wait between connection attempts between `TunClient` and
    `TunServer`.
"""

import struct

VERBOSE  = 9
SERVPORT = 54321
SERVHOST = "localhost"

MAXRETRY = 5
RECONNECTPAUSE = 5.0

def eth_aton(addr):
    """
    Translate Ethernet address from a readable, colon-separated, hexadecimal
    Ethernet address string to a 48-bit packed string.

    :param addr: Readable string of colon-separated Ethernet address.

    :returns: Packed string containing ordinal values from `addr`.

    This method is the Ethernet equivalent of `socket.inet_aton()`.
    """
    arr = addr.split(':')
    maclen = len(arr)
    mac = struct.pack('B'*maclen, \
                *([ord(eval("\"\\x%s\""%(x))) for x in arr]) )
    return mac

def eth_ntoa(addr):
    """
    Translate Ethernet address from a 48-bit packed string to a readable,
    colon-separated, hexadecimal address string.

    :param addr: 48-bit string containing address.

    :returns: Readable colon-separated hexadecimal Ethernet address string.

    This method is the Ethernet equivalent of `socket.inet_ntoa()`.
    """
    maclen = len(addr)
    s = ""
    for k in range(maclen):
        s += ":%02X"%(ord(addr[k]) )
    return s[1:]
