import socket
import sys
import json


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('localhost', 5558)
sock.bind(server_address)
sock.listen(1)
while 1==1:
	connection, client_address = sock.accept()
	try:
		while 1==1:
			
			data = connection.recv(8)
			sys.stdout.flush()
			if data:
				sys.stdout.write(data)
			else:
				break
	finally:
		connection.close()
