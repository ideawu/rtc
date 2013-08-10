#include <vector>
#include "base/log.h"
#include "mixer.h"

namespace voice{

Channel* Mixer::get_channel(int channel_id){
	std::map<int, Channel *>::iterator it = channels.find(channel_id);
	if(it == channels.end()){
		return NULL;
	}else{
		return it->second;
	}
}

void Mixer::free_channel(int channel_id){
	channels.erase(channel_id);
}

int Mixer::process_frame(int channel_id, const Frame &frame){
	Channel *channel;
	std::map<int, Channel *>::iterator it = channels.find(channel_id);
	if(it == channels.end()){
		channel = new Channel();
		channel->id = channel_id;
		channels[channel_id] = channel;
		log_debug("add new channel[%d]", channel->id);
	}else{
		channel = it->second;
	}
	channel->push_frame(frame);
	return 0;
}

Frame* Mixer::mix(){
	this->mixed_frame.reset();
	mix_list.clear();
	
	for(std::map<int, Channel *>::iterator it=channels.begin(); it!=channels.end();){
		Channel *channel = it->second;
		const Frame *frame = channel->next_frame();
		if(!frame){
			if(channel->ready()){
				log_trace("channel[%d] frame lost", channel->id);
			}
		}else{
			log_trace("channel[%d] mixed, buf=%d", channel->id, channel->size());
			mix_list.push_back(frame);
		}
		
		// remove idle channel
		if(channel->is_idle()){
			log_debug("remove idle channel[%d]", channel->id);
			channels.erase(it++);
			delete channel;
		}else{
			it ++;
		}
	}
		
	if(mix_list.size() == 0){
		return NULL;
	}else if(mix_list.size() == 1){
		this->mixed_frame.num_mixed = 1;
		this->mixed_frame.buf = mix_list[0]->buf;
	}else{
		for(std::vector<const Frame *>::const_iterator it=mix_list.begin();
			it!=mix_list.end(); it++)
		{
			const Frame *frame = *it;
			this->mixed_frame.mix(*frame);
		}
	}
	
	return &this->mixed_frame;
}

}; // namespace

