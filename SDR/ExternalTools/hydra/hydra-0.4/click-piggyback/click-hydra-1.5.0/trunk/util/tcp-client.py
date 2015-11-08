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

import optparse
import socket
import sys

bufsize = 4096

def grab_data_from_server(host, port):
    sys.stderr.write("Grabbing data from %s:%d\n" % (host, port) )
    total = 0
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((host, port))
        while True:
            data = sock.recv(bufsize)
            if not len(data):
                break

            total = total + len(data)
            
        sock.close()
    except socket.error, (client, error):
        sys.stderr.write("Error: %s\n" % error)


def main():
    parser = optparse.OptionParser()
    parser.add_option("-p", "--dest-port", type="int", default=40000,
                      dest="port", help="destination port to connect to "
                                        "(default=%default)")
    parser.add_option("-d", "--destination", type="string",
                      default="127.0.0.1", dest="host",
                      help="name or ip of the remote host "
                           "(default=%default)")
    (options, args) = parser.parse_args()

    if len(args) > 1:
        parser.print_help()
        sys.exit(1)

    grab_data_from_server(options.host, options.port)

if __name__ == '__main__':
    main()
