

from socketIO_client import SocketIO, BaseNamespace
#import json
import sys
#import os

class Namespace(BaseNamespace):
    def on_connect(self):
        print('Connected')

def send (lines):
	print(lines)
	
	data = []
	
	for line in lines:
	#"{"Macaddr" : "freq\x01"}"
		print (line)

		frame = line.strip()[2:-3].split(', ')
		freqandtime = frame[1].split('\\')[0]
		freq,time = freqandtime[1:].split('%')
		time = time[:-2]
		mac = frame[0]

		if not data:
			data = [mac,freq,time]
		elif data[2] >= time:
			data = [mac,freq,time]


	socketIO = SocketIO('127.0.0.1', 5000)
	socketIO.emit('alfred set freq', str(data[1]))
	socketIO.wait(1)

if __name__ == '__main__':

	lines = []

	for line in sys.stdin:	
		lines.append(line)
	send(lines)

	'''
	Some dummy data if you need to do testing
	line = '{ "c2:4c:e0:9f:80:07", "918000000%1456077160.77\x0a" },'
	lines = []
	lines.append(line)
	send(lines)
	'''

