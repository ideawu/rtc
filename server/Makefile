OBJS = fde.o room.o server.o token.o
CFLAGS = \
	-Wall -Wno-sign-compare -g
	

all: ${OBJS}
	g++ $(CFLAGS) rtc.cpp -o rtc.exe -I .. ../dist/lib/librtc-base.a voice/*.o ${OBJS}

fde.o: fde.h fde.cpp
	g++ $(CFLAGS) -c fde.cpp -I ..
room.o: room.h room.cpp
	g++ $(CFLAGS) -c room.cpp -I ..
token.o: token.h token.cpp
	g++ $(CFLAGS) -c token.cpp -I ..
server.o: server.h server.cpp
	g++ $(CFLAGS) -c server.cpp -I ..

clean:
	rm -rf *.a *.o *.exe
