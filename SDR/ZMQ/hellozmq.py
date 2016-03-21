

"""Decentralized chat example"""

import argparse
import os
from threading import Thread

from netifaces import interfaces, ifaddresses, AF_INET # dependency, not in stdlib

import zmq

class Node:
    def __init__(IP,tun0,bat0):
        self.IP = IP
        self.tun0 = tun0
        self.bat0 = bat0

def listen(masked):
    """listen for messages
    
    masked is the first three parts of an IP address:
    
        192.168.1
    
    The socket will connect to all of X.Y.Z.{1-254}.
    """
    ctx = zmq.Context.instance()
    listener = ctx.socket(zmq.SUB)
    for last in range(99, 110):
        listener.connect("tcp://{0}.{1}:9001".format(masked, last))
    
    listener.setsockopt(zmq.SUBSCRIBE, b'')
    while True:
        try:
            listenerOut = listener.recv_string()
            print(listenerOut)
            handleMsg(listenerOut)
        except (KeyboardInterrupt, zmq.ContextTerminated):
            break


def handleMsg(msg):
    #format    
    #IP/tun0/bat0: freqChange rx/tx
    #IP/tun0/bat0: OK null
    #IP/tun0/bat0: heartbeat time
    
    #parse
    msgParts = msg.split(' ')
    sourceDat = msgParts[0].split('/')
    msgType = msgParts[1]
    msgDat = msgParts[2]

    IP = sourceDat[0]
    tun0 = sourceDat[1]
    bat0 = sourceDat[2]

    UpdateNodes(IP,tun0,bat0)

    if msgType == 'freqChange':
        print "Ip = " + str(IP)
        print "tun0 = " + str(tun0)
        print "bat0 = " + str(bat0)
        print "msgDat = " + str(msgDat)
    elif msgType == 'OK':
        #updateChangeTable(IP)
        pass
    elif msgType == 'heartbeat':
        #whatever  heartbeats do
        pass
    else:
        print ("Invalid Message Type")
`

def UpdateNodes(IP,tun0,bat0):
    pass





def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("interface", type=str, help="the network interface",
        choices=interfaces(),
    )
    parser.add_argument("user", type=str, default=os.environ['USER'],
        nargs='?',
        help="Your username",
    )
    args = parser.parse_args()
    inet = ifaddresses(args.interface)[AF_INET]
    addr = inet[0]['addr']
    masked = addr.rsplit('.', 1)[0]
    
    ctx = zmq.Context.instance()
    
    listen_thread = Thread(target=listen, args=(masked,))
    listen_thread.start()
    
    bcast = ctx.socket(zmq.PUB)
    bcast.bind("tcp://%s:9001" % args.interface)
    print("starting chat on %s:9001 (%s.*)" % (args.interface, masked))
    while True:
        try:
            msg = raw_input()
            #IP/tun0/bat0: freqChange rx/tx
            message = "192.168.200.104/FF:FF:FF:FF:FF:FF/00:00:00:00:00:00 freqChange 915000"
            bcast.send_string(message)
            #bcast.send_string("%s: %s" % (args.user, msg))
        except KeyboardInterrupt:
            break
    bcast.close(linger=0)
    ctx.term()

if __name__ == '__main__':
    main()

