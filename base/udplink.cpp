#include "net.h"
#include "packet.h"
#include "udplink.h"

UdpLink::UdpLink(){
	sock = -1;
}

UdpLink::~UdpLink(){
	this->close();
}
	
UdpLink* UdpLink::create(){
	int sock = -1;
	UdpLink *link;
	
	if((sock = ::socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		goto sock_err;
	}
	
	link = new UdpLink();
	link->sock = sock;
	return link;
sock_err:
	return NULL;
}

UdpLink* UdpLink::server(const std::string &host, int port){
	int opt = 1;
	UdpLink *link = UdpLink::create();
	
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons((short)port);
	inet_pton(AF_INET, host.c_str(), &addr.sin_addr);
	
	if(::setsockopt(link->sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1){
		goto sock_err;
	}
	if(::bind(link->sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){
		goto sock_err;
	}
	return link;
sock_err:
	if(link){
		delete link;
	}
	return NULL;
}

int UdpLink::bind(const std::string &host, int port){
	int opt = 1;
	
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons((short)port);
	inet_pton(AF_INET, host.c_str(), &addr.sin_addr);
	
	if(::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1){
		goto sock_err;
	}
	if(::bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){
		goto sock_err;
	}
	return 0;
sock_err:
	return -1;
}

UdpLink* UdpLink::client(const std::string &host, int port){
	UdpLink *link = UdpLink::create();

	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons((short)port);
	inet_pton(AF_INET, host.c_str(), &addr.sin_addr);

	if(::connect(link->sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){
		goto sock_err;
	}
	return link;
sock_err:
	if(link){
		delete link;
	}
	return NULL;
}

int UdpLink::connect(const std::string &host, int port){
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons((short)port);
	inet_pton(AF_INET, host.c_str(), &addr.sin_addr);

	if(::connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){
		goto sock_err;
	}
	return 0;
sock_err:
	return -1;
}

int UdpLink::connect(const Address &addr){
	if(::connect(sock, (struct sockaddr *)&addr.sock_addr, sizeof(addr.sock_addr)) == -1){
		goto sock_err;
	}
	return 0;
sock_err:
	return -1;
}
	
int UdpLink::send(const Packet &packet, const Address *addr){
	int ret;
	if(addr){
		socklen_t len = sizeof(addr->sock_addr);
		ret = ::sendto(sock, packet.buf(), packet.len, 0,
			(struct sockaddr *)&addr->sock_addr, len);
	}else{
		ret = ::write(sock, packet.buf(), packet.len);
	}
	return ret;
}

int UdpLink::recv(Packet *packet, Address *addr){
	int ret;
	if(addr){
		socklen_t len = sizeof(addr->sock_addr);
		ret = ::recvfrom(sock, packet->buf(), Packet::MAX_LEN, 0,
			(struct sockaddr *)&addr->sock_addr, &len);
	}else{
		ret = ::read(sock, packet->buf(), Packet::MAX_LEN);
	}
	packet->len = (ret > 0)? ret : 0;
	return ret;
}

