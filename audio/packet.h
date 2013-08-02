#ifndef RTC_AUDIO_PACKET_H
#define RTC_AUDIO_PACKET_H

#include <string>

namespace audio{

class Packet
{
public:
	int seq;
	std::string buf;
	
	Packet(){
		seq = 0;
	}
	
	void clear(){
		seq = 0;
		buf.clear();
	}
	
	uint16_t* frames() const{
		return (uint16_t *)buf.data();
	}
	
	int size() const{
		return buf.size()/2;
	}
	
	void mix(const Packet &pkt){
		int size = std::min(this->size(), pkt.size());
		uint16_t *dst = this->frames();
		uint16_t *src = pkt.frames();
		for(int i=0; i<size; i++){
			dst[i] = dst[i]/2 + src[i]/2;
		}
	}
	
	void unmix(const Packet &pkt){
		int size = std::min(this->size(), pkt.size());
		uint16_t *dst = this->frames();
		uint16_t *src = pkt.frames();
		for(int i=0; i<size; i++){
			dst[i] = dst[i] + (dst[i] - src[i]);
		}
	}
};

};

#endif
