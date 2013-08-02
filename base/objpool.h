#ifndef RRoomC_BASE_OBJPOOL_H
#define RRoomC_BASE_OBJPOOL_H

#include <map>
#include <list>

class RoomSvc
{
private:
	std::map<int, Room *> used;
	std::list<Room *> pool;
public:
	Room* get(int id){
		std::map<int, Room *>::iterator it = used.find(id);
		if(used.find(id) == used.end()){
			return NULL;
		}else{
			return *it;
		}
	}

	Room* alloc(){
		Room *t = NULL;
		if(pool.size() > 0){
			t = pool.front();
			pool.pop_front();
			used[t->id] = t;
		}else{
			t = new Room();
			t->id = used.size();
			used[t->id] = t;
		}
		return t;
	}

	void free(int id){
		std::map<int, Room*>::iterator it = used.find(id);
		if(it == used.end()){
			return;
		}else{
			pool.push_back(*it);
			used.erase(it);
		}
	}
};

#endif
