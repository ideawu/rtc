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
	log_debug("");
	
	this->out_packet.clear();
	
	std::map<int, Channel *>::iterator it;
	for(it = channels.begin(); it != channels.end(); it++){
		Channel *channel = it->second;

		Packet *packet = channel->next_packet();
		if(!packet){
			// TODO:
			// if(channel->num_packets_dropped > ...) delete channel
			// else simulate packet
			continue;
		}
		this->out_packet.mix(*packet);
		// TODO: remember which channel is mixed,
		// unmix before sending the mixed packet to those users
	}
	
	return &this->out_packet;
}

}; // namespace

