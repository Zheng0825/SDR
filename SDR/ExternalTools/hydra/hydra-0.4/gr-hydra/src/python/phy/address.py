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
from gnuradio.wxgui import form
from gnuradio import eng_notation
from optparse import OptionParser

from hydra.PyHydra import FuncThread, Callable
from hydra import tcpserver, tcpclient
import hydra
import sys
import struct
import time
import wx

# used for getting IP address
import socket
import fcntl
import struct

class default_address_server_setup:
    d_options = {'verbose':0, 'debug':1, 'disable_gui':False, \
                 'ifname':None, \
                 'addrserver_max_retries': '10', \
                 'addrserver_addr':'0.0.0.0', \
                 'addrserver_port':50999}

    def get_options():
        dopt = default_address_server_setup()
        for s in default_address_server_setup.d_options:
            if s not in dir(dopt):
                setattr(dopt, s, default_address_server_setup.d_options[s] )
        return dopt
    
    def check_options(options):
        for s in default_address_server_setup.d_options:
            if s not in dir(options):
                setattr(options, s, default_address_server_setup.d_options[s] )

    get_options = Callable (get_options)
    check_options = Callable (check_options)

##  Contains convenience methods for translating address data into strings and
#   tuples and for generating packets exchanged by address server and client.
class Address(object):
    SHUTDOWN = -1   # Shutdown message -> closes ports
    REGISTER = 0    # Register an address
    REQUEST = 1     # Request a list of all addresses
    REPLY = 2       # Reply from server to request from client
    UNREGISTER = 3  # Unregister an address from server
    ERROR = 4       # Indicates an error has occurred with the request
    def __init__(self, **kwargs):
        pass
    ##  Extract IP address (4 byte vector) and port (i.e. 2 byte short) from 6 byte string
    def string_to_data(data=""):
        addr, port = "", None
        if (len(data)<(4+gr.sizeof_short) ):
            sys.stderr.write("Invalid data length!\n")
            raise RuntimeError
        else:
            for k in range(4):
                addr += "%d"%(ord(data[k]) )
                if (k+1<4): addr +="."
            (port,) = struct.unpack('<H', data[4:])
        return (addr, port)
    string_to_data = Callable(string_to_data)

    ##  Convert IP address and port to a 6 byte data string
    def data_to_string(addr="0.0.0.0", port=60000):
        r = ""
        a = str(addr).split('.')
        if (len(a)<4):
            sys.stderr.write("Invalid IP address!\n")
            raise RuntimeError
        else:
            v = [int(x) for x in a]
            r += struct.pack('<'+'B'*len(v), *v)    # 4 bytes
            r += struct.pack('<H', port)            # 2 bytes
        return r
    data_to_string = Callable(data_to_string)

    ##  Create Register Packet
    #       4  |   4  |  4
    #   | TYPE | ADDR | PORT |
    def create_register(addr, port):
        s = ""
        s += struct.pack('<i', Address.REGISTER)
        s += Address.data_to_string(addr, port)
        return s
    create_register = Callable(create_register)

    ##  Create Unregister Packet
    #       4  |   4  |  4
    #   | TYPE | ADDR | PORT |
    def create_unregister(addr, port):
        s = ""
        s += struct.pack('<i', Address.UNREGISTER)
        s += Address.data_to_string(addr, port)
        return s
    create_unregister = Callable(create_unregister)

    ##  Create Request Packet
    #      4 
    #   | TYPE |
    def create_request():
        s = ""
        s += struct.pack('<i', Address.REQUEST)
        return s
    create_request = Callable(create_request)

    ##  Create Reply Packet
    #      4   |   4   |   4   |   2   |   4   |   2   |
    #   | TYPE | NADDR | ADDR1 | PORT1 | ADDR2 | PORT2 | ...
    def create_reply(*args):
        assert (len(args)%2 == 0 ), "Invalid number of arguments!"
        naddr = len(args)/2
        s = ""
        s += struct.pack('<ii', Address.REPLY, naddr)
        for k in range(naddr):
            s += Address.data_to_string(args[2*k], args[2*k+1])
        return s
    create_reply = Callable(create_reply)

    ##  Create Error Packet
    #      4   |    4   |       LENGTH
    #   | TYPE | LENGTH |  ... PAYLOAD ...  |
    def create_error(payload):
        p = str(payload)
        plen = len(p)
        s = ""
        s += struct.pack('<ii', Address.ERROR, plen)
        s += p
        return s
    create_error = Callable(create_error)

    ## Create Shutdown Packet
    def create_shutdown():
        s = ""
        s += struct.pack('<i', Address.SHUTDOWN)
        return s
    create_shutdown = Callable(create_shutdown)

    ## Get IP Address from interface name
    #
    #   If no name is specifed, it will try to find the first ethernet device named
    #   'ethX' where X is the interface number. 'ntoa' specifies if the address
    #   should be translated to a readable string.
    def get_ip_address(ifname=None, ntoa=True):
        MAX_RETRIES = 4
        ncount = 0
        ename = ifname
        rval = ""
        while ncount<MAX_RETRIES:
            try:
                if ifname is None: ename = "eth%d"%(ncount)
                s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                rval = fcntl.ioctl(s.fileno(), 0x8915, \
                        struct.pack('256s', ename[:15]) )[20:24]
                if ntoa: rval = socket.inet_ntoa(rval)
                ncount = MAX_RETRIES
            except Exception, e:
                if (ncount+1) == MAX_RETRIES:
                    raise e, "[ADDRESS]: Cannot find interface name or IP address!"
                else: ncount += 1
        return rval
    get_ip_address = Callable(get_ip_address)


