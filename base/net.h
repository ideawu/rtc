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
	
	std::string repr() const{
		char buf[16];
		snprintf(buf, sizeof(buf), "%d", this->port());
		std::string ret = this->host();
		ret.append(":");
		ret.append(buf);
		return ret;
	}
};

#endif
