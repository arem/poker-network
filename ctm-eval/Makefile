# Makefile for top-level of the poker library, tests and sample programs
#

# ctm's gcc uses a new version of glibc that doesn't have the bug that
# the glibc on irc.poker.net has in it, but it can only be used to make static
# binaries
export CC = /home/ctm/bin/gcc -static

all:
	(cd lib; make)
	(cd tests; make)
	(cd drivers; make)
	(cd tbot2; make)

clean:
	(cd lib; make clean)
	(cd tests; make clean)
	(cd drivers; make clean)
	(cd tbot2; make clean)

test:
	(echo $$$$)

# hack so I can push the code over kermit quicker
# should really depend on more
/tmp/poker.tar.bz2:	Makefile tbot2/dealer
	(rm -rf /tmp/poker.$$$$ 			;\
	mkdir /tmp/poker.$$$$   			;\
	cp -a ../poker /tmp/poker.$$$$			;\
	cd /tmp/poker.$$$$/poker  			;\
	make clean					;\
	cd ..						;\
	tar cf - poker | bzip2 -9 > /tmp/poker.tar.bz2	;\
	rm -rf /tmp/poker.$$$$)
 