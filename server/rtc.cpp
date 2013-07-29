#include <errno.h>
#include "base/net.h"
#include "base/udplink.h"
#include "base/log.h"
#include "server.h"

UdpLink *serv_link;

int serv_init(int argc, char **argv);
int serv_free();

int main(int argc, char **argv){
	serv_init(argc, argv);
	
	Server serv;
	// Fdevent fdes;
	
	while(1){
		serv.proc(serv_link);
	}
	
	serv_free();
	return 0;
}

int serv_init(int argc, char **argv){
	serv_link = UdpLink::server("0.0.0.0", 10210);
}

int serv_free(){
	delete serv_link;
	return 0;
}
