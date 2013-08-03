#ifndef RTC_BASE_PACKET_H
#define RTC_BASE_PACKET_H

#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include <string>
#include "bytes.h"
#include "strings.h"

template<class T>
static std::string serialize_params(T &req);

class Packet
{
private:
	struct pkt_t{
		uint16_t type;
		uint16_t seq;
		char data[8192];
	}pkt;
	
	std::vector<Bytes> params_;
	bool parsed;
	
public:
	const static int MAX_LEN = sizeof(pkt_t);
	const static int HEADER_LEN = offsetof(pkt_t, data);
	const static int MAX_DATA_LEN = MAX_LEN - HEADER_LEN;
	int len;

#define TYPE_VALUE(a, b) ((a)+((b)<<8))
	// if big-endian ((a<<8)+(b))
	const static uint16_t RESP	= TYPE_VALUE('r', 'e');
	const static uint16_t OPEN	= TYPE_VALUE('o', 'p');
	const static uint16_t CLOSE	= TYPE_VALUE('c', 'l');
	const static uint16_t JOIN	= TYPE_VALUE('j', 'o');
	const static uint16_t QUIT	= TYPE_VALUE('q', 'u');
	const static uint16_t DATA	= TYPE_VALUE('d', 'a');
	const static uint16_t SUB	= TYPE_VALUE('s', 'u');
	const static uint16_t PUB	= TYPE_VALUE('p', 'u');
#undef TYPE_VALUE

	Packet(){
		len = HEADER_LEN;
		parsed = false;
	}
	
	char* buf() const{
		return (char *)&pkt;
	}
	
	int parse();
	
	void set_type(int16_t type){
		pkt.type = type;
	}

	void set_seq(uint16_t seq){
		pkt.seq = seq;
	}
	
	void set_data(const void *data, int size){
		int offset = HEADER_LEN;
		memcpy(pkt.data, data, size);
		this->len = offset + size;
	}
	
	uint16_t type(){
		return pkt.type;
	}
	
	uint16_t seq(){
		return pkt.seq;
	}
	
	const char* data() const{
		return pkt.data;
	}
	
	// bytes of data
	int size() const{
		int offset = HEADER_LEN;
		return len - offset;
	}

	const std::vector<Bytes>* params() const{
		return &params_;
	}
	
	std::string repr() const{
		std::string ret = hexmem(&pkt, HEADER_LEN);
		if(!parsed){
			((Packet *)this)->parse();
		}
		std::vector<Bytes>::const_iterator it;
		for(it=params_.begin(); it!=params_.end(); it++){
			const Bytes &b = *it;
			if(b.size() == 0){
				ret.append(" \"\"");
			}else if(b.size() > 32){
				char buf[32];
				snprintf(buf, sizeof(buf), " [%d]", b.size());
				ret.append(buf);
			}else{
				ret.append(" ");
				ret.append(hexmem(b.data(), b.size()).c_str());
			}
		}
		return ret;
	}

	int append(const Bytes &b);
	int set_params(const Bytes &b1);
	int set_params(const Bytes &b1, const Bytes &b2);
	int set_params(const Bytes &b1, const Bytes &b2, const Bytes &b3);
};


template<class T>
static std::string serialize_params(T &req){
	std::string ret;
	char buf[50];
	for(int i=0; i<req.size(); i++){
		if(i >= 5 && i < req.size() - 1){
			sprintf(buf, "[%d more...]", (int)req.size() - i);
			ret.append(buf);
			break;
		}
		if(req[i].size() < 30){
			if(req[i].size() == 0){
				ret.append("\"\"");
			}else{
				std::string h = hexmem(req[i].data(), req[i].size());
				ret.append(h);
			}
		}else{
			sprintf(buf, "[%d]", (int)req[i].size());
			ret.append(buf);
		}
		if(i < req.size() - 1){
			ret.append(" ");
		}
	}
	return ret;
}

#endif