class AddrServer(object):
    def __init__(self, options):
        # check for default options before continuing with constructor
        default_address_server_setup.check_options(options)
        self.verbose = options.verbose
        self.debug = options.debug

        # instantiate blocks
        self.addr, self.port = options.addrserver_addr, options.addrserver_port
        self.server = tcpserver(self.addr, self.port)
        self.isalive = False
        self.buffer = []    # buffer of (addr, port) registered with server

        # set options
        self.disable_gui = options.disable_gui

    def __del__(self, *args):
        self.stop()
        for k in range(3):
            sys.stderr.write('.')
            time.sleep(1)
        sys.stderr.write('\n')

    def start(self):
        f = FuncThread(self.__run__, "run server")
        f.start()
        return f

    def stop(self):
        if hasattr(self.server, 'close'): self.server.close()
        self.isalive = False

    ## Blocking call
    def run(self):
        self.start()
        try:
            print "Press Ctrl-C to quit\n"
            while 1: time.sleep(3)
        except KeyboardInterrupt:
            self.stop()
        print "exiting Address Server ..."
        self.stop()

    def __run__(self):
        cid = 1
        self.isalive = True
        while (self.isalive):
            cid = self.server.accept()
            if (cid > 0):
                if self.debug: print "[ADDRSERVER]: Accepts new connection, id =", cid
                g = FuncThread(self.manage_connection, "manage connection", cid)
                g.start()
            else:
                sys.stderr.write("[ADDRSERVER]: TCP Server shutdown initiated!\n")
                self.isalive = False
                return self.server.close()
        self.server.close()
        self.isalive = False

    def manage_connection(self, cid):
        conn = self.server.connection(cid)
        close_connection = False
        while (not close_connection):
            s = conn.recv(4)
            if (len(s) < 4):
                close_connection = True
            else:
                (type,) = struct.unpack('<i', s[0:4])
                if   type == Address.REGISTER:
                    sys.stderr.write("[ADDRSERVER]: REGISTER received\n")
                    s = conn.recv(4+gr.sizeof_short)
                    if (len(s) < 4+gr.sizeof_short):
                        close_connection = True
                    else:
                        (addr, port) = Address.string_to_data(s)
                        if (addr,port) not in self.buffer:
                            self.buffer.append( (addr,port) )
                        if self.debug:
                            print "[ADDRSERVER]: Registered", (addr, port)
                elif type == Address.UNREGISTER:
                    sys.stderr.write("[ADDRSERVER]: UNREGISTER received\n")
                    s = conn.recv(4+gr.sizeof_short)
                    if (len(s) < 4+gr.sizeof_short):
                        close_connection = True
                    else:
                        (addr, port) = Address.string_to_data(s)
                        if (addr,port) in self.buffer:
                            self.buffer.remove( (addr,port) )
                        if self.debug:
                            print "[ADDRSERVER]: Unregistered", (addr, port)
                elif type == Address.REQUEST:
                    sys.stderr.write("[ADDRSERVER]: REQUEST received\n")
                    dbuf = ()
                    for x in self.buffer: dbuf += x
                    s = Address.create_reply(*dbuf)
                    sys.stderr.write("[ADDRSERVER]: REPLY sent\n")
                    conn.send(s)
                else:
                    # treat message like a shutdown command
                    self.stop()
                    close_connection = True
        self.server.shutdown(cid)  # close connection using server command

    """ Add parser options to an OptionParser """
    def add_parser_options (parser):
        if not parser.has_option("-v"):
            parser.add_option ("-v", "--verbose", type="int", \
                    default=default_address_server_setup.d_options['verbose'], \
                    help="set verbose level of output [default=%default]")
        if not parser.has_option("--debug"):
            parser.add_option ("", "--debug", type="int", \
                    default=default_address_server_setup.d_options['debug'], \
                    help="set debug level of system [default=%default]")
        if not parser.has_option("--ifname"):
            parser.add_option("", "--ifname", type="string", \
                    default=default_address_server_setup.d_options['ifname'],
                    help="specify ethernet interface\'s name (e.g. eth0) [default=%default]")
        if not parser.has_option("--addrserver-max-retries"):
            parser.add_option ("", "--addrserver-max-retries", type="int", \
                    default=default_address_server_setup.d_options['addrserver_max_retries'], \
                    help="specify max number of retries for client connecting to Address Server [default=%default]")
        if not parser.has_option("--addrserver-addr"):
            parser.add_option("", "--addrserver-addr", type="string", \
                    default=default_address_server_setup.d_options['addrserver_addr'],
                    help="set address for Address Server [default=%default]")
        if not parser.has_option("--addrserver-port"):
            parser.add_option ("", "--addrserver-port", type="int", \
                    default=default_address_server_setup.d_options['addrserver_port'], \
                    help="specify port used by Address Server [default=%default]")
        if not parser.has_option("--disable-gui"):
            parser.add_option("", "--disable-gui", action="store_true", \
                    default=default_address_server_setup.d_options['disable_gui'],
                    help="disable GUI operation [default=%default]")

    add_parser_options = Callable (add_parser_options)

