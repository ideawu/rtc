#include <errno.h>
#include "base/net.h"
#include "base/udplink.h"
#include "base/log.h"
#include "base/fde.h"
#include "server.h"

#define TICK_INTERVAL 10 // ms

UdpLink *serv_link;
volatile bool quit = false;
volatile uint32_t timer_ticks = 0;

int serv_init(int argc, char **argv);
int serv_free();

void signal_handler(int sig){
	switch(sig){
		case SIGTERM:
		case SIGINT:{
			quit = true;
			break;
		}
		case SIGALRM:{
			timer_ticks ++;
			break;
		}
	}
}

#define FRONT_LISTEN_LINK	0
#define FRONT_CLIENT_LINK	1
#define ADMIN_LISTEN_LINK	2
#define ADMIN_CLIENT_LINK	3

int main(int argc, char **argv){
	serv_init(argc, argv);
	
	Server serv;
	Fdevents fdes;
	const Fdevents::events_t *events;

	fdes.set(serv_link->fd(), FDEVENT_IN, FRONT_LISTEN_LINK, serv_link);
	
	uint32_t last_timer_ticks = timer_ticks;
	while(!quit){
		events = fdes.wait(TICK_INTERVAL);
		if(events == NULL){
			log_fatal("events.wait error: %s", strerror(errno));
			break;
		}
		
		uint32_t curr_ticks = timer_ticks;
		if(last_timer_ticks < curr_ticks){
			uint32_t elapsed_ticks = (uint32_t)(curr_ticks - last_timer_ticks);
			last_timer_ticks = curr_ticks;
			if(elapsed_ticks > 5 * 1000/TICK_INTERVAL){ // 5 seconds
				// something blocks too long
				elapsed_ticks = 0;
			}else{
				for(int i=0; i<elapsed_ticks; i++){
					serv.tick();
				}
			}
		}
		
		for(int i=0; i<(int)events->size(); i++){
			const Fdevent *fde = events->at(i);
			int type = fde->data.num;
			UdpLink *link = (UdpLink *)fde->data.ptr;
			
			if(type == FRONT_LISTEN_LINK){
				UdpLink *new_link = serv.proc_listen_link(link, &fdes);
				if(new_link){
					fdes.set(new_link->fd(), FDEVENT_IN, FRONT_CLIENT_LINK, new_link);
				}
			}else if(type == FRONT_CLIENT_LINK){
				int ret = serv.proc_client_link(link, &fdes);
				if(ret == -1){
					fdes.del(link->fd());
					log_debug("close link: %d", link->fd());
					delete link;
				}
			}else if(type == ADMIN_LISTEN_LINK){
				//
			}else if(type == ADMIN_CLIENT_LINK){
				//
			}
		}
	}
	
	serv_free();
	return 0;
}

int serv_init(int argc, char **argv){
	serv_link = UdpLink::server("0.0.0.0", 10210);

	signal(SIGPIPE, SIG_IGN);
	signal(SIGALRM, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	
	struct itimerval tv;
	tv.it_interval.tv_sec = 0;
	tv.it_interval.tv_usec = TICK_INTERVAL * 1000;
	tv.it_value.tv_sec = 0;
	tv.it_value.tv_usec = 1;
	setitimer(ITIMER_REAL, &tv, NULL); // not accurate

	return 0;
}

int serv_free(){
	delete serv_link;
	return 0;
}
