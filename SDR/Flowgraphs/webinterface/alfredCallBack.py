

from socketIO_client import SocketIO, BaseNamespace
#import json
import sys
#import os

class Namespace(BaseNamespace):
    def on_connect(self):
        print('Connected')

def send (lines):

	data = []
	
	for line in lines:
	#"{"Macaddr" : "freq\x01"}"
		print (line)

		frame = line.strip()[2:-3].split(', ')
		freq = frame[1].split('\\')[0]
		freq = freq[1:]
		mac = frame[0]
		data.append([mac,freq])

	if len(data) == 1:
		socketIO = SocketIO('127.0.0.1', 5000)
		socketIO.emit('alfred set freq', str(data[1]))
		socketIO.wait(1)

		print("Frequency change to " + data[1] +  " arrived from " + data[0])
		return

	freqlist = {}

	for chunk in data:
		if chunk[1] not in freqlist:
			freqlist[chunk[1]] = 1
		else:
			freqlist[chunk[1]] += 1

	print freqlist 


	print("Frequency change to " + data[0][1] +  " arrived from " + data[0][0])
	 

	socketIO = SocketIO('127.0.0.1', 5000)
	socketIO.emit('alfred set freq', str(freq[1:]))
	socketIO.wait(1)


if __name__ == '__main__':

	lines = []

	for line in sys.stdin:	
		lines.append(line)
	send(lines)
	
	# Left a piece of dummy data here in case
	# its needed for debugging
	#send('{ "ca:f3:d4:01:f3:04", "915000000\x0a" }, { "2e:9c:6a:a8:54:96", "900000000\x0a" },')
