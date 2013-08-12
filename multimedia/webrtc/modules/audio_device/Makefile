include $(ROOT)/config.mk

LOCAL_MODULE := libwebrtc_audio_device
LOCAL_SRC_FILES := \
    audio_device_buffer.cc \
    audio_device_generic.cc \
    audio_device_utility.cc \
    audio_device_impl.cc

ifeq ($(OS), mac)
LOCAL_SRC_FILES += \
    mac/audio_device_mac.cc \
    mac/audio_device_utility_mac.cc \
    mac/audio_mixer_manager_mac.cc \
	mac/portaudio/pa_ringbuffer.c
endif
ifeq ($(OS), linux)
LOCAL_SRC_FILES += \
    linux/audio_device_alsa_linux.cc \
    linux/audio_device_utility_linux.cc \
    linux/audio_mixer_manager_alsa_linux.cc
endif

CFLAGS += \
	-I ./$(OS)


include $(ROOT)/build_library.mk
	
