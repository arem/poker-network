FISH_OBJ=fish.o
CMP2_OBJ=cmp2.o
CARO2_OBJ=caro2.o
CARODUMP_OBJ=carodump.o
CMPALL_OBJ=cmpall.o

LIB_POKER=../lib/libpoker.a

CC=gcc
# CFLAGS=-g -O4 -Wall -finline-functions -Winline -DNDEBUG -fomit-frame-pointer
CFLAGS=-g -Wall

all:    fish cmp2 cmpall caro2

fish:   $(FISH_OBJ)
	$(CC) $(CFLAGS) -o fish $(FISH_OBJ) $(LIB_POKER)

cmp2:   $(CMP2_OBJ)
	$(CC) $(CFLAGS) -o cmp2 $(CMP2_OBJ) $(LIB_POKER)

cmpall: $(CMPALL_OBJ)
	$(CC) $(CFLAGS) -o cmpall $(CMPALL_OBJ) $(LIB_POKER)

caro2:	$(CARO2_OBJ)
	$(CC) $(CFLAGS) -o caro2 $(CARO2_OBJ) $(LIB_POKER)

carodump:	$(CARODUMP_OBJ)
	$(CC) $(CFLAGS) -o carodump $(CARODUMP_OBJ) $(LIB_POKER)


clean:
	rm -f $(FISH_OBJ) $(CMP2_OBJ) $(CMPALL_OBJ) fish cmp2 cmpall caro2 carodump

.c.o:
	$(CC) $(CFLAGS) -I../include -c $<
