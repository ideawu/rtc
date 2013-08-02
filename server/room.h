#ifndef RTC_SERVER_ROOM_H
#define RTC_SERVER_ROOM_H

#include <map>
#include <list>
#include "base/net.h"
#include "audio/mixer.h"
#include "room.h"
#include "client.h"

class Room
{
private:
  	std::map<int, Client*> clients_;
	audio::Mixer mixer;
	int ticks;
public:
	int id;

	Room(){
		ticks = 0;
	}
	
	void reset(){
		ticks = 0;
		clients_.clear();
		mixer.reset();
	}
	
	void tick();
	
	int join(Client *client);
	int quit(Client *client);
	
	int publish(Client *client, Packet *req);
};


class RoomSvc
{
private:
	std::list<Room *> pool;
	std::map<int, Room *> items;
public:
	typedef std::map<int, Room *>::iterator iterator;
	
	iterator begin(){
		return items.begin();
	}
	
	iterator end(){
		return items.end();
	}

	Room* get(int id){
		std::map<int, Room *>::iterator it = items.find(id);
		if(items.find(id) == items.end()){
			return NULL;
		}else{
			return it->second;
		}
	}

	Room* alloc(){
		Room *t = NULL;
		if(pool.size() > 0){
			t = pool.front();
			pool.pop_front();
			items[t->id] = t;
		}else{
			t = new Room();
			t->id = items.size();
			items[t->id] = t;
		}
		t->reset();
		return t;
	}

	void free(int id){
		std::map<int, Room*>::iterator it = items.find(id);
		if(it == items.end()){
			return;
		}else{
			pool.push_back(it->second);
			items.erase(it);
		}
	}
};

#endif
