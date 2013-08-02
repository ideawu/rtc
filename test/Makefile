all:
	g++ av.cpp -o av \
		-I .. \
		-I ../deps/sfml-2.0/include \
		-I ../deps/opus-1.0.3/include \
		-L ../deps/sfml-2.0 -lsfml-audio -lsfml-system \
		../deps/opus-1.0.3/libopus.a \
		../base/log.o \
		../base/net.o \
		../base/udplink.o
	#export DYLD_LIBRARY_PATH="../deps/sfml-2.0:../deps/sndfile.framework/Versions/A"