class AddrClient(object):
    def __init__(self, options):
        # check for default options before continuing with constructor
        default_address_server_setup.check_options(options)
        self.verbose = options.verbose
        self.debug = options.debug

        ## instantiate blocks
        self.addrgui = None
        self.client = None
        self.dest = None
        self.local = None
        self.destQ = gr.msg_queue()

        ## set options
        self.disable_gui = options.disable_gui
        self.ifname = options.ifname
        self.addr, self.port = options.addrserver_addr, options.addrserver_port
        self.max_retry = options.addrserver_max_retries

    def __del__(self):
        self.close()

    def close(self):
        if self.client:
            self.unregister()
            self.client.close()
        self.put_destaddr()

    def startwx(self):
        if hydra.PyHydra.gui.window.wxapp is not None:
            if not hydra.PyHydra.gui.window.wxapp.IsMainLoopRunning():
                hydra.PyHydra.gui.window.wxapp.MainLoop()
            else:
                while (hydra.PyHydra.gui.window.wxapp.IsMainLoopRunning() ): time.sleep(2)

    # Connect to address server
    def connect(self):
        # create tcp client and connect to address server
        self.client = tcpclient(self.addr, self.port)
        retrycount = 0
        while ((self.client.connect()<0) and (retrycount<self.max_retry)):
            retrycount += 1
            time.sleep(1)
        if retrycount+1>self.max_retry:
            sys.stderr.write("[ADDRCLIENT]: Cannot connect to AddrServer (%s,%d)\n"%(str(self.addr), self.port) )
            self.close()
            return -1
        return 0

    def register(self, addr, port):
        self.local = (addr, port)
        if self.client:
            s = Address.create_register(addr, port)
            n = self.client.send(s)
            if (n<0):
                sys.stderr.write("[ADDRCLIENT]: TCP connection to (%s,%d) is not alive\n"%(str(self.addr), self.port) )
                self.close()
                return -1
            return 0
        return -1

    def unregister(self, addr=None, port=None):
        if self.local is not None:
            if addr is None: addr = self.local[0]
            if port is None: port = self.local[1]
        if self.client and (addr is not None) and (port is not None):
            s = Address.create_unregister(addr, port)
            n = self.client.send(s)
            self.local = None
            if (n<0):
                sys.stderr.write("[ADDRCLIENT]: TCP connection to (%s,%d) is not alive\n"%(str(self.addr), self.port) )
                self.close()
                return -1
            return 0
        return -1

    def get_destaddr(self):
        m = self.destQ.delete_head()
        return self.dest

    def put_destaddr(self, addr=None):
        if addr: self.dest = addr
        m = gr.message()
        self.destQ.insert_tail(m)

    def get_dest_addr(self):
        if not self.disable_gui:
            if hydra.PyHydra.gui.window.wxapp is None: hydra.PyHydra.gui.window.wxapp = wx.App(0)
            self.addrgui = hydra.PyHydra.AddrGUI(client=self)
            if self.addrgui: self.addrgui.Show(True)
            f = FuncThread(self.startwx)
            f.setDaemon(1)
            f.start()
            self.dest = self.get_destaddr()
            if not isinstance(self.dest, tuple):
                sys.stderr.write("[ADDRCLIENT]: Error occurred during user input!\n")
                print "self.dest =", self.dest
                self.close()
                return -1
        else:
            while (self.dest is None):
                addrlist = self.server_request()
                print ""
                print "========================"
                print "Address List :", self.local
                print "========================"
                print "[R] - refresh list from server"
                for k in range(len(addrlist) ):
                    print "[%d] -"%(k), addrlist[k]
                print ""
                c = raw_input(">>> ")
                if c.lower().strip() == "r":
                    continue
                else:
                    try:    k = int(c.lower() )
                    except: k = None
                    if (k not in range(len(addrlist) ) ):
                        sys.stderr.write("Invalid input!\n")
                        continue
                    else:
                        self.dest = addrlist[k]
        return self.dest 

    def server_request(self):
        error = True
        addr_reply = []
        if self.client:
            error = False
            s = Address.create_request()
            self.client.send(s)         # send request
            s = self.client.recv(4)     # wait for reply
            if (len(s)<4):
                error = True
            else:
                (type,) = struct.unpack('<i', s[0:4])
                if type == Address.REPLY:
                    s = self.client.recv(4)
                    if (len(s) < 4):
                        error = True
                    else:
                        (naddr,) = struct.unpack('<i', s[0:4])
                        addrsize = 4+gr.sizeof_short
                        s = self.client.recv(addrsize*naddr)
                        if (len(s) < addrsize*naddr):
                            error = True
                        else:
                            for k in range(naddr):
                                addr_reply.append(Address.string_to_data(s[k*addrsize:(k+1)*addrsize]) )
                else:
                    error = True
        if error:
            sys.stderr.write("[ADDRCLIENT]: Error occurred during request to address server!\n")
            self.close()
            return []
        else:
            return addr_reply

    """ Add parser options to an OptionParser """
    def add_parser_options (parser):
        AddrServer.add_parser_options (parser)

    add_parser_options = Callable (add_parser_options)
