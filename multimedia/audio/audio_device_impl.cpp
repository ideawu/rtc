#include <string.h>
#include "audio_device_impl.h"
#include "base/log.h"

namespace ave{

AudioDeviceImpl::AudioDeviceImpl(){
	resampler_in = NULL;
	resampler_out = NULL;
	input_buffer = NULL;
	output_buffer = NULL;
	vad = NULL;
	audio = NULL;
	
	callback = NULL;
	
	input_sample_rate_ = 0;
	output_sample_rate_ = 0;
	output_channels_ = 0;
}

AudioDeviceImpl::~AudioDeviceImpl(){
	if(resampler_in){
		delete resampler_in;
	}
	if(resampler_out){
		delete resampler_out;
	}
	if(input_buffer){
		delete input_buffer;
	}
	if(output_buffer){
		delete output_buffer;
	}
	if(audio){
		audio->Terminate();
	}
	if(vad){
		WebRtcVad_Free(vad);
	}
}

AudioDevice* AudioDevice::create(){
	int ret;
	AudioDeviceImpl *impl = new AudioDeviceImpl();
	
	impl->audio = webrtc::CreateAudioDeviceModule(0, webrtc::AudioDeviceModule::kPlatformDefaultAudio);
	if(!impl->audio){
		goto err;
	}
	ret = impl->audio->Init();
	if(ret == -1){
		goto err;
	}
	
	WebRtcVad_Create(&impl->vad);
	if(!impl->vad){
		goto err;
	}
	ret = WebRtcVad_Init(impl->vad);
	if(ret == -1){
		goto err;
	}

	{	
		uint32_t device_rate = 0;
		ret = impl->audio->RecordingSampleRate(&device_rate);
		if(ret == -1){
			goto err;
		}
		impl->set_input_sample_rate(device_rate);
	}

	{
		uint32_t device_rate = 0;
		ret = impl->audio->PlayoutSampleRate(&device_rate);
		if(ret == -1){
			goto err;
		}
		impl->set_output_sample_rate(device_rate);
	}
	ret = impl->audio->RegisterAudioCallback(impl);
	if(ret == -1){
		goto err;
	}

	return impl;
err:
	delete impl;
	return NULL;
}

int AudioDeviceImpl::set_callback(AudioDeviceCallback *callback){
	this->callback = callback;
	return 0;
}

int AudioDeviceImpl::set_input_sample_rate(int hz){
	this->input_sample_rate_ = hz;
	input_samples_per_10ms = hz/100;
	return 0;
}

int AudioDeviceImpl::set_output_sample_rate(int hz){
	this->output_sample_rate_ = hz;
	output_samples_per_10ms = hz/100;
	return 0;
}

int AudioDeviceImpl::set_output_channels(int channels){
	this->output_channels_ = channels;
	return 0;
}

int AudioDeviceImpl::init_input_device(int id, int sample_rate_hz){
	this->set_input_sample_rate(sample_rate_hz);
	
	int ret;
	ret = audio->SetRecordingDevice(id);
	if(ret == -1){
		return -1;
	}
	ret = audio->SetRecordingSampleRate(input_sample_rate_);
	if(ret == -1 || 1){
		uint32_t device_rate = 0;
		ret = audio->RecordingSampleRate(&device_rate);
		if(ret == -1){
			return -1;
		}
		if(resampler_in){
			delete resampler_in;
			resampler_in = NULL;
		}
		webrtc::ResamplerType type;
		type = webrtc::kResamplerSynchronous;
		resampler_in = new webrtc::Resampler(device_rate, input_sample_rate_, type);
		log_trace("input resampler: %d=>%d", device_rate, input_sample_rate_);
	}
	ret = audio->InitRecording();
	if(ret == -1){
		return -1;
	}
	
	if(input_buffer){
		delete input_buffer;
	}
	input_buffer = new ChunkBuffer(10, input_samples_per_10ms * sizeof(int16_t));
	
	return 0;
}

int AudioDeviceImpl::init_output_device(int id, int sample_rate_hz, int channels){
	this->set_output_sample_rate(sample_rate_hz);
	this->set_output_channels(channels);
	
	int ret;
	ret = audio->SetPlayoutDevice(id);
	if(ret == -1){
		return -1;
	}
	ret = audio->SetPlayoutSampleRate(output_sample_rate_);
	if(ret == -1 || 1){
		uint32_t device_rate = 0;
		ret = audio->PlayoutSampleRate(&device_rate);
		if(ret == -1){
			return -1;
		}
		if(resampler_out){
			delete resampler_out;
			resampler_out = NULL;
		}
		webrtc::ResamplerType type;
		if(channels == 1){
			type = webrtc::kResamplerSynchronous;
		}else{
			type = webrtc::kResamplerSynchronousStereo;
		}
		resampler_out = new webrtc::Resampler(output_sample_rate_, device_rate, type);
		log_trace("output resampler: %d=>%d", output_sample_rate_, device_rate);
	}
	ret = audio->InitPlayout();
	if(ret == -1){
		return -1;
	}
	
	if(output_buffer){
		delete output_buffer;
	}
	output_buffer = new ChunkBuffer(10, output_samples_per_10ms * sizeof(int16_t));

	return 0;
}

int AudioDeviceImpl::start_record(){
	return audio->StartRecording();
}

int AudioDeviceImpl::stop_record(){
	return audio->StopRecording();
}
	
int AudioDeviceImpl::start_playout(){
	return audio->StartPlayout();
}

int AudioDeviceImpl::stop_playout(){
	return audio->StopPlayout();
}

int AudioDeviceImpl::play(const int16_t *samples, int size){
	if(size != this->output_samples_per_10ms){
		return -1;
	}
	
	int16_t *out = (int16_t *)output_buffer->space();
	if(out == NULL){
		log_trace("drop oldest recorded frame");
		output_buffer->pop_front();
		out = (int16_t *)output_buffer->space();
		assert(out);
	}
	int nbytes = size * sizeof(int16_t) * this->output_channels_;
	//log_trace("play samples: %d, nbytes: %d", size, nbytes);
	memcpy(out, samples, nbytes);
	// TODO: lock
	output_buffer->push_back();
	
	if(!audio->Playing()){
		log_trace("start playout");
		audio->StartPlayout();
	}
	return 0;
}

int32_t AudioDeviceImpl::RecordedDataIsAvailable(
	const void* audioSamples,
    const uint32_t nSamples,
    const uint8_t nBytesPerSample,
    const uint8_t nChannels,
    const uint32_t samplesPerSec,
    const uint32_t totalDelayMS,
    const int32_t clockDrift,
    const uint32_t currentMicLevel,
    const bool keyPressed,
    uint32_t& newMicLevel)
{
	int ret;
	ret = WebRtcVad_Process(vad, samplesPerSec,
		(int16_t*)audioSamples, nSamples);
	if(ret == 0){
		if(callback){
			callback->input(NULL, 0);
		}
		return 0;
	}
	if(nBytesPerSample != sizeof(int16_t)){
		log_error("invalid nBytesPerSample: %d", nBytesPerSample);
		return -1;
	}
	if(nChannels != 1){
		log_error("invalid nChannels: %d", nChannels);
		return -1;
	}
	
	const int16_t *samples;
	int16_t tmp[MAX_SAMPLES_PER_10MS];
	if(!resampler_in){
		samples = (const int16_t*)audioSamples;
		if(nSamples != this->input_samples_per_10ms){
			log_error("invalid nSamples: %d", nSamples);
			return -1;
		}
	}else{
		int out_len = 0;
		int ret = resampler_in->Push((const int16_t*)audioSamples,
			nSamples,
			tmp,
	     	MAX_SAMPLES_PER_10MS,
			out_len);
		if(ret != 0 || out_len != this->input_samples_per_10ms){
			log_error("resample error!");
			return -1;
		}
		samples = tmp;
	}
	if(callback){
		callback->input(samples, this->input_samples_per_10ms);
	}

	return 0;
}
		
int32_t AudioDeviceImpl::NeedMorePlayData(
	const uint32_t nSamples,
	const uint8_t nBytesPerSample,
	const uint8_t nChannels,
	const uint32_t samplesPerSec,
	void* audioSamples,
	uint32_t& nSamplesOut)
{
	// TODO: lock
	int16_t *samples = (int16_t *)output_buffer->pop_front();
	if(samples == NULL){
		// silence
		//log_trace("no data for playout");
		memset(audioSamples, 0, nSamples * nBytesPerSample);
		nSamplesOut = nSamples;
		return -1;
	}
	
	int num_samples = 0;
	int16_t tmp[MAX_SAMPLES_PER_10MS];
	if(!resampler_out){
		num_samples = output_samples_per_10ms;
	}else{
		int ret = resampler_out->Push(samples,
			output_samples_per_10ms,
			tmp,
	     	MAX_SAMPLES_PER_10MS,
			num_samples);
		if(ret != 0){
			log_error("resample error!");
			return -1;
		}
		samples = tmp;
	}
	
	if(this->output_channels_ == nChannels){
		int nbytes = num_samples * sizeof(int16_t) * this->output_channels_;
		memcpy(audioSamples, samples, nbytes);
		nSamplesOut = num_samples;
	}else if(this->output_channels_ == 1 && nChannels == 2){
		// mono => stereo
		int16_t *dst = (int16_t *)audioSamples;
		int16_t *src = samples;
        for(int i = 0; i < num_samples; i++){
           dst[0] = *src;
            dst[1] = *src;
            dst += 2;
            src ++;
		}
		nSamplesOut = num_samples;
	}else if(this->output_channels_ == 2 && nChannels == 1){
		// stereo => mono
		int16_t *dst = (int16_t *)audioSamples;
		int16_t *src = samples;
        for(int i = 0; i < num_samples; i++){
            *dst = (src[0] + src[1])/2;
            dst ++;
            src += 2;
        }
		nSamplesOut = num_samples;
	}else{
		log_error("invalid nChannels: %d and output_channels: %d", nChannels, output_channels_);
		nSamplesOut = nSamples;
		return -1;
	}
	
	return 0;
}

}; // namespace ave

