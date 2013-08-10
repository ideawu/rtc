#include "common.h"
#include "ave/audio_device.h"

#define SAMPLE_RATE			32000
#define SAMPLES_PER_10MS	(SAMPLE_RATE/100)

ave::AudioDevice *audio;

class Callback : public ave::AudioDeviceCallback
{
private:
	uint16_t seq;
	UdpLink *link;
public:
	Callback(UdpLink *link){
		seq = 0;
		this->link = link;
	}
	
	int input(const int16_t *samples, int size){
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
};

int main(int argc, char **argv){
	int ret;


	UdpLink *link = link_connect("127.0.0.1", 10210);
	if(!link){
		log_error("");
		exit(0);
	}

	audio = ave::AudioDevice::create();

	Callback callback(link);
	audio->set_callback(&callback);

	ret = audio->init_input_device(0, SAMPLE_RATE);
	log_debug("%d", ret);
	ret = audio->init_output_device(0, SAMPLE_RATE, 1);
	log_debug("%d", ret);
	
	//ret = audio->start_playout();
	//log_debug("%d", ret);
	ret = audio->start_record();
	log_debug("%d", ret);
	
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
	
	
	//sleep(2);
	//getchar();
	return 0;
}

