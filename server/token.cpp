#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "token.h"

TokenSvc::TokenSvc(){
	timer_ticks = 0;
	last_clean_ticks = 0;
	srand(time(NULL));
}

TokenSvc::~TokenSvc(){
}

int TokenSvc::tick(uint32_t g_ticks){
	this->timer_ticks = g_ticks;
	if(g_ticks - this->last_clean_ticks > 10000){
		this->last_clean_ticks = g_ticks;
		
		std::map<std::string, TokenItem>::iterator it;
		for(it = tokens.begin(); it != tokens.end(); /**/){
			TokenItem &item = it->second;
			if(item.expired_at < this->timer_ticks){
				tokens.erase(it++);
				continue;
			}
			it++;
		}
	}
	return 0;
}

std::string TokenSvc::create(int room_id, int ttl_sec){
	for(int i=0; i<100; i++){
		char buf[33];
	    int offset = 0;
	    while(1){
	        int r = rand();
	        int len = snprintf(buf + offset, sizeof(buf) - offset, "%08x", r);
	        if(len == -1){
	            return "";
	        }
	        offset += len;
	        if(offset >= sizeof(buf) - 1){
	            break;
	        }
	    }
	
		std::string token = std::string(buf, sizeof(buf) - 1);
		if(tokens.find(token) != tokens.end()){
			// it rarely happen
			continue;
		}
	
		TokenItem item;
		item.room_id = room_id;
		item.expired_at = timer_ticks + ttl_sec;
		tokens[token] = item;
	
		return token;
	}
	return "";
}

void TokenSvc::destroy(const std::string &token){
	tokens.erase(token);
}

// 返回 room_id
int TokenSvc::check_and_destroy(int room_id, const std::string &token){
	std::map<std::string, TokenItem>::iterator it;
	it = tokens.find(token);
	if(it == tokens.end()){
		return 0;
	}
	TokenItem &item = it->second;
	if(item.expired_at < this->timer_ticks){
		return 0;
	}
	tokens.erase(it);
	return item.room_id == room_id? 1 : 0;
}
