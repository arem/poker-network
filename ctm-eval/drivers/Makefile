FISH_OBJ=fish.o
CMP2_OBJ=cmp2.o
CMPALL_OBJ=cmpall.o

LIB_POKER=../lib/libpoker.a

CC=gcc
CFLAGS=-g -O4 -Wall -finline-functions -Winline

all:	fish cmp2 cmpall

fish:	$(FISH_OBJ)
	$(CC) -o fish $(FISH_OBJ) $(LIB_POKER)

cmp2:	$(CMP2_OBJ)
	$(CC) -o cmp2 $(CMP2_OBJ) $(LIB_POKER)

cmpall:	$(CMPALL_OBJ)
	$(CC) -o cmpall $(CMPALL_OBJ) $(LIB_POKER)

clean:
	rm -f $(FISH_OBJ) $(CMP2_OBJ) $(CMPALL_OBJ) fish cmp2 cmpall

.c.o:
	$(CC) $(CFLAGS) -I../include -c $<
