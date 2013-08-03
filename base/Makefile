OBJS = log.o net.o packet.o udplink.o tcplink.o fde.o

all: ${OBJS}
	ar -cru librtc-base.a ${OBJS}

log.o: log.h log.cpp
	g++ ${CFLAGS} -c log.cpp
net.o: net.h net.cpp
	g++ ${CFLAGS} -c net.cpp
packet.o: packet.h packet.cpp
	g++ ${CFLAGS} -c packet.cpp
udplink.o: udplink.h udplink.cpp
	g++ ${CFLAGS} -c udplink.cpp
tcplink.o: tcplink.h tcplink.cpp
	g++ ${CFLAGS} -c tcplink.cpp
fde.o: fde.h fde.cpp
	g++ ${CFLAGS} -c fde.cpp

clean:
	rm -rf *.a *.o	
