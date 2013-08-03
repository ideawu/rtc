#include "base/log.h"
#include "mixer.h"

namespace audio{

void Mixer::free_channel(int channel_id){
	channels.erase(channel_id);
}

int Mixer::process_chunk(int channel_id, const Chunk &chunk){
	Channel *channel;
	std::map<int, Channel *>::iterator it = channels.find(channel_id);
	if(it == channels.end()){
		channel = new Channel();
		channel->id = channel_id;
		channels[channel_id] = channel;
	}else{
		channel = it->second;
	}
	channel->push_chunk(chunk);
	return 0;
}

Chunk* Mixer::tick(){
	this->out_chunk.reset();
	
	std::map<int, Channel *>::iterator it;
	for(it = channels.begin(); it != channels.end(); it++){
		Channel *channel = it->second;
		channel->mix_into(&this->out_chunk);
	}
	
	// remove idle channel
	for(it = channels.begin(); it != channels.end();){
		Channel *channel = it->second;
		if(channel->is_idle()){
			log_debug("remove idle channel[%d]", channel->id);
			channels.erase(it++);
			delete channel;
		}else{
			it ++;
		}
	}
	
	return &this->out_chunk;
}

}; // namespace

