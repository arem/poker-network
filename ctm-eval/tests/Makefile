FIVE_OBJ=five_card_hands.o
RANDOM_OBJ=random_test.o eval2.o
NEWRANDOM_OBJ=newrandom.o

LIB_POKER=../lib/libpoker.a

CFLAGS=-g -O4 -Wall -finline-functions -Winline
# CC=egcs
# CFLAGS=-g -O6 -Wall -finline-functions -Winline -mpentiumpro
# CFLAGS=-g -Wall

all:	five_card_hands random_test newrandom tests

tests: eval-digest eval5-digest eval7-digest

five_card_hands:        $(FIVE_OBJ) $(LIB_POKER)
	$(CC) $(CFLAGS) -o $@ $(FIVE_OBJ) $(LIB_POKER)

random_test:    $(RANDOM_OBJ) $(LIB_POKER) 
	$(CC) $(CFLAGS) -o $@ $(RANDOM_OBJ) $(LIB_POKER)

newrandom:      $(NEWRANDOM_OBJ) $(LIB_POKER)
	$(CC) $(CFLAGS) -o $@ $(NEWRANDOM_OBJ) $(LIB_POKER)

eval-digest : eval-digest.o
	$(CC) $(CFLAGS) -o $@ $< $(LIB_POKER)

eval5-digest : eval5-digest.o
	$(CC) $(CFLAGS) -o $@ $< $(LIB_POKER)

eval7-digest : eval7-digest.o
	$(CC) $(CFLAGS) -o $@ $< $(LIB_POKER)

clean:
	rm -f $(FIVE_OBJ) $(RANDOM_OBJ) $(NEWRANDOM_OBJ) \
	five_card_hands random_test newrandom eval-digest eval5-digest \
	eval7-digest

.c.o:
	$(CC) $(CFLAGS) -I../include -c $<

test : tests
	@if [ `./eval-digest` = "d0facbffc225197e0bf30a9501ddbab8" ]; \
		then echo "eval-digest:		Passed"; \
		else echo "eval-digest:		Failed"; \
	fi
	@if [ `./eval5-digest` = "d0facbffc225197e0bf30a9501ddbab8" ]; \
		then echo "eval5-digest:		Passed"; \
		else echo "eval5-digest:		Failed"; \
	fi
	@if [ `./eval7-digest` = "73844ddc4017d186d0e590c46f6d6b8e" ]; \
		then echo "eval7-digest:		Passed"; \
		else echo "eval7-digest:		Failed"; \
	fi
