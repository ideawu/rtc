#ifndef RTC_AUDIO_CHANNEL_H
#define RTC_AUDIO_CHANNEL_H

#include <list>
#include "frame.h"
#include "channel.h"

namespace audio{

class Channel
{
private:
	const static int BUF_SIZE = 12;
	const static int SLOW_START_TICKS = BUF_SIZE/2;

	Frame out_frame;
	int slow_start;
	int idle;
	uint16_t next_seq;

	std::list<Frame> frames;
public:
	int id;

	Channel();
	~Channel();
	
	bool ready(){
		return slow_start >= SLOW_START_TICKS;
	}
	
	bool is_idle(){
		return idle > BUF_SIZE;
	}
	
	//Frame* simulate_lost_frame(int seq);
	
	Frame* last_frame();
	const Frame* next_frame();
	int push_frame(const Frame &frame);
};

};

#endif
