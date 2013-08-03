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
	
	int16_t* frames() const{
		return (int16_t *)buf.data();
	}
	
	int size() const{
		return buf.size()/2;
	}
	
	void mix(const Chunk &pkt){
		if(this->buf.size() < pkt.buf.size()){
			if(this->buf.empty()){
				this->buf = pkt.buf;
				return;
			}
			this->buf.resize(pkt.buf.size(), 0);
		}
		int16_t *dst = this->frames();
		int16_t *src = pkt.frames();
		int size = this->size();
		for(int i=0; i<size; i++){
			dst[i] = (dst[i] + src[i])/2;
		}
	}
	
	void unmix(const Chunk &pkt){
		int16_t *dst = this->frames();
		int16_t *src = pkt.frames();
		int size = std::min(this->size(), pkt.size());
		for(int i=0; i<size; i++){
			dst[i] = dst[i] + (dst[i] - src[i]);
		}
	}
};

};

#endif
