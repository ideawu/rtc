
$(sh build.sh > config.mk)
include config.mk

ROOT = $(shell pwd)

.PHONY: clean test

all:
	cd base; make ROOT=$(ROOT)
	cd server/voice; make ROOT=$(ROOT)
	cd server; make ROOT=$(ROOT)
	cd test; make ROOT=$(ROOT)
	cd client; make ROOT=$(ROOT)

test:
	cd test; make ROOT=$(ROOT)

a:
	cd client; make ROOT=$(ROOT)

clean:
	cd base; make clean
	cd server/voice; make clean
	cd server; make clean
	cd test; make clean

