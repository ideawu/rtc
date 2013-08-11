$(shell sh build.sh > config.mk)
include config.mk

ROOT = $(shell pwd)

all:
	cd webrtc/modules/audio_device; make ROOT=$(ROOT)
	cd webrtc/system_wrappers/source; make ROOT=$(ROOT)
	cd webrtc/common_audio/signal_processing; make ROOT=$(ROOT)
	cd webrtc/common_audio/vad; make ROOT=$(ROOT)
	cd webrtc/common_audio/resampler; make ROOT=$(ROOT)
	cd webrtc/modules/audio_processing/utility; make ROOT=$(ROOT)
	cd webrtc/modules/audio_processing/aec; make ROOT=$(ROOT)
	cd webrtc/modules/audio_processing/agc; make ROOT=$(ROOT)
	cd webrtc/modules/audio_processing/ns; make ROOT=$(ROOT)
	cd audio; make ROOT=$(ROOT)
	cp audio/audio_device.h $(DIST_INC_DIR)
	ar -cru $(DIST_LIB_DIR)/librtc-multimedia.a $(DIST_OBJ_DIR)/*.o

a:
