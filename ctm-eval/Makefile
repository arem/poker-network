CC=gcc
CFLAGS=-g -O4 -Wall
FIVE_OBJ=five_card_hands.o tables.o helper.o
RANDOM_OBJ=random_test.o tables.o helper.o eval2.o

five_card_hands:	$(FIVE_OBJ)
	$(CC) -o five_card_hands $(FIVE_OBJ)

random_test:	$(RANDOM_OBJ)
	$(CC) -o random_test $(RANDOM_OBJ)

five_card_hands.o:	poker.h eval.h
random_test.o:	poker.h eval.h

mktables.o:	poker.h

eval.o:	poker.h
eval2.o: poker.h

tables.c:	mktables
	rm -f tables.c
	./mktables > tables.c
	chmod a-w tables.c

clean:
	rm -f *.o mktables tables.c eval
