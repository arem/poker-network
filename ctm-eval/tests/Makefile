FIVE_OBJ=five_card_hands.o
RANDOM_OBJ=random_test.o eval2.o

LIB_POKER=../lib/libpoker.a

CC=gcc
CFLAGS=-g -O4 -Wall -finline-functions -Winline

five_card_hands:	$(FIVE_OBJ)
	$(CC) -o five_card_hands $(FIVE_OBJ) $(LIB_POKER)

random_test:	$(RANDOM_OBJ)
	$(CC) -o random_test $(RANDOM_OBJ) $(LIB_POKER)

clean:
	rm -f $(FIVE_OBJ) $(RANDOM_OBJ) five_card_hands random_test

.c.o:
	$(CC) $(CFLAGS) -I../include -c $<
