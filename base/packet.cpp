#include <stdlib.h>
#include "log.h"

#include "packet.h"

int Packet::parse(){
	this->params_.clear();
	
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
