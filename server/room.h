#ifndef RTC_SERVER_ROOM_H
#define RTC_SERVER_ROOM_H

#include <map>
#include <list>
#include <vector>
#include "base/net.h"
#include "room.h"
#include "client.h"


struct ClientList_cmp{
	bool operator()(const Address &a1, const Address &a2) const{
		return memcmp(&a1.sock_addr, &a2.sock_addr, sizeof(a1.sock_addr)) < 0;
	}
};
typedef std::map<Address, Client, ClientList_cmp> ClientList;

class Room
{
private:
  	ClientList clients_;
public:
	int id;

	const ClientList* clients(){
		return &clients_;
	}
	
	void join(const Address &addr, const Client &client){
		clients_[addr] = client;
	}
	
	bool has(const Address &addr){
		return clients_.find(addr) != clients_.end();
	}
	
	int join(Client *client);
	int quit(Client *client);
};

class RoomSvc
{
private:
	std::vector<Room *> rooms;
	std::list<Room *> tmp_rooms;
public:
	RoomSvc();
	~RoomSvc();
	
	Room* get(int id);
	Room* open();
	int close(int id);
	
};

#endif
