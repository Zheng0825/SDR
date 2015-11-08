#!/usr/bin/env python
# 
# Copyright (C) 2009  The University of Texas at Austin.
# 
# This file is part of Hydra: A wireless multihop testbed.
# 
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, subject to the conditions
# listed in the Click LICENSE file. These conditions include: you must
# preserve this copyright notice, and you cannot mention the copyright
# holders in advertising related to the Software without their permission.
# The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
# notice is a summary of the Click LICENSE file; the license in that file is
# legally binding.
# 

from optparse import OptionParser

from gnuradio.eng_option import eng_option
import gnuradio.gr.gr_threading as _threading
import random
import time
import struct
import sys
import os
import string
import socket
from string import atoi

  
class tx_udp_thread(_threading.Thread):
    def __init__(self, dest_ip, dest_port, interval, packetsize, totalsize, jitter, verbose):
      _threading.Thread.__init__(self)
      self.setDaemon(1)

      self.dest_ip = dest_ip;
      self.dest_port = dest_port;
      self.interval = interval
      self.packetsize = packetsize
      self.totalsize = totalsize
      self.jitter = jitter
      self.verbose = verbose

      self.sk = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
      self.n_packet_to_send = int(totalsize/packetsize)
      self.seqno = 0
      self.again = False

      self.start()

    def run(self):
	while 1:
          n_packet_sent = 0
          if self.again:             
            cinput = ""
            cinput = raw_input ("\nPress Enter to start send %(x)g Bytes of data (or type \'quit\'):\n" % {'x':self.totalsize})
            if string.lower(cinput) == 'quit':
              break

          self.again = True 
	  print '\nStarting Transmitter'
          time.sleep(1.0)

          while n_packet_sent < self.n_packet_to_send:

            packet = chr(self.seqno) + (self.packetsize - 1) * chr(0x39)
            self.sk.sendto(packet, (self.dest_ip, self.dest_port))     

            if self.verbose: 
              sys.stderr.write("Sent: dst=%s, %d bytes, seqno=%d\n" % (self.dest_ip, self.packetsize, self.seqno))
            else:
              sys.stderr.write('.')

            n_packet_sent += 1
            self.seqno += 1 

            if self.seqno == 256:
              self.seqno = 0

            sleep_time = self.interval
            if self.jitter == True:
              sleep_time += (random.random() - 0.5) * (self.interval/float(2))
                  
            time.sleep(sleep_time)
            

class rx_udp(object):
    def __init__(self, listen_port, mtu, verbose):
      self.listen_port = listen_port;
      self.mtu = mtu
      self.verbose = verbose

      self.sk = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
      self.sk.bind(('0.0.0.0', self.listen_port))

    def main_loop(self):
	while 1:
          (packet, source)  = self.sk.recvfrom(self.mtu)
          if self.verbose: 
            sys.stderr.write("Recv: src=%s, %d bytes, seqno=%d from %s\n" % ( source[0], len(packet), ord(packet[0]), source ) )
          else:
            sys.stderr.write('*')
            


def main():
    #print os.getpid()
    #raw_input('Attach and press enter')

    parser = OptionParser (option_class=eng_option)
    parser.add_option("-l", "--listen_port", type="int", default=40000,
	                      help="specify udp port of destination (default=%default)")
    parser.add_option("-d", "--dest_port", type="int", default=40000,
	                      help="specify udp port of destination (default=%default)")
 
    parser.add_option("-i", "--interval", type="eng_float", default=1.0,
	                       help="set pause time in seconds [default=%default]")
    parser.add_option("-s", "--packetsize", type="int", default=128,
	                      help="specify size of packet (default=%default)")
    parser.add_option("-t", "--totalsize", type="int", default=1e6,
                      help="set bytes to transmit [default=%default]")
    parser.add_option("-j","--jitter", action="store_true", default=False)
    parser.add_option("-v","--verbose", action="store_true", default=True)
    (options, args) = parser.parse_args()


    if len(args) > 1:
        parser.print_help()
        sys.exit(1)

    if options.packetsize < 1:
        options.packetsize = 1
    
    mtu = 1500
    if options.packetsize > mtu:
        options.packetsize = mtu

    if options.totalsize < options.packetsize:
      options.totalsize = options.packetsize
  
    if len(args) == 1:
      tx_thread = tx_udp_thread(args[0], options.dest_port, options.interval, options.packetsize, options.totalsize, options.jitter, options.verbose)

    rx = rx_udp(options.listen_port, mtu, options.verbose)
    rx.main_loop()
    

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
