FIVE_OBJ=five_card_hands.o
RANDOM_OBJ=random_test.o eval2.o
NEWRANDOM_OBJ=newrandom.o

LIB_POKER=../lib/libpoker.a

CC=gcc
CFLAGS=-g -O4 -Wall -finline-functions -Winline
# CFLAGS=-g -Wall

five_card_hands:        $(FIVE_OBJ)
	$(CC) -o five_card_hands $(FIVE_OBJ) $(LIB_POKER)

random_test:    $(RANDOM_OBJ)
	$(CC) -o random_test $(RANDOM_OBJ) $(LIB_POKER)

newrandom:      $(NEWRANDOM_OBJ) $(LIB_POKER)
	$(CC) -o newrandom $(NEWRANDOM_OBJ) $(LIB_POKER)

clean:
	rm -f $(FIVE_OBJ) $(RANDOM_OBJ) five_card_hands random_test

.c.o:
	$(CC) $(CFLAGS) -I../include -I../new -c $<
