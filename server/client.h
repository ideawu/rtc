#ifndef RTC_SERVER_CLIENT_H
#define RTC_SERVER_CLIENT_H

class Address;
class UdpLink;
class Room;

class Client
{
public:
	int id;
	Address addr;
	UdpLink *link;
	Room *room;
	uint32_t sub_seq;
	uint32_t pub_seq;
	
	Client(){
		sub_seq = 0;
		pub_seq = 0;
	}
};

#endif
