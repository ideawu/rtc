include $(ROOT)/config.mk

LOCAL_SRC_FILES = $(wildcard *.cpp)

all:
	$(CC) -c $(filter-out fde_%.cpp, $(LOCAL_SRC_FILES)) $(CFLAGS)
	ar -cru $(DIST_LIB_DIR)/librtc-base.a *.o
