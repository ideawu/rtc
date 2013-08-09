#include<inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <iterator>

#include <opus.h>

#include "base/log.h"
#include "base/net.h"
#include "base/packet.h"
#include "base/udplink.h"


UdpLink* link_connect(std::string host, int port){
	int ret;
	UdpLink *link;
	link = UdpLink::client(host, port);
		
	Packet req;
	req.set_type(Packet::JOIN);
	req.set_seq(101);
	req.set_params("0", "token");
		
	ret = link->send(req);
	log_debug("send %d bytes", ret);
	if(ret <= 0){
		exit(0);
	}
	
	Packet resp;
	ret = link->recv(&resp);
	log_debug("recv %d bytes", ret);
	if(ret > 0){
		log_debug("recv: %s", resp.repr().c_str());
	}else{
		exit(0);
	}

	int bufsize = 5000;
	::setsockopt(link->fd(), SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));
	::setsockopt(link->fd(), SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));
	return link;
}
