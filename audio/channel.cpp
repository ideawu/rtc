#include "channel.h"

namespace audio{

Channel::Channel(){
	id = 0;
	next_packet_seq = -1;
}

Channel::~Channel(){
}

Packet* Channel::next_packet(){
	while(packets.size() > 0){
		Packet &packet = packets.front();
		if(next_packet_seq == -1 || packet.seq == next_packet_seq){
			next_packet_seq = packet.seq + 1;
			packets.pop_front();
			out_packet = packet;
			return &out_packet;
		}else if(packet.seq < next_packet_seq){
			// 丢弃乱序包
			packets.pop_front();
			continue;
		}else if(packet.seq > next_packet_seq){
			// packet is lost
			next_packet_seq ++;
			break;
		}
	}
	return NULL;
}

int Channel::push_packet(const Packet &packet){
	packets.push_back(packet);
}

}; // namespace
