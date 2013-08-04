#include "base/log.h"
#include "room.h"

int Room::join(Client *client){
	clients_[client->id] = client;
	return 0;
}

int Room::quit(Client *client){
	mixer.free_channel(client->id);
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

	audio::Frame frame;
	frame.seq = req->seq();
	frame.buf.assign(data.data(), data.size());
	mixer.process_frame(client->id, frame);

	return 0;
}
	
void Room::tick(){
	audio::Frame *mixed_frame = mixer.mix();
	if(mixed_frame == NULL){
		return;
	}
	if(mixed_frame->buf.size() == 0){
		return;
	}
	//this->broadcast(frame->buf.data(), frame->buf.size());
	Packet resp;
	resp.set_type(Packet::DATA);
	resp.set_seq(data_seq_next);
	// TODO: room_id, data
	resp.set_data(mixed_frame->buf.data(), mixed_frame->buf.size());
	
	std::map<int, Client*>::const_iterator it;
	for(it = clients_.begin(); it!=clients_.end(); it++){
		const Client *client = (*it).second;
		audio::Channel *channel = mixer.get_channel(client->id);
		if(!channel || channel->last_frame()->empty()){
			int ret = client->link->send(resp);
			if(ret <= 0){
				log_debug("error: %s", strerror(errno));
			}
			log_debug("send %d byte(s) to: %s", ret, client->addr.repr().c_str());
		}else{
			audio::Frame *frame = channel->last_frame();
			frame->unmix_from(*mixed_frame);
			Packet p;
			p.set_type(Packet::DATA);
			p.set_seq(data_seq_next);
			// TODO: room_id, data
			p.set_data(frame->buf.data(), frame->buf.size());
			int ret = client->link->send(p);
			if(ret <= 0){
				log_debug("error: %s", strerror(errno));
			}
			log_debug("send %d byte(s) to: %s", ret, client->addr.repr().c_str());
		}
	}

	data_seq_next ++;
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
