#ifndef RTC_AUDIO_PACKET_H
#define RTC_AUDIO_PACKET_H

#include <string>

namespace audio{

class Chunk
{
public:
	uint16_t seq;
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
	
	// TODO: voice gain
	// mix this chunk into mixed
	void mix_into(Chunk *mixed) const{
		if(this->buf.size() > mixed->buf.size()){
			if(mixed->buf.empty()){
				mixed->buf = this->buf;
				return;
			}
			mixed->buf.resize(this->buf.size(), 0);
		}
		int16_t *src = this->frames();
		int16_t *dst = mixed->frames();
		int size = this->size();
		for(int i=0; i<size; i++){
			dst[i] = (dst[i] + src[i])/2;
		}
	}
	
	// unmix this chunk out of mixed, and set this chunk as the result
	void unmix_from(const Chunk &mixed){
		int16_t *dst = this->frames();
		int16_t *src = mixed.frames();
		int size = std::min(this->size(), mixed.size());
		for(int i=0; i<size; i++){
			dst[i] = src[i] * 2 - dst[i];
		}
	}
};

};

#endif
