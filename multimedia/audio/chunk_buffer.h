#ifndef AVE_CHUNK_BUFFER_H
#define AVE_CHUNK_BUFFER_H

#include <stdlib.h>

namespace ave{

class ChunkBuffer
{
private:
	int chunk_count;
	int chunk_size;
	int size;
	int first;
	char *buf;

public:
	ChunkBuffer(int chunk_count, int chunk_size){
		this->chunk_count = chunk_count;
		this->chunk_size = chunk_size;
		buf = (char *)malloc(chunk_count * chunk_size);
		size = 0;
		first = 0;
	}
	
	~ChunkBuffer(){
		free(buf);
	}
	
	void* space(){
		if(size >= chunk_count){
			return NULL;
		}
		int index = (first + size) % chunk_count;
		return buf + index * chunk_size;
	}
	
	bool push_back(){
		if(size >= chunk_count){
			return false;
		}
		size ++;
		return true;
	}
	
	void* pop_front(){
		if(size <= 0){
			return NULL;
		}
		void *ret = buf + first * chunk_size;
		size --;
		first = ++first % chunk_count;
		return ret;
	}
};

}; // namespace ave

#endif
