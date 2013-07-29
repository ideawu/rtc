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
	
	UdpLink();

public:
	~UdpLink();
	
	static UdpLink* create();
	static UdpLink* server(const std::string host, int port);
	static UdpLink* client(const std::string host, int port);
	
	int bind(const std::string host, int port);
	int connect(const std::string host, int port);
	
	int send(const Packet &packet, const Address *addr=NULL);
	int recv(Packet *packet, Address *addr=NULL);
};

#endif
