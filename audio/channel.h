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
	const static int BUF_SIZE = 3;
	int slow_start;
	int idle;
	int next_seq;
public:
	int id;
	std::list<Chunk> chunks;

	Channel();
	~Channel();
	
	bool ready(){
		return slow_start >= BUF_SIZE;
	}
	
	bool is_idle(){
		return idle > BUF_SIZE;
	}
	
	//Chunk* simulate_lost_frame(int seq);
	
	Chunk* next_chunk();
	int push_chunk(const Chunk &chunk);
};

};

#endif
