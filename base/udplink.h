#ifndef RTC_BASE_UDPLINK_H
#define RTC_BASE_UDPLINK_H

#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <string>

class Packet;
class Address;

class UdpLink
{
private:
	int sock;

public:
	void *data;

	UdpLink();
	~UdpLink();
	
	static UdpLink* create();
	static UdpLink* server(const std::string &host, int port);
	static UdpLink* client(const std::string &host, int port);

	int fd() const{
		return sock;
	}
	
	void close(){
		if(sock >= 0){
			::close(sock);
			sock = -1;
		}
	}
	
	int bind(const std::string &host, int port);
	int connect(const std::string &host, int port);
	int connect(const Address &addr);
	
	int send(const Packet &packet, const Address *addr=NULL);
	int recv(Packet *packet, Address *addr=NULL);
};

#endif
