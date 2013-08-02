OBJS = room.o server.o

all: ${OBJS}
	g++ rtc.cpp -o rtc -I .. ../base/librtc-base.a ../audio/librtc-audio.a ${OBJS}

room.o: room.h room.cpp
	g++ -c room.cpp -I ..
server.o: server.h server.cpp
	g++ -c server.cpp -I ..

clean:
	rm -f rtc
	rm -rf *.a *.o	
