#ifndef RTC_AUDIO_MIXER_H
#define RTC_AUDIO_MIXER_H

#include <map>
#include "packet.h"
#include "channel.h"

namespace audio{

// ticks_per_packet
class Mixer
{
private:
	std::map<int, Channel *> channels;
	Packet out_packet;
public:
	void free_channel(int channel_id);
	int process_packet(int channel_id, const Packet &packet);
	
	// 当需要混声下一个packet时, 调用本方法.
	// 如果所有声道的下一个packet为空, 则返回NULL, 否则返回混声后的packet.
	Packet* tick();
};

};

#endif
