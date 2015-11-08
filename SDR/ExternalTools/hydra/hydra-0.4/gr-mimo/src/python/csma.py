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
Carrier sense MAC protocol; contains `CSMA` class.

Carrier sense multiple access with collision avoidance (CSMA/CA) protocol.
Utilizes MAC data logger `MACDB` to maintain side information that is used by
the PHY.

:author: Ketan Mandke <mandke@ece.utexas.edu>

:var CSMA_VERBOSE:
    Constant enumeration to control verbose thresholds in this file.
  
    Setting the verbose level of a `CSMA` above this threshold will enable the
    corresponding output in this file.
"""

from gnuradio import gr
from threading import Event

from hydra.PyHydra import TunClient, tunclient_defaults
from hydra.PyHydra import tunconfig as TUN
from hydra.PyHydra import FSM, ParamEvent 
from hydra import mpif as MPIF
from hydra import mimo

from mimo_mpif import MimoMPIF

from dot11mac import Dot11Data, Dot11Ack
from dot11mac import DOT11_FC_TYPE_DATA, DOT11_FC_SUBTYPE_DATA
from dot11mac import DOT11_FC_TYPE_CONTROL,  DOT11_FC_SUBTYPE_ACK

from mac_db import MACDB

import struct
import numpy
import time

CSMA_VERBOSE = 4

class CSMA(TunClient):
    """
    Carrier sense multiple access protocol.

    Performs a simple backoff algorithm and Stop-and-Wait ARQ.

    :IVariables:
     * `datatosend`: Maintains packet that is currently being transmitted by
       CSMA; `None` if no packet is being sent.

     * `retrycount`: Current retry count.

     * `macdb`: MACDB used to log "meta" information related to potential
       destinations.

     * `__mpif`: MAC/PHY interface used to pass/recv data and control to/from
       the PHY side of the interface, which is implemented over a UDP socket.

     * `__addr`: MAC address (Ethernet frame format). This value is set in
       `configure()` using the value passed from `TunServer`.

    :CVariables:
     * `mpif`: Property to access `__mpif`.

     * `addr`: Property to access `__addr`.
    """
    CWMIN = 4
    CWMAX = 64
    MAXRETRY = 5
    def __init__(self, name="csma-mac", options=None, **kwargs):
        """Constructor; all arguments passed to `TunClient` constructor; see
        `csma_defaults` for more about `options`."""
        if options is None: options = csma_defaults
        self.__mtu  = options.mtu
        # call TunClient constructor
        TunClient.__init__(self, name=name, options=options, **kwargs)
        # set up public members
        self.datatosend = None
        self.retrycount = 0
        self.cslot = None
        self.macdb = MACDB(timeout=None)
        # set up remaining private members
        self.__mpif = mimo.mac(options.phy_addr, options.mac_port)
        self.__addr = None
        self.__rxdata = ParamEvent()    # got RXDATA from PHY
        self.__csbusy = ParamEvent()    # got CSBUSY from PHY
        self.__txdone = ParamEvent()    # got TXDONE from PHY
        self.__txdata = ParamEvent()    # New data to available to transmit
        self.__toidle = Event()         # Entered IDLE state
        self.__mctrl = None     # monitor ctrlQ of MPIF
        self.__mrecv = None     # monitor outputQ of MPIF
        self.__msend = None     # monitor send data from tun/tap interface

    # Properties
    mpif  = property(lambda self: self.__mpif)
    addr  = property(lambda self: self.__addr)
    mtu   = property(lambda self: self.__mtu)
    # Timing properties
    slottime = property(lambda self: self.options.slottime)
    sifs     = property(lambda self: self.options.sifs)
    difs     = property(lambda self: self.options.difs)
    ackduration = property(lambda self: self.options.ackduration)
    # Events
    rxdata = property(lambda self: self.__rxdata)
    csbusy = property(lambda self: self.__csbusy)
    txdone = property(lambda self: self.__txdone)
    txdata = property(lambda self: self.__txdata)
    toidle = property(lambda self: self.__toidle)
    # Other properties
    mctrl = property(lambda self: self.__mctrl)
    mrecv = property(lambda self: self.__mrecv)
    msend = property(lambda self: self.__msend)
    broadcastaddr = property(lambda self: self.get_broadcast_address() )

    def close(self):
        """Overload to also shutdown MPIF."""
        self.mpif.shutdown()
        return TunClient.close(self)

    def configure(self, mac=None, mtu=None, **kwargs):
        """Process control information received from `TunServer`."""
        if mac: self.__addr = mac
        if mtu: self.__mtu  = mtu

    def CONFIG(self, fsm):
        """CONFIG state; create `FSM` to monitor MPIF and tun/tap interface and
        transition to `IDLE()`."""
        # create FSMs to monitor MPIF and tun/tap interface
        self.__mrecv = FSM(name=self.getName()+".mrecv", verbose=self.verbose, \
                           initstate=self.MONRECV)
        self.__mctrl = FSM(name=self.getName()+".mctrl", verbose=self.verbose, \
                           initstate=self.MONCTRL)
        self.__msend = FSM(name=self.getName()+".msend", verbose=self.verbose, \
                           initstate=self.MONSEND)
        for x in [self.mpif, self.mctrl, self.mrecv, self.msend]: x.start()
        return fsm.goto(self.IDLE)

    ##  HELPER FUNCTIONS

    def datamsg(self, payload="", **kwargs):
        """Create `Dot11Data` message from `payload`; no CRC included."""
        d = Dot11Data(**kwargs) / payload
        #d.length = len(d)
        d.SC = len(d)
        return d

    def ackmsg(self, payload="", **kwargs):
        """Create `Dot11Ack` message with a `payload`; no CRC included."""
        a = Dot11Ack(**kwargs)/ payload
        a.length = len(a)
        return a

    def calc_crc32(self, s):
        """Calculate CRC-32 of string; returned as a signed-integer."""
        return mimo.mac_monitor.calc_crc32(str(s))

    def check_crc32(self, s):
        """Check CRC-32 of a string; assuming CRC is last 32 bits of string."""
        if len(s)<4: return False
        (rcrc,) = struct.unpack('I', s[-4:])
        acrc = self.calc_crc32(s[:-4])
        return (rcrc == acrc)

    def get_broadcast_address(self):
        """Return colon-separated, hexadecimal Ethernet broadcast address as a
        readable string."""
        maclen = 6
        if self.addr:
            maclen = len(TUN.eth_aton(self.addr))
        s = "FF:"*maclen
        return s[:-1]

    def isbroadcast(self, d):
        """Check if packet is a broadcast packet; d must be a valid packet."""
        return self.isforme(d, self.broadcastaddr)

    def isforme(self, d, other=None):
        """Check if packet's destination address matches `addr`; d must be a
        valid packet; optional 'other' address can be used instead of `addr`."""
        if other is None: other = self.addr
        return (d.addr1.lower() == other.lower() )

    def isack(self, d):
        """Check if packet is an acknowledgement packet; d must be a valid
        packet."""
        isack = (d.type == DOT11_FC_TYPE_CONTROL)
        isack = isack and (d.subtype == DOT11_FC_SUBTYPE_ACK)
        return isack

    def isdata(self, d):
        """Check if packet is a data packet; d must be a valid packet."""
        isdata = (d.type == DOT11_FC_TYPE_DATA)
        isdata = isdata and (d.subtype == DOT11_FC_SUBTYPE_DATA)
        return isdata

    def retry(self, count=None):
        """Update retry count and timing parameters."""
        if count is None: self.retrycount +=1
        else:             self.retrycount = count
        cwsize = CSMA.CWMIN * (2**self.retrycount)
        self.cslot = numpy.random.randint(cwsize)
        if self.verbose>CSMA_VERBOSE:
            self.debug("retry (%s/%s)"%(self.retrycount,CSMA.MAXRETRY))

    ##  MAIN STATES

    def IDLE(self, fsm):
        """IDLE state; wait on `csbusy`, `txdata`, and `rxdata`."""
        self.datatosend = None
        self.retrycount = 0
        self.cslot = None
        self.toidle.set()
        ns = self.IDLE
        # wait on events
        if fsm.waitonevents(self.txdata, self.csbusy, self.rxdata):
            if self.txdata.isSet():
                s = self.txdata.param
                # assume frame is an Ethernet frame
                dst = TUN.eth_ntoa(s[0:6])
                src = TUN.eth_ntoa(s[6:12])
                #src = self.addr    # use local address for source
                (ethertype, ) = struct.unpack(">H", s[12:14])
                fsm.debug("dst = %s"%(dst) )
                fsm.debug("src = %s"%(src) )
                fsm.debug("ethertype = %04X"%(ethertype) )
                fsm.datatosend = self.datamsg(payload=s, addr1=dst, addr2=src)
                # initialize backoff/retry parameters
                self.retry(count=0)
                self.txdata.clear()
                ns = self.BACKOFF
            elif self.rxdata.isSet():
                ns = self.RESP
            elif self.csbusy.isSet():
                ns = self.RECV
        self.toidle.clear()
        return fsm.goto(ns)

    def BACKOFF(self, fsm):
        """BACKOFF state; perform carrier sense."""
        if self.retrycount>CSMA.MAXRETRY:
            # drop packet and return to IDLE
            return fsm.goto(self.IDLE)
        else:
            botime  = self.difs + self.cslot * self.slottime
            bostart = time.time()
            ns = self.BACKOFF
            if fsm.waitonevents(self.csbusy, self.rxdata, timeout=botime):
                # event occurred
                if self.rxdata.isSet():
                    ns = self.RESP
                elif self.csbusy.isSet():
                    ns = self.RECV
                # update backoff slot
                bodiff = time.time() - bostart
                if bodiff>self.difs:
                    cslotdone = numpy.ceil((bodiff-self.difs)/self.slottime)
                    self.cslot = max(self.cslot-cslotdone, 0)
            else:
                # backoff timer expired
                ns = self.TXDATA
            # proceed to next state
            return fsm.goto(ns)

    def TXDATA(self, fsm):
        """TXDATA state; transmit data and ignore receive control messages."""
        if self.retrycount>0: self.datatosend.set_fcfield(retry=1)
        s = str(self.datatosend)
        # create crc-32
        crc = struct.pack('I', self.calc_crc32(s) )
        data = s+crc
        base = ""
        # Get meta data from MAC database
        dest = self.datatosend.addr1  # destination is addr1
        meta = self.macdb[dest]
        if meta is None: meta = ""
        else:
            # FIXME: need to back off rate based on retry count.
            #        This scheme is not necessarily the best backoff scheme.
            #        Might want to use something that takes spatial dimension
            #        into account. Need to work on this.
            r, dcond = struct.unpack("If", meta[0:8])
            r = max(r-self.retrycount, 0)
            self.debug("len(meta-data) = %d"%(len(meta)) )
            self.debug("using meta-data (%d,%.3f)"%(r,dcond) )
            meta = struct.pack("If", r, dcond) + meta[8:]
        rate = 0
        txpower = 8000**2.0
        # send data to MPIF and wait for txdone
        self.txdone.clear()
        self.mpif.send(data, base, meta, rate, txpower)
        self.txdone.wait()
        ns = None
        if self.txdone.isSet():
            # correct operation - ignore receive control messages
            self.rxdata.clear(), self.csbusy.clear()
            ns = self.RXACK
            if self.isbroadcast(self.datatosend): ns = self.IDLE
        # proceed
        return fsm.goto(ns)

    def RXACK(self, fsm):
        """RXACK state; wait for acknowledgement."""
        acktimeout = 2*(self.sifs + self.ackduration + self.slottime)
        ## FIXME: This is a hack to incorporate tx/rx duration into timeout
        slewrate = 0.67e-3/(100**2)  # 0.67 msec/ (100 bytes)^2
        trxtime = slewrate * ((len(str(self.datatosend) ))**2)
        acktimeout += trxtime

        ns = self.BACKOFF
        retry = True
        if fsm.waitonevents(self.rxdata, timeout=acktimeout):
            mrx = self.rxdata.param # get rxdata parameters
            d = Dot11Data(str(mrx['data']))
            ns = self.RESP
            if self.isack(d) and self.isforme(d):
                # ack is for me!
                self.rxdata.clear(), self.csbusy.clear()
                ns = self.IDLE
                retry = False
                # log meta-data embedded in base payload of ACK
                if self.check_crc32(str(mrx['data']) ):
                    base = mrx['base']
                    # assume ack sender is dest of data
                    sender = self.datatosend.addr1
                    r, dcond = struct.unpack("If", base[0:8])
                    self.debug("len(meta-data) = %d"%(len(base)) )
                    self.debug("logging meta-data (%d,%.3f)"%(r,dcond) )
                    self.macdb[sender] = base
        # otherwise timeout has occurred
        if retry: self.retry()
        return fsm.goto(ns)

    def RECV(self, fsm):
        """RECV state; assume `csbusy` happened, wait for `rxdata`."""
        self.csbusy.clear()
        if not self.rxdata.isSet():
            self.rxdata.wait()
        return fsm.goto(self.RESP)

    def RESP(self, fsm):
        """RESP state; respond to received message; assume 'rxdata' happened."""
        mrx = self.rxdata.param
        self.rxdata.clear(), self.csbusy.clear()
        meta = mrx['meta']
        data = mrx['data']
        # check crc - assume crc is last 32 bits of data
        crcok = self.check_crc32(str(data))
        if not crcok: return fsm.goto(self.RESUME)
        # is data for me?
        d = Dot11Data(data)
        ucast = self.isforme(d)
        bcast = self.isbroadcast(d)
        ns = (self.IDLE,)
        if self.isdata(d) and (ucast or bcast):
            # forward data to upstream using sock
            dummy = Dot11Data()
            hlen = len(dummy)
            s = data[hlen:-4]
            self.sock.send(s)
            fsm.debug("wrote %d bytes to socket"%(len(s) ) )
            dst = TUN.eth_ntoa(s[0:6])
            src = TUN.eth_ntoa(s[6:12])
            (ethertype, ) = struct.unpack(">H", s[12:14])
            fsm.debug("dst = %s"%(dst) )
            fsm.debug("src = %s"%(src) )
            fsm.debug("ethertype = %04X"%(ethertype) )
            if ucast: ns = (self.TXACK, d, meta)
            if crcok:
                # FIXME: Need to implement this if using reciprocity!
                # Log meta data if using reciprocity!!
                sender = d.addr2
        return fsm.goto(*ns)

    def TXACK(self, fsm, d, meta=""):
        """TXACK; transmit acknowledgement; assume `d` is a valid packet."""
        src = d.addr2
        ack = self.ackmsg(addr1=d.addr2)
        crc = struct.pack('I', self.calc_crc32(ack) )
        s = str(ack)+crc
        # set timer and send ack
        ns = None
        if not self.waitonevents(timeout=self.sifs):
            base = meta     # send meta data as base in ACK
            rate = 0
            txpower = 8000**2.0
            # send to MPIF and wait for txdone
            self.txdone.clear()
            self.mpif.send(s, base, "", rate, txpower)
            self.txdone.wait()
            if self.txdone.isSet():
                # correct operation - ignore receive control messages
                self.rxdata.clear(), self.csbusy.clear()
                ns = self.RESUME
        # proceed to next state
        return fsm.goto(ns)

    def RESUME(self, fsm):
        """RESUME state; resume sending data if needed."""
        ns = self.IDLE
        if self.datatosend: ns = self.BACKOFF
        return fsm.goto(ns)

    ##  MONITOR STATES

    def MONCTRL(self, fsm):
        """MONCTRL state; monitor MPIF control queue."""
        m = self.mpif.ctrlQ().delete_head()
        if m.type() == MPIF.CSBUSY:
            fsm.debug("CSBUSY")
            self.csbusy.set()
        elif m.type() == MPIF.TXDONE:
            fsm.debug("TXDONE")
            self.txdone.set()
        else:
            raise RuntimeError, \
                  "[CSMA]: Got unexpected message from MPIF control queue!"
        return fsm.goto(self.MONCTRL)

    def MONRECV(self, fsm):
        """MONRECV state; monitor MPIF output queue."""
        m = self.mpif.outputQ().delete_head()
        if m.type() == MPIF.RXDATA:
            # decode MPIF message into dictionary
            s = m.to_string()
            vlen = len(struct.pack('iiiIf', *([0]*5)) )
            if len(s)>vlen:
                meta, base, data = "", "", ""
                (mlen,blen,dlen,r,snr) = \
                        struct.unpack('iiiIf', s[0:vlen])
                if len(s)+1>vlen+mlen:
                    meta = s[vlen           : vlen+mlen]
                if len(s)+1>vlen+mlen+blen:
                    base = s[vlen+mlen      : vlen+mlen+blen]
                if len(s)+1>vlen+mlen+blen+dlen:
                    data = s[vlen+mlen+blen : vlen+mlen+blen+dlen]
                mrx = {'type':MPIF.RXDATA, 'metalen':mlen, 'baselen':blen, \
                       'varlen':vlen, 'rate':r, 'snr': snr, \
                       'meta':meta, 'base':base, 'data':data}
                fsm.debug("RXDATA")
                self.rxdata.set(mrx)
        else:
            raise RuntimeError, \
                  "[CSMA]: Got unexpected message from MPIF output queue!"
        return fsm.goto(self.MONRECV)

    def MONSEND(self, fsm):
        """MONSEND state; monitor tun/tap interface for new data; set `txdata`
        everytime `toidle` is signalled."""
        data = self.sock.recv(self.mtu)
        if not data: return self.close()
        # wait for transition to IDLE before signalling new data message
        self.toidle.wait()
        fsm.debug("TXDATA")
        self.txdata.set(data)
        return fsm.goto(self.MONSEND)

class csma_defaults(tunclient_defaults):
    """Default options used for `CSMA`."""
    phy_addr = "0.0.0.0"
    mtu      = 2236
    sifs     = 10e-3
    difs     = 30e-3
    slottime = 5e-3
    ackduration = 100e-3

    @classmethod
    def add_options(cls, parser):
        """Add options to parser."""
        tunclient_defaults.add_options(parser)
        if not parser.has_option("--phy-addr"):
            parser.add_option("", "--phy-addr", type="string", \
                    default=cls.phy_addr, \
                    help="address for PHY side of MPIF [default=%default]")
        if not parser.has_option("--mtu"):
            parser.add_option("", "--mtu", type="int", default=cls.mtu, \
                    help="Maximum transmission unit that can be received from tun/tap interface [default=%default]")
        if not parser.has_option("--sifs"):
            parser.add_option("", "--sifs", type="float", default=cls.sifs, \
                    help="Short interframe spacing [default=%default]")
        if not parser.has_option("--difs"):
            parser.add_option("", "--difs", type="float", default=cls.difs, \
                    help="Distributed interframe spacing [default=%default]")
        if not parser.has_option("--slottime"):
            parser.add_option("", "--slottime", type="float",
                    default=cls.slottime, \
                    help="Contention slot duration [default=%default]")
        if not parser.has_option("--ackduration"):
            parser.add_option("", "--ackduration", type="float",
                    default=cls.ackduration, \
                    help="Duration of acknowledgement (used to calculate timeout) [default=%default]")
        MimoMPIF.add_parser_options(parser)
        return parser
