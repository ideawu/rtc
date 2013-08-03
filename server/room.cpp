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
	const std::vector<Bytes> *params = req->params();
	if(params->size() < 2){
		log_error("missing arguments");
		return -1;
	}
	int room_id = params->at(0).Int();
	if(room_id != this->id){
		log_error("invalid room_id: %d, should be: %d", room_id, this->id);
		return -1;
	}
	const Bytes &data = params->at(1);

	audio::Chunk chunk;
	chunk.seq = req->seq();
	chunk.buf.assign(data.data(), data.size());
	mixer.process_chunk(client->id, chunk);

	return 0;
}
	
void Room::tick(){
	audio::Chunk *chunk = mixer.tick();
	if(chunk == NULL){
		return;
	}
	if(chunk->buf.size() == 0){
		return;
	}
	this->broadcast(chunk->buf.data(), chunk->buf.size());
}

void Room::broadcast(const void *data, int size){
	Packet resp;
	resp.set_type(Packet::DATA);
	resp.set_seq(data_seq_next);
	resp.set_data(data, size); // TODO: room_id, data
	
	data_seq_next ++;
	
	std::map<int, Client*>::const_iterator it;
	for(it = clients_.begin(); it!=clients_.end(); it++){
		const Client *client = (*it).second;
		int ret = client->link->send(resp);
		if(ret <= 0){
			log_debug("error: %s", strerror(errno));
		}
		log_debug("send %d byte(s) to: %s", ret, client->addr.repr().c_str());
	}
}
