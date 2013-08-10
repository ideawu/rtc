#ifndef RTC_VOICE_CLIENT_IMPL_H
#define RTC_VOICE_CLIENT_IMPL_H

#include "voice_client.h"

class VoiceClientImpl : public VoiceClient
{
private:
	friend class VoiceClient;
public:
	VoiceClientImpl();
	~VoiceClientImpl();
	
	virtual int connect(std::string host, int port) = 0;
	virtual int join_room(int room_id, std::string token) = 0;
	virtual int close() = 0;
	
	virtual int start_send() = 0;
	virtual int stop_send() = 0;
	virtual int start_recv() = 0;
	virtual int stop_recv() = 0;
};

#endif
