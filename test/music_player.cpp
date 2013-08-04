#include "common.h"

#define SAMPLE_RATE 48000
// 40ms, allow 10, 20, 40, 60
#define PACKET_TIME 40
#define FRAME_SIZE (PACKET_TIME * SAMPLE_RATE/1000)

int main(int argc, char **argv){
	if(argc != 2){
		printf("Usage: %s music_file\n", argv[0]);
		exit(0);
	}
	std::string music_file = argv[1];
	
	sf::SoundBuffer sound;
	if(!sound.loadFromFile(music_file)){
		log_error("invalid music file");
		exit(0);
	}
	
	int chunk_size = FRAME_SIZE;
	int count = sound.getSampleCount();
	int rate = sound.getSampleRate();
	log_debug("count=%d rate=%d chunk_size=%d %f", count, rate, chunk_size, (float)count/rate);
	
	if(rate != SAMPLE_RATE){
		log_error("invalid sample rate %d, it should be %d", rate, SAMPLE_RATE);
		exit(0);
	}
	
	int sample_size;
	if(count % chunk_size == 0){
		sample_size = count;
	}else{
		sample_size = (count/chunk_size + 1) * chunk_size;
		log_debug("resize samples to %d", sample_size);
	}
	sf::Int16 *samples = (sf::Int16 *)malloc(sizeof(sf::Int16) * sample_size);
	memcpy(samples, (void *)sound.getSamples(), count * sizeof(sf::Int16));

	const sf::Int16 *data = samples;
	int seq = 65400;
	
	UdpLink *link = link_connect("127.0.0.1", 10210);
	if(!link){
		log_error("connect error");
		exit(0);
	}

	while(1){
		usleep(39 * 1000);
		if(data - samples >= count){
			data = samples;
		}
		
		Packet req;
		req.set_type(Packet::PUB);
		req.set_seq(seq ++);
		req.set_params(Bytes("0", 1), Bytes((char *)data, chunk_size * sizeof(sf::Int16)));

		int ret = link->send(req);
		log_debug("send %d", ret);
		if(ret <= 0){
			exit(0);
		}
		
		log_debug("offset: %d", data - samples);
		data += chunk_size;
	}
	
	return 0;
}
