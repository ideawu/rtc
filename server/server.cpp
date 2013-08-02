#include "server.h"
#include "base/net.h"
#include "base/packet.h"
#include "base/udplink.h"
#include "base/log.h"
#include "client.h"

Server::Server(){
	// TEST:
	Room *room = rooms.alloc();
	log_debug("TEST: alloc room: %d", room->id);
}

Server::~Server(){
}

UdpLink* Server::proc_listen_link(UdpLink *listen_link, Fdevents *fdes){
	Packet req;
	Address addr;

	int ret;
	ret = listen_link->recv(&req, &addr);
	log_debug("recv %d bytes from %s, %s", ret, addr.repr().c_str(), req.repr().c_str());

	ret = req.parse();
	if(ret == -1){
		log_debug("bad request");
		return NULL;
	}
	const std::vector<Bytes> *params = req.params();
	//log_debug("%s", serialize_params(*params).c_str());

	switch(req.type()){
		// TODO: OPEN is only allowed in admin_client_link
		case Packet::OPEN:{
			int ret = this->proc_open(listen_link, &req, &addr);
			break;
		}
		case Packet::JOIN:{
			Client *client = this->proc_join(listen_link, &req, &addr);
			if(client){
				return client->link;
			}
			break;
		}
		default:{
			log_debug("unknow command: %d", req.type());
			break;
		}
	}

	return NULL;
}

int Server::proc_client_link(UdpLink *link, Fdevents *fdes){
	Packet req;
	Address addr;
	Client *client = (Client *)link->data;
	
	int ret = link->recv(&req, &addr);
	if(ret <= 0){
		log_error("recv %d from %s, close.", ret, client->addr.repr().c_str());
		client->room->quit(client);
		client->room = NULL;
		client->link = NULL;
		return -1;
	}
	log_debug("recv %d bytes from %s, %s", ret, client->addr.repr().c_str(), req.repr().c_str());

	ret = req.parse();
	if(ret == -1){
		log_debug("bad request");
		return 0;
	}
	const std::vector<Bytes> *params = req.params();
	//log_debug("%s", serialize_params(*params).c_str());
	
	switch(req.type()){
		case Packet::DATA:{
			int ret = this->proc_data(client, &req);
			break;
		}
		case Packet::JOIN:{
			// simply send response
			Packet resp;
			resp.set_type(Packet::RESP);
			resp.set_seq(req.seq());
			resp.set_params("ok", "already joined\n");
			int ret = link->send(resp);
			if(ret <= 0){
				log_error("send error: %s", strerror(errno));
			}
			break;
		}
		default:{
			log_debug("unknow command: %d", req.type());
			break;
		}
	}
	
	return 0;
}

// TODO: TcpLink
int Server::proc_open(UdpLink *link, Packet *req, Address *addr){
	Room *room = rooms.alloc();
	log_debug("open room %d", room->id);
	
	char buf[16];
	snprintf(buf, sizeof(buf), "%d", room->id);
	
	Packet resp;
	resp.set_type(Packet::RESP);
	resp.set_seq(req->seq());
	resp.set_params("ok", buf);
	int ret = link->send(resp, addr);
	if(ret <= 0){
		log_error("send error: %s", strerror(errno));
	}
	return 0;
}

Client* Server::proc_join(UdpLink *serv_link, Packet *req, Address *addr){
	const std::vector<Bytes> *params = req->params();
	if(params->size() < 2){
		log_error("bad format");
		return NULL;
	}
	int room_id = params->at(0).Int();
	std::string token = params->at(1).String();
	
	log_debug("%s join room %d token %s", addr->repr().c_str(), room_id, token.c_str());
	
	Room *room = rooms.get(room_id);
	if(!room){
		log_error("room %d not exists!", room_id);
		Packet resp;
		resp.set_type(Packet::RESP);
		resp.set_seq(req->seq());
		resp.set_params("fail", "room_id and token do not match!\n");
		int ret = serv_link->send(resp, addr);
		if(ret <= 0){
			log_error("send error: %s", strerror(errno));
		}
		return NULL;
	}
	
	// TODO: do auth
	// room->check_token();
	
	UdpLink *link = UdpLink::server("127.0.0.1", 10210);
	if(link == NULL){
		log_error("error: %s", strerror(errno));
		return NULL;
	}
	if(link->connect(*addr) == -1){
		log_error("error: %s", strerror(errno));
		return NULL;
	}

	Client *client = clients.alloc();
	client->addr = *addr;
	client->link = link;
	client->room = room;
	link->data = client;
				
	Packet resp;
	resp.set_type(Packet::RESP);
	resp.set_seq(req->seq());
	resp.set_params("ok", "joined!\n");
	int ret = client->link->send(resp);
	if(ret <= 0){
		log_error("send error: %s", strerror(errno));
	}

	log_debug("client %s, joined room: %d", addr->repr().c_str(), room_id);
	room->join(client);

	return client;
}

int Server::proc_data(Client *client, Packet *req){
	Room *room = client->room;
	room->publish(client, req);
	return 0;
}
