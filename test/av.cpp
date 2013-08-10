#include <inttypes.h>
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

#include "base/log.h"
#include "base/net.h"
#include "base/packet.h"
#include "base/udplink.h"
#include "voice_client.h"

int main(int argc, char **argv){
	int ret;
	
	rtc::VoiceClient *client = rtc::VoiceClient::create();
	ret = client->connect("127.0.0.1", 10210);
	assert(ret == 0);
	ret = client->join_room(0, "token");
	assert(ret == 0);
	
	ret = client->start_send();
	assert(ret == 0);
	ret = client->start_recv();
	assert(ret == 0);

	client->run();
	
	//sleep(2);
	//getchar();
	return 0;
}

