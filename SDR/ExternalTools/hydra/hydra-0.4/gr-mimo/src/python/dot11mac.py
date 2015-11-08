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
Implements helper methods and classes for IEEE 802.11 MAC protocol; contains
`Dot11Ack` and `Dot11Data` classes.

There are two implementations of each class in this file. If Scapy 2.0 (or
better) is availabe, then this module will use the Scapy version otherwise it
implements a basic interface which mimics the features of Scapy.

:author: Ketan Mandke <mandke@ece.utexas.edu>

:var DOT11_FC_SUBTYPE_DATA:
    Constant enumeration for subtype field of an IEEE 802.11 Data packet.

:var DOT11_FC_SUBTYPE_ACK:
    Constant enumeration for subtype field of an IEEE 802.11 Ack packet.

:var DOT11_FC_TYPE_DATA:
    Constant enumeration for type field of an IEEE 802.11 Data packet.

:var DOT11_FC_TYPE_CONTROL:
    Constant enumeration for type field of an IEEE 802.11 Control packet.
"""

import struct
from copy import deepcopy

DOT11_FC_SUBTYPE_DATA = 0x00
DOT11_FC_SUBTYPE_ACK  = 0x0D
DOT11_FC_TYPE_DATA    = 2
DOT11_FC_TYPE_CONTROL = 1

_SCAPYSUPPORT=1
try:
    from scapy.all import Dot11, IntField, LEShortField
except ImportError:
    _SCAPYSUPPORT=0

if _SCAPYSUPPORT:
    class Dot11Data(Dot11):
        """
        IEEE 802.11 Data packet (**with** Scapy).
        """
        # fields description
        fields_desc = []
        @classmethod
        def _init_fields_desc(cls):
            """Initialize field_desc of Dot11Data."""
            cls.fields_desc = [Dot11.fields_desc[2],   # proto       \
                               Dot11.fields_desc[1],   # type        \
                               Dot11.fields_desc[0],   # subtype     \
                               Dot11.fields_desc[3],   # FCfield     \
                               IntField("ID", 0),      # Duration/ID \
                               Dot11.fields_desc[5],   # addr1       \
                               Dot11.fields_desc[6],   # addr2       \
                               Dot11.fields_desc[7],   # addr3       \
                               LEShortField("SC", 0) ] # length
            cls.fields_desc = [deepcopy(x) for x in cls.fields_desc]
            # override defaults
            cls.fields_desc[1].default = DOT11_FC_TYPE_DATA
            cls.fields_desc[2].default = DOT11_FC_SUBTYPE_DATA

        def _set_SC(self, v):
            self.SC = v

        # Properties
        length = property(lambda self: self.SC, \
                          lambda self,v: self._set_SC(v) )

        def set_fcfield(self, **kwargs):
            """Change frame control field using keyword parameters (where '-' is
            replaced by '_'); use keyword 'reset' to force overwrite of
            FCfield."""
            if 'reset' in kwargs:
                if kwargs['reset']: self.FCfield = 0
                del kwargs['reset']
            for key in kwargs:
                if kwargs[key]:
                    s = key
                    s.replace('_', '-')
                    fc = self.FCfield
                    self.FCfield = s
                    self.FCfield = fc + self.FCfield

        def get_fcfield(self):
            """Return dictionary containing current values in FCfield."""
            fc = int(repr(self.FCfield))
            rdict = {}
            rkeys = ["to-DS", "from-DS", "MF", "retry", "pw-mgt", "MD", "wep", "order"]
            rkeys = [s.replace('-','_') for s in rkeys]
            for k in range(len(rkeys) ):
                key = rkeys[k]
                rdict[key] = (fc>>k) & 0x01
            return rdict

    class Dot11Ack(Dot11Data):
        """
        IEEE 802.11 Ack packet (**with** Scapy).
        """
        fields_desc = []
        @classmethod
        def _init_fields_desc(cls):
            """Remove addr2 and addr3 fields."""
            cls.fields_desc = Dot11Data.fields_desc[:6]+[Dot11.fields_desc[8]]
            cls.fields_desc = [deepcopy(x) for x in cls.fields_desc]
            # override defaults
            cls.fields_desc[1].default = DOT11_FC_TYPE_CONTROL
            cls.fields_desc[2].default = DOT11_FC_SUBTYPE_ACK

    Dot11Data._init_fields_desc()
    Dot11Ack._init_fields_desc()

else:

    from hydra.PyHydra import tunconfig
    eth_ntoa = tunconfig.eth_ntoa
    eth_aton = tunconfig.eth_aton
    class Dot11Data(object):
        """
        IEEE 802.11 Data packet (**without** Scapy).
        """
        FC_TYPE_PARAMS = ["Management", "Control", "Data", "Reserved"]
        FC_PARAMS      = ["to-DS", "from-DS", "MF", "retry", "pw-mgt", "MD", "wep", "order"]
        def __init__(self, s=None, proto=None, type=DOT11_FC_TYPE_DATA, \
                           subtype=DOT11_FC_SUBTYPE_DATA, FCfield=None, \
                           addr1=None, addr2=None, addr3=None, SC=None):
            self.__proto   = 0
            self.__type    = 2      # type    = Data
            self.__subtype = 0x00   # subtype = Data
            self.__FCfield = 0
            self.__ID  = 0
            self.addr1 = eth_ntoa("\x00"*6)
            self.addr2 = eth_ntoa("\x00"*6)
            self.addr3 = eth_ntoa("\x00"*6)
            self.__SC  = 0
            self.payload = ""
            # initialize fields
            self.type = DOT11_FC_TYPE_DATA
            self.subtype = DOT11_FC_SUBTYPE_DATA
            if proto is not None:   self.proto = proto
            if type is not None:    self.type = type
            if subtype is not None: self.subtype = subtype
            if FCfield is not None: self.__FCfield = FCfield
            if addr1 is not None:   self.addr1 = addr1
            if addr2 is not None:   self.addr2 = addr2
            if addr3 is not None:   self.addr3 = addr3
            if SC is not None:      self.SC = SC
            # read in from string
            if isinstance(s, str):  self._read_from_string(s)

        def _read_from_string(self, s):
            """Read fields from a String."""
            if len(s)>0:
                b = ord(s[0])
                self.proto = ((b&0xC0)>>6)
                self.type, self.subtype = ((b&0x30)>>4), (b&0x0F)
            if len(s)>1:  self.__FCfield = ord(s[1])
            if len(s)>5:  (self.ID, ) = struct.unpack("I", s[2:6])
            if len(s)>11: self.naddr1 = s[6:12]
            if len(s)>17: self.naddr2 = s[12:18]
            if len(s)>23: self.naddr3 = s[18:24]
            if len(s)>25: (self.SC, ) = struct.unpack("H", s[24:26])
            if len(s)>26: self.payload = s[26:]

        def add_payload(self, s):
            """Append string to payload"""
            self.payload += str(s)

        def __div__(self, other):
            """Allows for use of division operator to append payload."""
            self.add_payload(other)
            return self

        def __set_proto(self, p):   self.__type = int(p)%2**2
        def __set_subtype(self, s): self.__subtype = int(s)%2**4
        def __set_ID(self, id):     self.__ID = int(id)%2**32
        def __set_SC(self, sc):     self.__SC = int(sc)%2**16
        def __set_type(self, t):
            d = Dot11Data.FC_TYPE_PARAMS
            if isinstance(t, str) and (t in d): t = d[d.index(t)]
            self.__type = int(t)%2**2

        def set_fcfield(self, **kwargs):
            """Overwrite frame control field using keyword parameters (where '-'
            is replaced by '_')."""
            self.__FCfield = 0
            for key in kwargs:
                if kwargs[key]:
                    s = key
                    s.replace('_', '-')
                    if s in Dot11Data.FC_PARAMS:
                        idx = Dot11Data.FC_PARAMS.index(s)
                        self.__FCfield |= (1<<idx)

        def get_fcfield(self):
            """Return dictionary containing current values in FCfield."""
            fc = int(repr(self.FCfield))
            rdict = {}
            rkeys = ["to-DS", "from-DS", "MF", "retry", "pw-mgt", "MD", "wep", "order"]
            rkeys = [s.replace('-','_') for s in rkeys]
            for k in range(len(rkeys) ):
                key = rkeys[k]
                rdict[key] = (fc>>k) & 0x01
            return rdict

        # Properties
        FCfield = property(lambda self: self.__FCfield)
        proto   = property(lambda self:   self.__proto, \
                           lambda self,v: self.__set_proto(v) )
        type    = property(lambda self:   self.__type, \
                           lambda self,v: self.__set_type(v) )
        subtype = property(lambda self:   self.__subtype, \
                           lambda self,v: self.__set_subtype(v) )
        ID      = property(lambda self:   self.__ID, \
                           lambda self,v: self.__set_ID(v) )
        naddr1  = property(lambda self:   eth_aton(self.addr1), \
                           lambda self,n: setattr(self,'addr1',eth_ntoa(n)) )
        naddr2  = property(lambda self:   eth_aton(self.addr2), \
                           lambda self,n: setattr(self,'addr2',eth_ntoa(n)) )
        naddr3  = property(lambda self:   eth_aton(self.addr3), \
                           lambda self,n: setattr(self,'addr3',eth_ntoa(n)) )
        SC      = property(lambda self:   self.__SC, \
                           lambda self,v: self.__set_SC(v) )
        length  = property(lambda self:   self.SC, \
                           lambda self,v: self.__set_SC(v) )

        def __str__(self):
            """Assemble a string representation for packet."""
            b = (self.proto<<6) + (self.type<<4) + (self.subtype)
            sfc = struct.pack('BB', b, self.FCfield)
            sid = struct.pack('I', self.ID)
            saddr = struct.pack('6s6s6s', self.naddr1, self.naddr2, self.addr3)
            slen  = struct.pack('H', self.length)
            return sfc+sid+saddr+slen+self.payload

        def __repr__(self):
            """Readable representation of packet."""
            stype = self.type
            d = Dot11Data.FC_TYPE_PARAMS
            if stype in range(len(d)): stype = d[stype]
            r = "<Dot11Data  proto=%d type=%s "%(self.proto, stype) + \
                          "subtype=%d "%(self.subtype) + \
                          "FCfield=%d ID=%d "%(self.FCfield, self.ID) + \
                          "addr1=%s addr2=%s "%(self.addr1, self.addr2) + \
                          "addr3=%s SC=%d "%(self.addr3, self.SC) + \
                          "| length = %d>"%(len(self) )
            return r

        def __len__(self):
            """Return length of packet after converting to a string."""
            return len(str(self) )

    class Dot11Ack(Dot11Data):
        """
        IEEE 802.11 Ack packet (**without** Scapy).
        """
        def __init__(self, s=None, type=DOT11_FC_TYPE_CONTROL, \
                           subtype=DOT11_FC_SUBTYPE_ACK, **kwargs):
            """Constructor; overload type and subtype defaults."""
            Dot11Data.__init__(self,s=s,type=type,subtype=subtype,**kwargs)

        def _read_from_string(self, s):
            """Read fields from a string."""
            Dot11Data._read_from_string(self, s)
            # reorder SC and ignore other fields after addr2
            if len(s)>13: (self.SC, ) = struct.unpack("H", s[12:14])
            if len(s)>14: self.payload = s[14:]

        def __str__(self):
            """Assemble a string representation for packet."""
            b = (self.proto<<6) + (self.type<<4) + (self.subtype)
            sfc = struct.pack('BB', b, self.FCfield)
            sid = struct.pack('I', self.ID)
            saddr = struct.pack('6s', self.naddr1)
            slen  = struct.pack('H', self.length)
            return sfc+sid+saddr+slen

        def __repr__(self):
            """Readable representation of packet."""
            stype = self.type
            d = Dot11Data.FC_TYPE_PARAMS
            if stype in d: stype = d[d.index(stype)]
            r = "<Dot11Ack   proto=%d type=%s "%(self.proto, stype) + \
                            "subtype=%d "%(self.subtype) + \
                            "FCfield=%d ID=%d "%(self.FCfield, self.ID) + \
                            "addr1=%s SC=%d "%(self.addr1, self.SC) + \
                            "| length = %d>"%(len(self) )
            return r
