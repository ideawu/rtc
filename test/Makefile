include $(ROOT)/config.mk
LIBAVE_PATH = ../../libave/dist

all:
	$(CC) av.cpp -o av.exe \
		$(CFLAGS) \
		-I .. \
		-I ../client \
		-I $(LIBAVE_PATH)/include \
		-framework ApplicationServices -framework AudioToolbox -framework CoreAudio \
		$(LIBAVE_PATH)/lib/libave.a \
		../base/librtc-base.a \
		../client/*.o
	$(CC) music_player.cpp -o music_player.exe \
		-I .. \
		-I ../deps/sfml-2.0/include \
		-I ../deps/opus-1.0.3/include \
		-L ../deps/sfml-2.0 -lsfml-audio -lsfml-system \
		../deps/opus-1.0.3/libopus.a \
		../base/librtc-base.a
	export DYLD_LIBRARY_PATH="./deps/sfml-2.0:./deps/sndfile.framework/Versions/A"
	export DYLD_LIBRARY_PATH="../deps/sfml-2.0:../deps/sndfile.framework/Versions/A"

