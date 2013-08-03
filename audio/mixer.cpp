#include "base/log.h"
#include "mixer.h"

namespace audio{

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

		Chunk *chunk = channel->next_chunk();
		if(!chunk){
			if(channel->ready()){
				log_trace("channel[%d] chunk lost", channel->id);
				// TODO:
				// simulate chunk, break
			}
			continue;
		}

		log_trace("channel[%d] mixed, buf=%d", channel->id, channel->chunks.size());
		this->out_chunk.mix(*chunk);
		// TODO: remember which channel is mixed,
		// unmix before sending the mixed chunk to those users
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

