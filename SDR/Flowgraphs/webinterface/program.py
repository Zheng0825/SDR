

from socketIO_client import SocketIO, BaseNamespace
#import json
import sys
#import os

def send (line):
		
	#"{"Macaddr" : "freq\x01"}"
	print (line)

	data = line.strip()[2:-3].split(', ')
	freq = data[1].split('\\')[0]
	mac = data[0]

	print("Frequency change to " + freq[1:] +  " arrived from " + mac)
	 

	socketIO = SocketIO('localhost', 5000)
	socketIO.emit('alfred set freq', freq[1:])
	socketIO.close()


if __name__ == '__main__':	
	for line in sys.stdin:	
		main(line)
	
