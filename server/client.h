#ifndef RTC_SERVER_CLIENT_H
#define RTC_SERVER_CLIENT_H

#include <map>
#include <list>
#include "base/udplink.h"

class Address;
class Room;

class Client
{
public:
	int id;
	Address addr;
	UdpLink *link;
	Room *room;
	
	struct{
		uint32_t send_next;
		uint32_t recv_next;
	}seq;
	
	Client(){
	}
	
	~Client(){
	}
};


class ClientSvc
{
private:
	std::list<Client *> pool;
	std::map<int, Client *> items;
public:
	typedef std::map<int, Client *>::iterator iterator;
	
	iterator begin(){
		return items.begin();
	}
	
	iterator end(){
		return items.end();
	}

	Client* get(int id){
		std::map<int, Client *>::iterator it = items.find(id);
		if(items.find(id) == items.end()){
			return NULL;
		}else{
			return it->second;
		}
	}

	Client* alloc(){
		Client *t = NULL;
		if(pool.size() > 0){
			t = pool.front();
			pool.pop_front();
			items[t->id] = t;
		}else{
			t = new Client();
			t->id = items.size();
			items[t->id] = t;
		}
		return t;
	}

	void free(int id){
		std::map<int, Client*>::iterator it = items.find(id);
		if(it == items.end()){
			return;
		}else{
			pool.push_back(it->second);
			items.erase(it);
		}
	}
};

#endif
