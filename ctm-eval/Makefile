CC=gcc
CFLAGS=-g -O4 -Wall
FIVE_OBJ=five_card_hands.o tables.o helper.o

five_card_hands:	$(FIVE_OBJ)
	$(CC) -o five_card_hands $(FIVE_OBJ)

five_card_hands.o:	poker.h eval.h

mktables.o:	poker.h

eval.o:	poker.h

tables.c:	mktables
	rm -f tables.c
	./mktables > tables.c
	chmod a-w tables.c

clean:
	rm -f *.o mktables tables.c eval
