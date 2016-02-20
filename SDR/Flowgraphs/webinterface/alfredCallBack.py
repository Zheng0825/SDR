

from socketIO_client import SocketIO, BaseNamespace
#import json
import sys
#import os

class Namespace(BaseNamespace):
    def on_connect(self):
        print('Connected')

def send (line):
		
	#"{"Macaddr" : "freq\x01"}"
	print (line)

	data = line.strip()[2:-3].split(', ')
	freq = data[1].split('\\')[0]
	mac = data[0]

	print("Frequency change to " + freq[1:] +  " arrived from " + mac)
	 

	socketIO = SocketIO('127.0.0.1', 5000)
	socketIO.emit('alfred set freq', str(freq[1:]))
	socketIO.wait(1)

if __name__ == '__main__':

	#for line in sys.stdin:	
	#	send(line)
	
	# Left a piece of dummy data here in case
	# its needed for debugging
	send('{ "ca:26:ff:04:42:e4", "922000000\x0a" }')
