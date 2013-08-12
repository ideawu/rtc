$(shell sh build.sh > config.mk)
include config.mk

ROOT = $(shell pwd)

.PHONY: *

all: base mm client
	@echo

server:
	cd server/voice; make ROOT=$(ROOT)
	cd server; make ROOT=$(ROOT)
	@echo

base:
	cd base; make ROOT=$(ROOT)
	@echo

mm:
	cd multimedia; make ROOT=$(ROOT)
	@echo

client:
	cd client; make ROOT=$(ROOT)
	@echo

test:
	cd test; make ROOT=$(ROOT)
	@echo

clean:
	cd server; make clean
	rm -f $(DIST_LIB_DIR)/* $(DIST_INC_DIR)/* $(DIST_OBJ_DIR)/*
	@echo

