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
	uint16_t next_seq;

	std::list<Chunk> chunks;
	// read next chunk from chunks list into out_chunk
	int next_chunk();
public:
	int id;

	Channel();
	~Channel();
	
	bool ready(){
		return slow_start >= BUF_SIZE;
	}
	
	bool is_idle(){
		return idle > BUF_SIZE;
	}
	
	//Chunk* simulate_lost_chunk(int seq);
	
	int mix_into(Chunk *chunk);
	const Chunk* unmix_from(const Chunk &mixed);
	
	int push_chunk(const Chunk &chunk);
};

};

#endif
