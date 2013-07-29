#ifndef RTC_SERVER_SERVER_H
#define RTC_SERVER_SERVER_H

#include "room.h"

class RoomSvc;
class ClientSvc;
class UdpLink;
class Packet;
class Address;

class Server
{
public:
	//RoomSvc room_svc;
	//ClientSvc client_svc;
	Room room; // TODO
	
	Server();
	~Server();

	int proc(UdpLink *link);
private:
	int proc_join(UdpLink *link, Packet *req, Address *addr);
	int proc_data(UdpLink *link, Packet *req, Address *addr);
};

#endif
