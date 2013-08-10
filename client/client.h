#ifndef RTC_CLIENT_CLIENT_H
#define RTC_CLIENT_CLIENT_H

#include <string>
#include "base/packet.h"
#include "base/udplink.h"

namespace rtc{

class Client
{
public:
	Client();
	~Client();
	
	UdpLink* link();
		
	int connect(std::string host, int port);
	int join(int room_id, std::string token);
	int quit();
};

#endif
