#include "base/log.h"
#include "room.h"

int Room::join(Client *client){
	clients_[client->id] = client;
	return 0;
}

int Room::quit(Client *client){
	clients_.erase(client->id);
	return 0;
}

int Room::publish(Client *client, Packet *req){
	//audio::Packet ap
	//mixer.process_packet(client->id, ap);
	std::map<int, Client*>::const_iterator it;
	for(it = clients_.begin(); it!=clients_.end(); it++){
		const Client *client = (*it).second;
		int ret = client->link->send(*req);
		if(ret <= 0){
			log_debug("error: %s", strerror(errno));
		}
		log_debug("send %d byte(s) to: %s", ret, client->addr.repr().c_str());
	}
	return 0;
}
	
void Room::tick(){
	audio::Packet *ap = mixer.tick();
	if(ap == NULL){
		return;
	}
}
