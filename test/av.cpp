#include <SFML/Audio.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <opus.h>

#include "base/log.h"
#include "base/net.h"
#include "base/packet.h"
#include "base/udplink.h"

#define SAMPLE_RATE 48000

class NetworkRecorder : public sf::SoundRecorder
{
private:
	UdpLink *link;
    OpusEncoder *enc;
	
	std::vector<sf::Int16> m_samples;

public:
	NetworkRecorder(){
		link = NULL;
		
		int error;
		int channels = 1;
		//OPUS_APPLICATION_VOIP
		//OPUS_APPLICATION_AUDIO 
		enc = opus_encoder_create(SAMPLE_RATE, channels, OPUS_APPLICATION_AUDIO, &error);
		if(enc == NULL){
			log_error("opus error!");
		}
	}
	
	~NetworkRecorder(){
		opus_encoder_destroy(enc);
	}

	int connect(std::string host, short port){
		int ret;
		link = UdpLink::client(host, port);
		
		Packet req;
		req.set_type(Packet::JOIN);
		req.set_seq(101);
		
		ret = link->send(req);
		log_debug("send %d bytes", ret);
	
		Packet resp;
		ret = link->recv(&resp);
		log_debug("recv %d bytes", ret);
		if(ret > 0){
			log_debug("recv type: %d, seq: %u, data: %s", resp.type(), resp.seq(), resp.data());
		}

		return 0;
	}

private:

    virtual bool onProcessSamples(const sf::Int16 *samples, std::size_t count){
		std::copy(samples, samples + count, std::back_inserter(m_samples));
		
		int frame_size = SAMPLE_RATE/100 * 4; // max 480 * 6
		while(m_samples.size() >= frame_size){
			const sf::Int16 *src = &m_samples[0];
			unsigned char buf[16 * 1024];
			int enc_bytes = opus_encode(enc, (opus_int16 *)src, frame_size, buf, sizeof(buf));
			log_debug("raw: %d, encoded: %d", frame_size*sizeof(sf::Int16), enc_bytes);
		
			if(enc_bytes > 0){
				Packet req;
				req.set_type(Packet::DATA);
				req.set_data(buf, enc_bytes);
				link->send(req);
			}
			
			m_samples.erase(m_samples.begin(), m_samples.begin() + frame_size - 1);
		}
		return true;
    }
};


class NetworkPlayer : public sf::SoundStream
{
private:
	UdpLink *link;
    OpusDecoder *dec;

    sf::Mutex              m_mutex;
    std::vector<sf::Int16> m_samples;
    std::vector<sf::Int16> m_tempBuffer;
    std::size_t            m_offset;
    bool                   m_hasFinished;
	
public:
	NetworkPlayer(){
		initialize(1, SAMPLE_RATE);
		
		int error;
		int channels = 1;
		dec = opus_decoder_create(SAMPLE_RATE, channels, &error);
		if(dec == NULL){
			log_error("opus error!");
		}
	}

	int connect(std::string host, short port){
		int ret;
		link = UdpLink::client(host, port);
		
		Packet req;
		req.set_type(Packet::JOIN);
		req.set_seq(101);
		
		ret = link->send(req);
		log_debug("send %d bytes", ret);
	
		Packet resp;
		ret = link->recv(&resp);
		log_debug("recv %d bytes", ret);
		if(ret > 0){
			log_debug("recv type: %d, seq: %u, data: %s", resp.type(), resp.seq(), resp.data());
		}

		return 0;
		
		// set smaller recvbuf for aggressive drop
		//::setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, sizeof(recvbuf));
	}
	
	void start(){
		play();
		
		opus_int16 pcm[8 * 1024];
		while(1){
			Packet resp;
			link->recv(&resp);
			
			unsigned char *raw = (unsigned char *)resp.data();
			int raw_bytes = resp.size();
			
			bool to_drop = false;
			{
			    sf::Lock lock(m_mutex);
				if(m_samples.size() > SAMPLE_RATE/10){ // 100ms
					to_drop = true;
				}
			}
			if(to_drop){
				if(opus_decode(dec, NULL, 0, NULL, 0, 0)){
					//
				}
				log_debug("long delay, drop packet!!!!!! %d", m_samples.size());
				continue;
			}
			
			int sampleCount = opus_decode(dec, raw, raw_bytes, pcm, sizeof(pcm), 0);
			log_debug("raw: %d, decoded: %d", raw_bytes, sampleCount*sizeof(opus_int16));
			
            const sf::Int16* samples = (const sf::Int16 *)pcm;
			{
			    sf::Lock lock(m_mutex);
			    std::copy(samples, samples + sampleCount, std::back_inserter(m_samples));
			}
		}
	}

    virtual bool onGetData(sf::SoundStream::Chunk& data){
        // No new data has arrived since last update : wait until we get some
        while (m_offset >= m_samples.size()){
            sf::sleep(sf::milliseconds(10));
		}

        // Copy samples into a local buffer to avoid synchronization problems
        // (don't forget that we run in two separate threads)
        {
            sf::Lock lock(m_mutex);
            m_tempBuffer.assign(m_samples.begin(), m_samples.end());
			m_samples.clear();
        }

        // Fill audio data to pass to the stream
        data.samples     = &m_tempBuffer[0];
        data.sampleCount = m_tempBuffer.size();

        // Update the playing offset
        //m_offset += m_tempBuffer.size();

        return true;
    }

    virtual void onSeek(sf::Time timeOffset){
		return;
        //m_offset = timeOffset.asMilliseconds() * getSampleRate() * getChannelCount() / 1000;
    }
};


int main(int argc, char **argv){
	if(argc > 1){
		log_debug("run as speaker");
		
		NetworkRecorder recorder;
		recorder.connect("127.0.0.1", 10210);
	
		recorder.start(SAMPLE_RATE);
		getchar();
		recorder.stop();
	}else{
		log_debug("run as player");

		NetworkPlayer player;
		player.connect("127.0.0.1", 10210);
		player.start();
	}
	
	return 0;
}
