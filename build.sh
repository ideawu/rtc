#!/bin/sh
# used to generate config.mk
DIR=`pwd`
cd `dirname $0`
BASE_DIR=`pwd`
cd $DIR

echo BASE_DIR := $BASE_DIR
echo DIST_LIB_DIR := $\(BASE_DIR\)/dist/lib
echo DIST_OBJ_DIR := $\(BASE_DIR\)/dist/obj
echo DIST_INC_DIR := $\(BASE_DIR\)/dist/include/rtc
echo 
echo TARGET_ARCH=x86
echo OS=mac
echo C=clang
echo CC=clang++
echo 
echo CFLAGS :=
echo CFLAGS += -Wall -Wno-sign-compare -g
echo CFLAGS += -I $BASE_DIR
echo CFLAGS += -I $BASE_DIR/../libave/dist/include

