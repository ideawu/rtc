#ifndef RTC_AUDIO_PACKET_H
#define RTC_AUDIO_PACKET_H

#include <string>

namespace audio{

class Chunk
{
public:
	int seq; // TODO:
	std::string buf;
	
	Chunk(){
		reset();
	}
	
	void reset(){
		seq = 0;
		buf.clear();
	}
	
	uint16_t* frames() const{
		return (uint16_t *)buf.data();
	}
	
	int size() const{
		return buf.size()/2;
	}
	
	void mix(const Chunk &pkt){
		this->buf = pkt.buf;
		/*
		if(this->buf.size() < pkt.buf.size()){
			this->buf.resize(pkt.buf.size(), 0);
		}
		uint16_t *dst = this->frames();
		uint16_t *src = pkt.frames();
		int size = this->size();
		for(int i=0; i<size; i++){
			dst[i] = dst[i]/2 + src[i]/2;
		}
		*/
	}
	
	void unmix(const Chunk &pkt){
		uint16_t *dst = this->frames();
		uint16_t *src = pkt.frames();
		int size = std::min(this->size(), pkt.size());
		for(int i=0; i<size; i++){
			dst[i] = dst[i] + (dst[i] - src[i]);
		}
	}
};

};

#endif
