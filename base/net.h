#ifndef RTC_BASE_NET_H
#define RTC_BASE_NET_H

#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <string.h>
#include <string>

class Address
{
public:
	struct sockaddr_in sock_addr;

public:
	Address();
	~Address();
	
	std::string host() const;
	int port() const;
};

#endif
