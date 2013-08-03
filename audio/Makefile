OBJS = mixer.o channel.o

all: ${OBJS}
	ar -cru librtc-audio.a ${OBJS}

channel.o: channel.h channel.cpp chunk.h
	g++ ${CFLAGS} -c channel.cpp -I ../
mixer.o: mixer.h mixer.cpp chunk.h
	g++ ${CFLAGS} -c mixer.cpp -I ../

clean:
	rm -rf *.a *.o	
