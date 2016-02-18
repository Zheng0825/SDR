

from socketIO_client import SocketIO
import json
import sys


def main ():
	
	
	#"{"Macaddr" : "freq\x01"}"
	jdoc = json.loads(args[0])
	print("Frequency change arrived from" + jdoc.keys()[0])
	freq = jdoc[jdoc.keys()[0]]
	freq = freq.split('\')[0]

	socketIO = SocketIO('localhost', 5000, '/test')	
	socketIO.emit('alfred set freq', freq)
	socketIO.close()


if __name__ == '__main__':
	for line in sys.stdin:
		main(line)
	
