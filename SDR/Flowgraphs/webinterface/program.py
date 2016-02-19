

from socketIO_client import SocketIO
import json
import sys


def main (line):
	
	
	#"{"Macaddr" : "freq\x01"}"
	print (line)

	data = line.strip()[2:-3].split(', ')
	freq = data[1].split('\\')[0]
	mac = data[0]

	print("Frequency change to " + freq +  " arrived from " + mac)
	 

	socketIO = SocketIO('localhost', 5000, '/test')	
	socketIO.emit('alfred set freq', freq)
	socketIO.close()


if __name__ == '__main__':
	for line in sys.stdin:
		main(line)
	
