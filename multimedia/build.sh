#!/bin/sh
# used to generate config.mk
DIR=`pwd`
cd `dirname $0`
BASE_DIR=`pwd`
cd $DIR
BASE_DIR=`echo $BASE_DIR | sed -e "s/^\/cygdrive\/\([a-z]\)/\1:/i"`

DIST_LIB_DIR=${BASE_DIR}/../dist/lib
DIST_OBJ_DIR=${BASE_DIR}/../dist/obj
DIST_INC_DIR=${BASE_DIR}/../dist/include/rtc

mkdir -p ${DIST_LIB_DIR}
mkdir -p ${DIST_OBJ_DIR}
mkdir -p ${DIST_INC_DIR}


TARGET_OS=`uname -s`
case "$TARGET_OS" in
    Darwin)
		OS=mac
		C=clang
		CC=clang++
		AR="ar -cru "
		OBJ=o
		LIB=a
        ;;
    CYGWIN_*)
		OS=win
		C=cl
		CC=cl
		AR="lib /NOLOGO /OUT:"
 		OBJ=obj
		LIB=lib
       ;;
    *)
        echo "Unknown platform!" >&2
        exit 1
esac



#### generate config.mk ####

echo BASE_DIR := $BASE_DIR
echo DIST_LIB_DIR := $DIST_LIB_DIR
echo DIST_OBJ_DIR := $DIST_OBJ_DIR
echo DIST_INC_DIR := $DIST_INC_DIR

echo TARGET_ARCH=x86
echo OS=$OS
echo C=$C
echo CC=$CC
echo AR=$AR
echo OBJ=$OBJ
echo LIB=$LIB

echo CFLAGS :=
echo CFLAGS += -I $BASE_DIR
echo CFLAGS += -D WEBRTC_${OS^^} -D WEBRTC_CLOCK_TYPE_REALTIME -DWEBRTC_NS_FIXED
if [ "$OS" == "mac" ]; then
	echo CFLAGS += -Wall -Wno-sign-compare -g
fi
if [ "$OS" == "win" ]; then
	echo CFLAGS += /EHsc
fi


