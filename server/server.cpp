#include "server.h"
#include "base/net.h"
#include "base/packet.h"
#include "base/udplink.h"
#include "base/log.h"
#include "client.h"

Server::Server(){
}

Server::~Server(){
}

int Server::proc(UdpLink *serv_link){
	Packet req;
	Address addr;

	int ret;
	ret = serv_link->recv(&req, &addr);
	log_debug("recv %d bytes from %s:%d, %s", ret, addr.host().c_str(), addr.port(), req.repr().c_str());

	ret = req.parse();
	if(ret == -1){
		log_debug("bad request");
		return 0;
	}
	const std::vector<Bytes> *params = req.params();
	std::vector<Bytes>::const_iterator it;
	for(it = params->begin(); it != params->end(); it++){
		const Bytes &b = *it;
		log_debug("%s", hexmem(b.data(), b.size()).c_str());
	}

	switch(req.type()){
		case Packet::JOIN:{
			return this->proc_join(serv_link, &req, &addr);
		}
		case Packet::QUIT:{
			break;
		}
		case Packet::DATA:{
			return this->proc_data(serv_link, &req, &addr);
		}
		default:{
			break;
		}
	}

	return 0;
}

int Server::proc_join(UdpLink *link, Packet *req, Address *addr){
	if(!room.has(*addr)){
		// TODO: do auth
		Client client;
		client.addr = *addr;
		room.join(*addr, client);
		log_debug("new client from %s:%d", addr->host().c_str(), addr->port());
	}else{
		log_debug("old client from %s:%d", addr->host().c_str(), addr->port());
	}
				
	Packet resp;
	resp.set_type(Packet::RESP);
	resp.set_seq(req->seq());
	resp.set_data("hello\0", 6);
	int ret = link->send(resp, addr);
	log_debug("send %d bytes", ret);
	if(ret <= 0){
		log_error("send error: %s", strerror(errno));
	}
}

int Server::proc_data(UdpLink *link, Packet *req, Address *addr){
	if(!room.has(*addr)){
		// deny
	}else{
		//req.set_seq(); // room.seq
		const ClientList *clients = room.clients();
		ClientList::const_iterator it;
		for(it = clients->begin(); it!=clients->end(); it++){
			const Client &client = (*it).second;
			int ret = link->send(*req, &client.addr);
			log_debug("send %d byte(s) to: %s:%d", ret, client.addr.host().c_str(), client.addr.port());
		}
	}
}
