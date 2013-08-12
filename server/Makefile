OBJS = room.o server.o fde.o

all: ${OBJS}
	g++ $(CFLAGS) -c fde.cpp
	g++ ${CFLAGS} rtc.cpp -o rtc.exe -I .. ../dist/lib/librtc-base.a voice/*.o ${OBJS}

room.o: room.h room.cpp
	g++ ${CFLAGS} -c room.cpp -I ..
server.o: server.h server.cpp
	g++ ${CFLAGS} -c server.cpp -I ..

clean:
	rm -rf *.a *.o *.exe
