#include "base/log.h"
#include "channel.h"

namespace audio{

Channel::Channel(){
	id = 0;
	slow_start = 0;
	idle = 0;
	next_seq = -1;
}

Channel::~Channel(){
}

Chunk* Channel::next_chunk(){
	if(slow_start < BUF_SIZE){
		log_trace("slow start %d", slow_start);
		slow_start ++;
		return NULL;
	}
	
	while(chunks.size() > 0){
		out_chunk = chunks.front();
		if(next_seq == -1 || out_chunk.seq == next_seq){
			chunks.pop_front();
			next_seq = out_chunk.seq + 1;
			idle = 0;
			return &out_chunk;
		}else if(out_chunk.seq < next_seq){
			// 丢弃乱序包
			chunks.pop_front();
			continue;
		}else if(out_chunk.seq > next_seq){
			// chunk is lost
			next_seq ++;
			break;
		}
	}

	idle ++;
	return NULL;
}

int Channel::push_chunk(const Chunk &chunk){
	if(chunks.size() > BUF_SIZE){
		// maybe we should not drop these chunks, we just force
		// the mixer to mix quicker
		while(chunks.size() > BUF_SIZE){
			chunks.pop_front();
			next_seq ++;
		}
		log_trace("buf exceed %d, drop, next_seq: %d", BUF_SIZE, next_seq);
	}
	chunks.push_back(chunk);
	return 1;
}

}; // namespace
