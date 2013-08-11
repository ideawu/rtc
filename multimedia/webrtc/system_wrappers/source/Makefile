include $(ROOT)/config.mk

LOCAL_MODULE := libwebrtc_system_wrappers
LOCAL_SRC_FILES := \
	map.cc \
	sort.cc \
	aligned_malloc.cc \
	condition_variable.cc \
	cpu_features.cc \
	cpu_info.cc \
	critical_section.cc \
	event.cc \
	event_tracer.cc \
	file_impl.cc \
	list_no_stl.cc \
	logging.cc \
	rw_lock.cc \
	thread.cc \
	trace_impl.cc \
	sleep.cc \
	tick_util.cc

ifeq ($(OS), mac)
LOCAL_SRC_FILES += \
	condition_variable_posix.cc \
	critical_section_posix.cc \
	event_posix.cc \
	thread_posix.cc \
	trace_posix.cc \
	rw_lock_posix.cc 
endif

ifeq ($(OS), win)
LOCAL_SRC_FILES += \
    thread_win.cc \
    condition_variable_native_win.cc \
    critical_section_win.cc \
    event_win.cc \
    trace_win.cc \
    rw_lock_win.cc 
endif


include $(ROOT)/build_library.mk

