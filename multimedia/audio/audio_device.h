#ifndef AVE_AUDIO_DEVICE_H
#define AVE_AUDIO_DEVICE_H

#include<inttypes.h>

namespace ave{

class AudioDeviceCallback
{
public:
	// if Non-active Voice detected, size is set to zero
	virtual int input(const int16_t *samples, int size) = 0;
};


class AudioDevice
{
public:
	// AudioDevice methods
	static AudioDevice* create();
	
	virtual int set_callback(AudioDeviceCallback *callback) = 0;
	
	virtual int init_input_device(int id, int sample_rate_hz) = 0;
	virtual int init_output_device(int id, int sample_rate_hz, int channels) = 0;
	
	virtual int start_record() = 0;
	virtual int stop_record() = 0;
	
	virtual int start_playout() = 0;
	virtual int stop_playout() = 0;

	//playout automatically startup
	virtual int play(const int16_t *samples, int size) = 0;
};

}; // namespace ave

#endif
