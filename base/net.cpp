#include "net.h"
#include <stdlib.h>

Address::Address(){
	bzero(&sock_addr, sizeof(sock_addr));
}

Address::~Address(){
}


std::string Address::host() const{
	char buf[INET_ADDRSTRLEN];
	if(inet_ntop(AF_INET, &(sock_addr.sin_addr), buf, INET_ADDRSTRLEN) != NULL){
		return buf;
	}else{
		return "";
	}
}

int Address::port() const{
	return ntohs(sock_addr.sin_port);;
}

/*
void Address::set(const std::string host, int port){
	bzero(&sock_addr, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons((short)port);
	inet_pton(AF_INET, host.c_str(), &sock_addr.sin_addr);
	this->host = host;
	this->port = port;
}

void Address::set(const struct sockaddr_in *addr){
	bzero(&sock_addr, sizeof(sock_addr));
	this->sock_addr = *addr;
	
	char buf[INET_ADDRSTRLEN];
	if(inet_ntop(AF_INET, &(sock_addr.sin_addr), buf, INET_ADDRSTRLEN) != NULL){
		this->host = host;
	}
	this->port = ntohs(sock_addr.sin_port);
}
*/
