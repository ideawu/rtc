#include <stdlib.h>
#include "log.h"

#include "packet.h"

int Packet::parse(){
	parsed = true;
	this->params_.clear();
	if(this->len < HEADER_LEN){
		return -1;
	}
	
	int size = this->size();
	char *head = (char *)this->data();

	while(size > 0){
		if(head[0] == ' ' || head[0] == '\r'){
			head ++;
			size --;
			continue;
		}else if(head[0] == '\n'){
			break;
		}
		char *body = (char *)memchr(head, ' ', size);
		if(body == NULL){
			return -1;
		}
		body ++;

		int head_len = body - head;
		if(head[0] < '0' || head[0] > '9'){
			log_warn("bad format");
			return -1;
		}

		char head_str[20];
		if(head_len > (int)sizeof(head_str) - 1){
			return -1;
		}
		memcpy(head_str, head, head_len - 1); // no '\n'
		head_str[head_len - 1] = '\0';

		int body_len = atoi(head_str);
		if(body_len < 0){
			log_warn("bad format");
			return -1;
		}
		//log_debug("size: %d, head_len: %d, body_len: %d", size, head_len, body_len);
		size -= head_len + body_len;
		if(size < 0){
			return -1;
		}

		this->params_.push_back(Bytes(body, body_len));

		head += head_len + body_len;
		//if(parsed > MAX_PACKET_SIZE){
		//	 log_warn("fd: %d, exceed max packet size, parsed: %d", this->sock, parsed);
		//	 return -1;
		//}
	}
	return 1;
}

int Packet::append(const Bytes &b){
#define MAX_BLOCK_SIZE 10000
#define MAX_BLOCK_SIZE_STR_LEN 5
	if(MAX_BLOCK_SIZE_STR_LEN + 2 + b.size() + this->len > MAX_DATA_LEN){
		return -1;
	}
	char *p = this->buf() + this->len;
	int size_str_len = sprintf(p, " %d ", b.size());
	if(size_str_len < 0){
		return -1;
	}
	p += size_str_len;
	memcpy(p, b.data(), b.size());
	p += b.size();
	
	int ret = size_str_len + b.size();
	this->len += ret;
	return ret;
}

int Packet::set_params(const Bytes &b1){
	this->len = HEADER_LEN;
	int ret = 0;
	int tmp;
	tmp = this->append(b1);
	if(tmp == -1){
		return -1;
	}
	ret += tmp;
	
	// add a '\n' to let the packet print-friendly
	this->buf()[this->len++] = '\n';
	return ret;
}

int Packet::set_params(const Bytes &b1, const Bytes &b2){
	this->len = HEADER_LEN;
	int ret = 0;
	int tmp;
	tmp = this->append(b1);
	if(tmp == -1){
		return -1;
	}
	ret += tmp;
	tmp = this->append(b2);
	if(tmp == -1){
		return -1;
	}
	ret += tmp;
	
	// add a '\n' to let the packet print-friendly
	this->buf()[this->len++] = '\n';
	return ret;
}

int Packet::set_params(const Bytes &b1, const Bytes &b2, const Bytes &b3){
	this->len = HEADER_LEN;
	int ret = 0;
	int tmp;
	tmp = this->append(b1);
	if(tmp == -1){
		return -1;
	}
	ret += tmp;
	tmp = this->append(b2);
	if(tmp == -1){
		return -1;
	}
	ret += tmp;
	tmp = this->append(b3);
	if(tmp == -1){
		return -1;
	}
	ret += tmp;
	
	// add a '\n' to let the packet print-friendly
	this->buf()[this->len++] = '\n';
	return ret;
}
