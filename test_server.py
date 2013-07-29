import socket, select;

host = '';
port = 10210;

serv = socket.socket(socket.AF_INET, socket.SOCK_DGRAM);
serv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1);
serv.bind((host, port));

clients = {};

while True:
	data, addr = serv.recvfrom(2 * 8192);
	print len(data);
	if clients.has_key(addr):
		pass;
	else:
		clients[addr] = addr;
		print 'new client from: ', addr
	for addr in clients.iterkeys():
		serv.sendto(data, addr);

