#ifndef RTC_AUDIO_CHANNEL_H
#define RTC_AUDIO_CHANNEL_H

#include <list>
#include "chunk.h"
#include "channel.h"

namespace audio{

class Channel
{
private:
	Chunk out_chunk;
	const static int BUF_LOW	= 3;
	const static int BUF_HIGH	= 6;
	int slow_start;
	int idle;
	int next_seq;
public:
	int id;
	std::list<Chunk> chunks;

	Channel();
	~Channel();
	
	bool is_idle(){
		return idle > BUF_HIGH;
	}
	
	//Chunk* simulate_lost_frame(int seq);
	
	Chunk* next_chunk();
	int push_chunk(const Chunk &chunk);
};

};

#endif
