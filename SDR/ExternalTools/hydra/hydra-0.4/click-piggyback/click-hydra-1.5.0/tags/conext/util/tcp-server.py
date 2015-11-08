#!/usr/bin/env python

import threading
import struct
import optparse
import socket
import sys
import time

chunk_size = 1024

class TCP_log(threading.Thread):
  def __init__(self, sock):
    threading.Thread.__init__(self)
    self.sock=sock
    self.__running=False
    self.stime = time.time()

  def run(self):

    self.__running= True
    while(self.__running):
      tcp_info=self.sock.getsockopt(socket.SOL_TCP, socket.TCP_INFO, 111) 
      (rtt)=struct.unpack('<I',tcp_info[67:71])
      (cwnd)=struct.unpack('<I',tcp_info[79:83])
      (lostack)=struct.unpack('<I',tcp_info[35:39])
      #(ssthresh)=struct.unpack('<I',tcp_info[75:79])
      print"%.6f"%(time.time()-self.stime)
      print"rtt %d"%(rtt)
      #print"ssthresh %d"%(ssthresh)
      print"cwnd %d"%(cwnd)
      print"lostack %d"%(lostack)

      time.sleep(10)
    
  def quit(self):
    self.__running = False

class ClientConnection(threading.Thread):
    def __init__(self, (sock, addr), total, r):
        threading.Thread.__init__(self)
        sys.stderr.write("Received a connection from %s\n" % str(addr))
        self.__remaining = total
        self.__sock = sock
        self.tcp_log = TCP_log(self.__sock)
        self.__r = r

    def run(self):
        self.__running = True
        self.tcp_log.start()
        while self.__remaining > 0 and self.__running:
            try:
                d_size = min(self.__remaining, chunk_size)
                data = self.__r.read(d_size)
               
                self.__sock.send(data)
                self.__remaining = self.__remaining - d_size
            except socket.error, (client, error):
                sys.stderr.write("Client connection error: %s\n" % error)
                break
        self.tcp_log.quit()
        self.quit()

    def quit(self):
        self.__running = False
        self.__sock.close()


class Server(threading.Thread):
    def __init__(self, port, total):
        threading.Thread.__init__(self)
        self.setDaemon(True)
        self.__total = total
        self.__clients = []
        self.__r = open("/dev/urandom")
        sys.stderr.write("Listening for connections on port %d\n" % port)

        try:
            self.__sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.__sock.bind(('', port))
            self.__sock.listen(5)
        except socket.error, (client, error):
            sys.stderr.write("Error: %s\n" % error)

    def run(self):
        self.__running = True
        while self.__running:
            client = ClientConnection(self.__sock.accept(), self.__total,
                                      self.__r)
            client.start()
            self.__clients.append(client)

    def quit(self):
        self.__running = False
        self.__sock.close()
        for client in self.__clients:
            client.quit()


def main():
    parser = optparse.OptionParser()
    parser.add_option("-p", "--dest-port", type="int", default=40000,
                      dest="port", help="destination port to connect to "
                                        "(default=%default)")
    parser.add_option("-t", "--total-size", type="int", default=1e6,
                      dest="total", help="bytes to transmit "
                                         "(default=%default)")
    (options, args) = parser.parse_args()

    if len(args) > 1:
        parser.print_help()
        sys.exit(1)

    server = Server(options.port, int(options.total))
    server.start()

    print "Press Enter to exit..."
    r = sys.stdin.read(1)
    server.quit()

if __name__ == '__main__':
    main()
