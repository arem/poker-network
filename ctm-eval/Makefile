CC=gcc
OBJ=eval.o tables.o eval2.o
CFLAGS=-g -O4 -Wall

eval:	$(OBJ)
	$(CC) -o eval $(OBJ)

mktables.o:	poker.h

eval.o:	poker.h

tables.c:	mktables
	rm -f tables.c
	./mktables > tables.c
	chmod a-w tables.c

clean:
	rm -f $(OBJ) mktables mktables.o tables.c eval
