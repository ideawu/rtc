all:
	g++ av.cpp -o av.exe \
		-I .. \
		-I ../deps/sfml-2.0/include \
		-I ../deps/opus-1.0.3/include \
		-L ../deps/sfml-2.0 -lsfml-audio -lsfml-system \
		../deps/opus-1.0.3/libopus.a \
		../base/librtc-base.a
	g++ music_player.cpp -o music_player.exe \
		-I .. \
		-I ../deps/sfml-2.0/include \
		-I ../deps/opus-1.0.3/include \
		-L ../deps/sfml-2.0 -lsfml-audio -lsfml-system \
		../deps/opus-1.0.3/libopus.a \
		../base/librtc-base.a
	export DYLD_LIBRARY_PATH="../deps/sfml-2.0:../deps/sndfile.framework/Versions/A"

clean:
	rm -rf *.a *.o *.exe
