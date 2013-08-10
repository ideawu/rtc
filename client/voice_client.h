#ifndef RTC_VOICE_CLIENT_H
#define RTC_VOICE_CLIENT_H

#include <string>

namespace rtc{

class VoiceClient
{
public:	
	static VoiceClient* create();

	virtual int init() = 0;
	
	virtual int connect(std::string host, int port) = 0;
	virtual int join_room(int room_id, std::string token) = 0;
	virtual int close() = 0;
	
	virtual int start_send() = 0;
	virtual int stop_send() = 0;
	virtual int start_recv() = 0;
	virtual int stop_recv() = 0;
	
	virtual int run() = 0;
};

};

#endif
