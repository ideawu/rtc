#include "voice_client_impl.h"

#define SAMPLE_RATE			32000
#define SAMPLES_PER_10MS	(SAMPLE_RATE/100)

namespace rtc{


VoiceClient* VoiceClient::create(){
	VoiceClientImpl *impl = new VoiceClientImpl();
	return impl;
}

VoiceClientImpl::VoiceClientImpl(){
	audio = NULL;
	audio_callback = NULL;
	link = NULL;
	inited_ = false;
}

VoiceClientImpl::~VoiceClientImpl(){
}

int VoiceClientImpl::init(){
	int ret;
	
	if(!inited_){
		link = UdpLink::create();
		audio = ave::AudioDevice::create();

		ret = audio->init_input_device(0, SAMPLE_RATE);
		log_debug("%d", ret);
		ret = audio->init_output_device(0, SAMPLE_RATE, 1);
		log_debug("%d", ret);

		audio_callback = new AudioCallbackImpl(link);
		audio->set_callback(audio_callback);
	}
	inited_ = true;
	return 0;
}

int VoiceClientImpl::connect(std::string host, int port){
	link->connect(host, port);
	
	int bufsize = 5000;
	::setsockopt(link->fd(), SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));
	::setsockopt(link->fd(), SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));

	return 0;
}

int VoiceClientImpl::join_room(int room_id, std::string token){	
	int ret;
	char room_id_buf[32];
	snprintf(room_id_buf, sizeof(room_id_buf), "%d", room_id);
	
	Packet req;
	req.set_type(Packet::JOIN);
	req.set_seq(101);
	req.set_params(room_id_buf, token);
	
	ret = link->send(req);
	log_debug("send %d bytes", ret);
	if(ret <= 0){
		exit(0);
	}
	
	Packet resp;
	ret = link->recv(&resp);
	log_debug("recv %d bytes", ret);
	if(ret > 0){
		log_debug("recv: %s", resp.repr().c_str());
	}else{
		exit(0);
	}

	return 0;
}

int VoiceClientImpl::close(){
	return 0;
}
	
int VoiceClientImpl::start_send(){
	return audio->start_record();
}

int VoiceClientImpl::stop_send(){
	return 0;
}

int VoiceClientImpl::start_recv(){
	return audio->start_playout();
}

int VoiceClientImpl::stop_recv(){
	return 0;
}

int VoiceClientImpl::run(){
	while(1){
		int ret;
		Packet resp;
		ret = link->recv(&resp);
		log_debug("recv %d", ret);
		if(ret <= 0){
			exit(0);
		}
			
		const char *raw = resp.data();
		int nbytes = resp.size();

		// decode
		if(nbytes > 0){
			const int16_t *samples = (const int16_t *)raw;
			int num_samples = nbytes/sizeof(int16_t);
			ret = audio->play(samples, num_samples);
			if(ret == -1){
				log_error("error");
				exit(0);
			}
		}
	}
}



int AudioCallbackImpl::input(const int16_t *samples, int size){
	seq ++;
	if(size <= 0){
		return 0;
	}
		
	// TODO: encode
		
	int enc_bytes = size * sizeof(int16_t);

	Packet req;
	req.set_type(Packet::PUB);
	req.set_seq(seq);
	req.set_params(Bytes("0", 1), Bytes(samples, enc_bytes));
	int ret = link->send(req);
	log_debug("send %d", ret);
	if(ret <= 0){
		exit(0);
	}

	return 0;
}


}; // namespace
