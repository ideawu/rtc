#ifndef RTC_VOICE_CLIENT_IMPL_H
#define RTC_VOICE_CLIENT_IMPL_H

#include "ave/audio_device.h"
#include "base/log.h"
#include "base/net.h"
#include "base/packet.h"
#include "base/udplink.h"

#include "voice_client.h"

// TODO: multithread-lock

namespace rtc{

class AudioCallbackImpl : public ave::AudioDeviceCallback
{
private:
	uint16_t seq;
	UdpLink *link;
public:
	AudioCallbackImpl(UdpLink *link){
		seq = 0;
		this->link = link;
	}
	int input(const int16_t *samples, int size);
};


class VoiceClientImpl : public VoiceClient
{
private:
	friend class VoiceClient;
	
	UdpLink *link;

	ave::AudioDevice *audio;
	AudioCallbackImpl *audio_callback;
	
	bool inited_;
	int init();
	
public:
	VoiceClientImpl();
	~VoiceClientImpl();
	
	virtual int connect(std::string host, int port);
	virtual int join_room(int room_id, std::string token);
	virtual int close();
	
	virtual int start_send();
	virtual int stop_send();
	virtual int start_recv();
	virtual int stop_recv();

	virtual int run();
};


};
#endif
