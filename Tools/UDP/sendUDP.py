import socket

UDP_IP = "127.0.0.1"
UDP_PORT = 8084
MESSAGE = "Hello SDR World!"

print "UDP target IP: ", UDP_IP
print "UDP target Port: ", UDP_PORT
print "message: ", MESSAGE

#for a in range(0, 10000):
while(1):
	MESSAGE_SENT = MESSAGE
	print "Sent"
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.sendto(MESSAGE_SENT, (UDP_IP, UDP_PORT))
