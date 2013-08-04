#include "base/log.h"
#include "channel.h"

namespace audio{

Channel::Channel(){
	id = 0;
	slow_start = 0;
	idle = 0;
	next_seq = 0;
}

Channel::~Channel(){
}

int Channel::push_frame(const Frame &frame){
	if(frames.size() > BUF_SIZE){
		// maybe we should not drop these frames, we just force
		// the mixer to mix quicker
		while(frames.size() > BUF_SIZE){
			frames.pop_front();
			next_seq ++;
		}
		log_trace("buf exceed %d, drop, next_seq: %d", BUF_SIZE, next_seq);
	}
	frames.push_back(frame);
	return 1;
}

Frame* Channel::last_frame(){
	return &this->out_frame;
}

const Frame* Channel::next_frame(){
	if(slow_start < BUF_SIZE){
		log_trace("slow start %d", slow_start);
		slow_start ++;
		return NULL;
	}
	
	while(frames.size() > 0){
		out_frame = frames.front();
		if(next_seq == 0 || out_frame.seq == next_seq){
			frames.pop_front();
			next_seq = out_frame.seq + 1;
			idle = 0;
			log_trace("channel[%d] mixed, buf=%d", this->id, this->frames.size());
			return &this->out_frame;
		}else if(out_frame.seq < next_seq){
			// 丢弃乱序包
			frames.pop_front();
			continue;
		}else if(out_frame.seq > next_seq){
			// frame is lost
			next_seq ++;
			break;
		}
	}

	idle ++;
	// TODO:
	// simulate frame, break
	return NULL;
}

}; // namespace
