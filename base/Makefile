OBJS = log.o net.o packet.o udplink.o tcplink.o fde.o

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
tcplink.o: tcplink.h tcplink.cpp
	g++ -c tcplink.cpp
fde.o: fde.h fde.cpp
	g++ -c fde.cpp

clean:
	rm -rf *.a *.o	
