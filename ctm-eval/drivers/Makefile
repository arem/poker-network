FISH_OBJ=fish.o
CMP2_OBJ=cmp2.o
CMPN_OBJ=cmpn.o
CARO2_OBJ=caro2.o
CARO2A_OBJ=caro2a.o
CARO2B_OBJ=caro2b.o
CARODUMP_OBJ=carodump.o
CMPALL_OBJ=cmpall.o

LIB_POKER=../lib/libpoker.a

CFLAGS=-g -O4 -Wall -finline-functions -Winline -DNDEBUG -fomit-frame-pointer
# CC=egcs
# CFLAGS=-g -O6 -Wall -finline-functions -Winline -DNDEBUG -fomit-frame-pointer -mpentiumpro
# CFLAGS=-g -Wall

all:    fish cmp2 cmpn cmpall caro2 caro2a caro2b

fish:   $(FISH_OBJ)
	$(CC) $(CFLAGS) -o fish $(FISH_OBJ) $(LIB_POKER)

cmp2:   $(CMP2_OBJ)
	$(CC) $(CFLAGS) -o cmp2 $(CMP2_OBJ) $(LIB_POKER)

cmpn:   $(CMPN_OBJ)
	$(CC) $(CFLAGS) -o cmpn $(CMPN_OBJ) $(LIB_POKER)

cmpall: $(CMPALL_OBJ)
	$(CC) $(CFLAGS) -o cmpall $(CMPALL_OBJ) $(LIB_POKER)

caro2:	$(CARO2_OBJ)
	$(CC) $(CFLAGS) -o caro2 $(CARO2_OBJ) $(LIB_POKER)

caro2a:	$(CARO2A_OBJ)
	$(CC) $(CFLAGS) -o caro2a $(CARO2A_OBJ) $(LIB_POKER)

caro2b:	$(CARO2B_OBJ)
	$(CC) $(CFLAGS) -o caro2b $(CARO2B_OBJ) $(LIB_POKER)

carodump:	$(CARODUMP_OBJ)
	$(CC) $(CFLAGS) -o carodump $(CARODUMP_OBJ) $(LIB_POKER)


clean:
	rm -f $(FISH_OBJ) $(CMP2_OBJ) $(CMPALL_OBJ) $(CARO2_OBJ) \
	$(CARO2A_OBJ) $(CARO2B_OBJ) fish cmp2 cmpall caro2 caro2a caro2b \
	carodump cmpn $(CMPN_OBJ)

.c.o:
	$(CC) $(CFLAGS) -I../include -c $<
