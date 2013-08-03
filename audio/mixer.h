#ifndef RTC_AUDIO_MIXER_H
#define RTC_AUDIO_MIXER_H

#include <map>
#include "chunk.h"
#include "channel.h"

namespace audio{

// ticks_per_chunk
class Mixer
{
private:
	std::map<int, Channel *> channels;
	Chunk out_chunk;
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
	
	void free_channel(int channel_id);
	int process_chunk(int channel_id, const Chunk &chunk);
	
	// 当需要混声下一个chunk时, 调用本方法.
	// 如果所有声道的下一个chunk为空, 则返回NULL, 否则返回混声后的chunk.
	Chunk* tick();
};

};

#endif
