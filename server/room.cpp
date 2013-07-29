#include "room.h"

RoomSvc::RoomSvc(){
}

RoomSvc::~RoomSvc(){
	for(std::vector<Room *>::iterator it = rooms.begin(); it != rooms.end(); it++){
		Room *room = *it;
		if(room != NULL){
			delete room;
		}
	}
	rooms.clear();
	
	for(std::list<Room *>::iterator it = tmp_rooms.begin(); it != tmp_rooms.end(); it++){
		Room *room = *it;
		if(room != NULL){
			delete room;
		}
	}
	tmp_rooms.clear();
}

Room* RoomSvc::get(int id){
	int index = id - 1;
	if(index >= 0 && index < this->rooms.size()){
		return rooms[index];
	}
	return NULL;
}

Room* RoomSvc::open(){
	Room *room = NULL;
	if(tmp_rooms.size() > 0){
		room = tmp_rooms.front();
		tmp_rooms.pop_front();
		rooms[room->id - 1] = room;
	}else{
		room = new Room();
		room->id = rooms.size();
		rooms.push_back(room);
	}
	return room;
}

int RoomSvc::close(int id){
	Room *room = this->get(id);
	if(!room){
		return -1;
	}

	int index = id - 1;
	rooms[index] = NULL;
	tmp_rooms.push_back(room);
	return id;
}
