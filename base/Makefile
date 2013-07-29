OBJS = log.o net.o packet.o udplink.o

all: ${OBJS}
	ar -cru librtc-base.a ${OBJS}

log.o: log.h log.cpp
	g++ -c log.cpp
net.o: net.h net.cpp
	g++ -c net.cpp
packet.o: packet.h packet.cpp
	g++ -c packet.cpp
udplink.o: udplink.h udplink.cpp
	g++ -c udplink.cpp
	
