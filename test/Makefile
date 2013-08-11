include $(ROOT)/config.mk

all:
	$(CC) av.cpp -o av.exe \
		$(CFLAGS) \
		-I $(DIST_INC_DIR) \
		-framework ApplicationServices -framework AudioToolbox -framework CoreAudio \
		$(DIST_LIB_DIR)/librtc-base.a $(DIST_LIB_DIR)/librtc-multimedia.a
		
	$(CC) music_player.cpp -o music_player.exe \
		-I .. \
		-I ../deps/sfml-2.0/include \
		-I ../deps/opus-1.0.3/include \
		-L ../deps/sfml-2.0 -lsfml-audio -lsfml-system \
		../deps/opus-1.0.3/libopus.a \
		$(DIST_LIB_DIR)/librtc-base.a

	export DYLD_LIBRARY_PATH="./deps/sfml-2.0:./deps/sndfile.framework/Versions/A"
	export DYLD_LIBRARY_PATH="../deps/sfml-2.0:../deps/sndfile.framework/Versions/A"

