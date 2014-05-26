import socket
import sys
import json
import Library
from threading import Thread
from time import sleep
import os

def checktoexit():
	while Library.alive():
		sleep(3)
	os._exit(1)
	print("QUIT")

t = Thread(target=checktoexit)
t.start()

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('localhost', 5558)
sock.bind(server_address)
sock.listen(1)
while Library.alive():
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
