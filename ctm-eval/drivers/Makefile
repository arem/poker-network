FISH_OBJ=fish.o
CMP2_OBJ=cmp2.o
CMPALL_OBJ=cmpall.o
NEWCMP2_OBJ=newcmp2.o
NEWCMPALL_OBJ=newcmpall.o

LIB_POKER=../lib/libpoker.a

CC=gcc
CFLAGS=-g -O4 -Wall -finline-functions -Winline -DNDEBUG
# CFLAGS=-g -Wall

all:    fish cmp2 cmpall newcmp2 newcmpall

fish:   $(FISH_OBJ)
	$(CC) -o fish $(FISH_OBJ) $(LIB_POKER)

cmp2:   $(CMP2_OBJ)
	$(CC) -o cmp2 $(CMP2_OBJ) $(LIB_POKER)

newcmp2:        $(NEWCMP2_OBJ)	../new/neweval.h
	$(CC) -o newcmp2 $(NEWCMP2_OBJ) $(LIB_POKER)

cmpall: $(CMPALL_OBJ)
	$(CC) -o cmpall $(CMPALL_OBJ) $(LIB_POKER)

newcmpall: $(NEWCMPALL_OBJ)	../new/neweval.h
	$(CC) -o newcmpall $(NEWCMPALL_OBJ) $(LIB_POKER)

../new/neweval.h:
	(cd ../new; make)

clean:
	rm -f $(FISH_OBJ) $(CMP2_OBJ) $(CMPALL_OBJ) $(NEWCMP2_OBJ) \
	$(NEWCMPALL_OBJ) fish cmp2 cmpall newcmp2 newcmpall
	
.c.o:
	$(CC) $(CFLAGS) -I../include -I../new -c $<
