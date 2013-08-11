#ifndef AVE_AUDIO_DEVICE_IMPL_H
#define AVE_AUDIO_DEVICE_IMPL_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "webrtc/modules/audio_device/include/audio_device.h"
#include "webrtc/common_audio/resampler/include/resampler.h"
#include "webrtc/common_audio/vad/include/webrtc_vad.h"
#include "audio_device.h"
#include "chunk_buffer.h"


namespace ave{

class AudioDeviceImpl : public AudioDevice
	, public webrtc::AudioTransport
{
private:
	friend class AudioDevice;

	// 2 channels
	static const int MAX_SAMPLES_PER_10MS = 2 * 48000/100;

	webrtc::Resampler *resampler_in;
	webrtc::Resampler *resampler_out;
	ChunkBuffer *input_buffer;
	ChunkBuffer *output_buffer;
	webrtc::AudioDeviceModule *audio;
	VadInst *vad;
	AudioDeviceCallback *callback;
	
	int input_sample_rate_;
	int output_sample_rate_;
	int output_channels_;
	
	int input_samples_per_10ms;
	int output_samples_per_10ms;
	
	int set_input_sample_rate(int hz);

	int set_output_sample_rate(int hz);
	int set_output_channels(int channels);

	AudioDeviceImpl();
	~AudioDeviceImpl();

public:
	// AudioDevice methods
	virtual int set_callback(AudioDeviceCallback *callback);
	
	virtual int init_input_device(int id, int sample_rate_hz);
	virtual int init_output_device(int id, int sample_rate_hz, int channels);
	
	virtual int start_record();
	virtual int stop_record();
	
	virtual int start_playout();
	virtual int stop_playout();

	//playout automatically startup
	virtual int play(const int16_t *samples, int size);
	
	//list_input_devices(std::vector<std::string> *devices);
	//list_output_devices(std::vector<std::string> *devices);

public:
	// webrtc::AudioTransport methods
	virtual int32_t RecordedDataIsAvailable(
		const void* audioSamples,
	    const uint32_t nSamples,
	    const uint8_t nBytesPerSample,
	    const uint8_t nChannels,
	    const uint32_t samplesPerSec,
	    const uint32_t totalDelayMS,
	    const int32_t clockDrift,
	    const uint32_t currentMicLevel,
	    const bool keyPressed,
	    uint32_t& newMicLevel);
		
	virtual int32_t NeedMorePlayData(
		const uint32_t nSamples,
		const uint8_t nBytesPerSample,
		const uint8_t nChannels,
		const uint32_t samplesPerSec,
		void* audioSamples,
		uint32_t& nSamplesOut);

};

}; // namespace ave

#endif
