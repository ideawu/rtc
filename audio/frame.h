#ifndef RTC_AUDIO_PACKET_H
#define RTC_AUDIO_PACKET_H

#include <string>

namespace audio{

class Frame
{
public:
	uint16_t seq;
	int num_mixed;
	std::string buf;
	
	Frame(){
		reset();
	}
	
	void reset(){
		seq = 0;
		num_mixed = 0;
		buf.clear();
	}
	
	bool empty() const{
		return buf.empty();
	}
	
	int16_t* samples() const{
		return (int16_t *)buf.data();
	}
	
	int size() const{
		return buf.size()/2;
	}
	
	// TODO: not this simple
	void mix(const Frame &frame){
		num_mixed ++;
		if(this->buf.size() < frame.buf.size()){
			this->buf.resize(frame.buf.size(), 0);
		}
		int16_t *dst = this->samples();
		int16_t *src = frame.samples();
		int size = this->size();
		for(int i=0; i<size; i++){
			// the webrtc algorithm
			int32_t val = (int32_t)dst[i] + (int32_t)src[i]/2;
			if(val < -32768){
				dst[i] = -32768;
			}else if(val > 32767){
				dst[i] = 32767;
			}else{
				dst[i] = (int16_t)val;
			}
		}
	}
	
	// TODO: just like echo cancellation?
	// unmix this frame out of mixed, and set this frame as the result
	void unmix_from(const Frame &mixed){
		if(mixed.num_mixed <= 1){
			this->buf.clear();
			return;
		}
		int16_t *dst = this->samples();
		int16_t *src = mixed.samples();
		int size = std::min(this->size(), mixed.size());
		for(int i=0; i<size; i++){
			int32_t val = (int32_t)src[i] - (int32_t)dst[i]/2;
			if(val < -32768){
				dst[i] = -32768;
			}else if(val > 32767){
				dst[i] = 32767;
			}else{
				if(mixed.num_mixed == 2){
					dst[i] = (int16_t)(val * 2);
				}else{
					dst[i] = (int16_t)val;
				}
			}
		}
	}
	
	/* not this way
	void mix(const Frame &frame){
		num_mixed ++;
		if(this->buf.size() < frame.buf.size()){
			this->buf.resize(frame.buf.size(), 0);
		}
		int16_t *dst = this->samples();
		int16_t *src = frame.samples();
		int size = this->size();
		for(int i=0; i<size; i++){
			dst[i] = dst[i]/2 + src[i]/2;
		}
	}
	
	// TODO: just like echo cancellation?
	// unmix this frame out of mixed, and set this frame as the result
	void unmix_from(const Frame &mixed){
		if(mixed.num_mixed <= 1){
			this->buf.clear();
			return;
		}
		int16_t *dst = this->samples();
		int16_t *src = mixed.samples();
		int size = std::min(this->size(), mixed.size());
		for(int i=0; i<size; i++){
			dst[i] = src[i] * 2 - dst[i];
		}
	}
	*/
};

};

#endif
