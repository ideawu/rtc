
all:
	cd base; make
	cd audio; make
	cd server; make

clean:
	cd base; make clean
	cd audio; make clean
	cd server; make clean

