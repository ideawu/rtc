#include "base/log.h"
#include "mixer.h"

namespace audio{

int Mixer::process_packet(int channel_id, const Packet &packet){
	Channel *channel;
	std::map<int, Channel *>::iterator it = channels.find(channel_id);
	if(it == channels.end()){
		channel = new Channel();
		channel->id = channel_id;
		channels[channel_id] = channel;
	}else{
		channel = it->second;
	}
	channel->push_packet(packet);
	return 0;
}

Packet* Mixer::tick(){
	this->out_packet.clear();
	
	std::map<int, Channel *>::iterator it;
	for(it = channels.begin(); it != channels.end(); it++){
		Channel *channel = it->second;
		
		if(channel->buffering < BUF_SIZE){
			channel->buffering ++;
			continue;
		}

		Packet *packet = channel->next_packet();
		if(!packet){
			log_debug("channel[%d] packet lost", channel->id);
			channel->dropped ++;
			// TODO:
			// simulate packet, break
			continue;
		}else{
			// we only cound continuous dropped packets
			channel->dropped = 0;
		}
		
		log_debug("channel[%d] packet mixed", channel->id);
		this->out_packet.mix(*packet);
		// TODO: remember which channel is mixed,
		// unmix before sending the mixed packet to those users
	}
	// remove idle channel
	for(it = channels.begin(); it != channels.end();){
		Channel *channel = it->second;
		if(channel->dropped > BUF_SIZE){
			log_debug("remove idle channel[%d]", channel->id);
			channels.erase(it++);
			delete channel;
		}else{
			it ++;
		}
	}
	
	return &this->out_packet;
}

}; // namespace

