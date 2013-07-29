#ifndef RTC_BASE_PACKET_H
#define RTC_BASE_PACKET_H

#include <stdlib.h>
#include <vector>
#include "bytes.h"
#include "strings.h"

class Packet
{
	const static int MAX_PACKET_LEN = 500;
	std::vector<Bytes> params_;
public:
	int len;
	char buf[MAX_PACKET_LEN];

#define TYPE_VALUE(a, b) ((a)+((b)<<8))
	// if big-endian ((a<<8)+(b))
	const static int16_t RESP = TYPE_VALUE('r', 'e');
	const static int16_t JOIN = TYPE_VALUE('j', 'o');
	const static int16_t QUIT = TYPE_VALUE('q', 'u');
	const static int16_t DATA = TYPE_VALUE('d', 'a');
#undef TYPE_VALUE

	Packet(){
		len = sizeof(uint16_t) + sizeof(uint32_t);
	}
	
	int parse();
	
	void set_type(int16_t type){
		*((int16_t *)buf) = type;
	}

	void set_seq(uint32_t seq){
		*((uint32_t *)(buf + sizeof(uint16_t))) = seq;
	}
	
	void set_data(const void *data, int size){
		int offset = sizeof(uint16_t) + sizeof(uint32_t);
		memcpy(buf + offset, data, size);
		this->len = offset + size;
	}
	
	int16_t type(){
		return *((int16_t *)buf);
	}
	
	uint32_t seq(){
		return *((uint32_t *)(buf + sizeof(uint16_t)));
	}
	
	const char* data() const{
		int offset = sizeof(uint16_t) + sizeof(uint32_t);
		return buf + offset;
	}
	
	// bytes of data
	int size() const{
		int offset = sizeof(uint16_t) + sizeof(uint32_t);
		return len - offset;
	}

	const std::vector<Bytes>* params() const{
		return &params_;
	}
	
	std::string repr() const{
		return hexmem(buf, len);
	}

	//int set_params(Bytes &b1);
	//int set_params(Bytes &b1, Bytes &b2);
	//int set_params(Bytes &b1, Bytes &b2, Bytes &b3);
};

#endif
