#ifndef RTC_AUDIO_MIXER_H
#define RTC_AUDIO_MIXER_H

#include <map>
#include "frame.h"
#include "channel.h"

namespace voice{

// ticks_per_frame
class Mixer
{
private:
	Frame mixed_frame;
	std::map<int, Channel *> channels;
	std::vector<const Frame *> mix_list;
	const static int BUF_SIZE = 3;
public:
	void reset(){
		std::map<int, Channel *>::iterator it;
		for(it = channels.begin(); it != channels.end(); it++){
			Channel *channel = it->second;
			delete channel;
		}
		channels.clear();
	}
	
	Channel* get_channel(int channel_id);
	void free_channel(int channel_id);
	int process_frame(int channel_id, const Frame &frame);
	
	// 当需要混声下一个frame时, 调用本方法.
	// 如果所有声道的下一个frame为空, 则返回NULL, 否则返回混声后的frame.
	Frame* mix();
	
	// struct MixItem{channel, frame};
	//std::vector<MixItem *> *mixlist();
	//Frame* mix_without_channel(channel_id);
};

};

#endif
