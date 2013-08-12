$(shell sh build.sh > config.mk)
include config.mk

ROOT = $(shell pwd)

.PHONY: *

all: base mm client

server:
	cd server/voice; make ROOT=$(ROOT)
	cd server; make ROOT=$(ROOT)

base:
	cd base; make ROOT=$(ROOT)

mm:
	cd multimedia; make ROOT=$(ROOT)

client:
	cd client; make ROOT=$(ROOT)

test:
	cd test; make ROOT=$(ROOT)

clean:
	cd server; make clean

