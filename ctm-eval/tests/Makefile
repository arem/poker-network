FIVE_OBJ=five_card_hands.o
RANDOM_OBJ=random_test.o eval2.o
NEWRANDOM_OBJ=newrandom.o

LIB_POKER=../lib/libpoker.a

CC=gcc
CFLAGS=-g -O4 -Wall -finline-functions -Winline
# CFLAGS=-g -Wall

all:	five_card_hands random_test newrandom

five_card_hands:        $(FIVE_OBJ)
	$(CC) $(CFLAGS) -o five_card_hands $(FIVE_OBJ) $(LIB_POKER)

random_test:    $(RANDOM_OBJ)
	$(CC) $(CFLAGS) -o random_test $(RANDOM_OBJ) $(LIB_POKER)

newrandom:      $(NEWRANDOM_OBJ) $(LIB_POKER)
	$(CC) $(CFLAGS) -o newrandom $(NEWRANDOM_OBJ) $(LIB_POKER)

clean:
	rm -f $(FIVE_OBJ) $(RANDOM_OBJ) $(NEWRANDOM_OBJ) \
	five_card_hands random_test newrandom

.c.o:
	$(CC) $(CFLAGS) -I../include -I../new -c $<
