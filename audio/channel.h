#ifndef RTC_AUDIO_CHANNEL_H
#define RTC_AUDIO_CHANNEL_H

#include <list>
#include "packet.h"
#include "channel.h"

namespace audio{

class Channel
{
private:
	std::list<Packet> packets;
	Packet out_packet;
public:
	int buffering;
	int dropped;

	Channel();
	~Channel();
	
	int id;
	int next_packet_seq;
	//Packet* simulate_lost_frame(int seq);
	
	Packet* next_packet();
	int push_packet(const Packet &packet);
};

};

#endif
