include $(ROOT)/config.mk

CFLAGS += \
	-I $(DIST_INC_DIR)
LOCAL_SRC_FILES = $(wildcard *.cpp)

all:
	$(CC) -o rtc_voice_client_impl.o -c voice_client_impl.cpp $(CFLAGS)
	cp *.o $(DIST_OBJ_DIR)
	cp voice_client.h $(DIST_INC_DIR)

