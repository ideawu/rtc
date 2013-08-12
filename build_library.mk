
C_SRC_FILES = $(filter %.c, $(LOCAL_SRC_FILES))
CC_SRC_FILES = $(filter %.cc %.cpp, $(LOCAL_SRC_FILES))

build_c :=
build_cc :=

ifneq ($(C_SRC_FILES),)
	build_c = $(C)   -c $(C_SRC_FILES) $(CFLAGS)
endif
ifneq ($(CC_SRC_FILES),)
	build_cc = $(CC) -c $(CC_SRC_FILES) $(CFLAGS)
endif


all:
	$(build_c)
	$(build_cc)
	$(AR)$(DIST_LIB_DIR)/$(LOCAL_MODULE).$(LIB) *.$(OBJ)
	cp *.$(OBJ) $(DIST_OBJ_DIR)
	echo

