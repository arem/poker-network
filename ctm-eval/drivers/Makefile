FISH_OBJ=fish.o
CMP2_OBJ=cmp2.o

LIB_POKER=../lib/libpoker.a

CC=gcc
CFLAGS=-g -O4 -Wall -finline-functions -Winline

fish:	$(FISH_OBJ)
	$(CC) -o fish $(FISH_OBJ) $(LIB_POKER)

cmp2:	$(CMP2_OBJ)
	$(CC) -o cmp2 $(CMP2_OBJ) $(LIB_POKER)

clean:
	rm -f $(FISH_OBJ) fish

.c.o:
	$(CC) $(CFLAGS) -I../include -c $<
