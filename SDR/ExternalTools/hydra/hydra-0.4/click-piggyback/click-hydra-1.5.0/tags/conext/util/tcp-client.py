#!/usr/bin/env python

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
