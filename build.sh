#!/bin/sh
# used to generate config.mk
DIR=`pwd`
cd `dirname $0`
BASE_DIR=`pwd`
cd $DIR

DIST_LIB_DIR=${BASE_DIR}/dist/lib
DIST_OBJ_DIR=${BASE_DIR}/dist/obj
DIST_INC_DIR=${BASE_DIR}/dist/include/rtc

mkdir -p ${DIST_LIB_DIR}
mkdir -p ${DIST_OBJ_DIR}
mkdir -p ${DIST_INC_DIR}


#### generate config.mk ####

echo BASE_DIR := $BASE_DIR
echo DIST_LIB_DIR := $DIST_LIB_DIR
echo DIST_OBJ_DIR := $DIST_OBJ_DIR
echo DIST_INC_DIR := $DIST_INC_DIR
echo 
echo TARGET_ARCH=x86
echo OS=mac
echo C=clang
echo CC=clang++
echo 
echo CFLAGS :=
echo CFLAGS += -Wall -Wno-sign-compare -g
echo CFLAGS += -I $BASE_DIR

