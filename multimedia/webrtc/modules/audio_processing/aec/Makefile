include $(ROOT)/config.mk

LOCAL_MODULE := libwebrtc_aec.a
LOCAL_SRC_FILES := \
    echo_cancellation.c \
    aec_resampler.c \
    aec_core.c \
    aec_rdft.c \

ifeq ($(TARGET_ARCH),x86)
LOCAL_SRC_FILES += \
    aec_core_sse2.c \
    aec_rdft_sse2.c
endif


include $(ROOT)/build_library.mk

