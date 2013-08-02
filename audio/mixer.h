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
	// 对于一条新的声道, 尝试先缓冲一部分数据再处理.
	// 也就是在最初的BUF_SIZE个tick周期内, 忽略该声道的数据
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
	int process_packet(int channel_id, const Packet &packet);
	
	// 当需要混声下一个packet时, 调用本方法.
	// 如果所有声道的下一个packet为空, 则返回NULL, 否则返回混声后的packet.
	Packet* tick();
};

};

#endif
