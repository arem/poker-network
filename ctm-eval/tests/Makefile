FIVE_OBJ=five_card_hands.o
RANDOM_OBJ=random_test.o eval2.o
NEWRANDOM_OBJ=newrandom.o

LIB_POKER=../lib/libpoker.a

CFLAGS=-g -O4 -Wall -finline-functions -Winline
# CC=egcs
# CFLAGS=-g -O6 -Wall -finline-functions -Winline -mpentiumpro
# CFLAGS=-g -Wall

all:	five_card_hands random_test newrandom tests

tests: digtest5 digtest7

five_card_hands:        $(FIVE_OBJ) $(LIB_POKER)
	$(CC) $(CFLAGS) -o $@ $(FIVE_OBJ) $(LIB_POKER)

random_test:    $(RANDOM_OBJ) $(LIB_POKER) 
	$(CC) $(CFLAGS) -o $@ $(RANDOM_OBJ) $(LIB_POKER)

newrandom:      $(NEWRANDOM_OBJ) $(LIB_POKER)
	$(CC) $(CFLAGS) -o $@ $(NEWRANDOM_OBJ) $(LIB_POKER)

digtest5 : digtest5.o
	$(CC) $(CFLAGS) -o $@ $< $(LIB_POKER)

digtest7 : digtest7.o
	$(CC) $(CFLAGS) -o $@ $< $(LIB_POKER)

clean:
	rm -f $(FIVE_OBJ) $(RANDOM_OBJ) $(NEWRANDOM_OBJ) \
	five_card_hands random_test newrandom digtest5 digtest7

.c.o:
	$(CC) $(CFLAGS) -I../include -c $<

test : tests
	@echo -n "digtest5: 5-card hands..."
	@if [ `./digtest5` = "1f066feff1a8b3406fe069c05c4ba17d" ]; \
		then echo "		Passed"; \
		else echo "		Failed"; \
	fi
	@echo -n "digtest7: 7-card hands..."
	@if [ `./digtest7` = "03a86bd2deac3052bbe93256e0d7b156" ]; \
		then echo "		Passed"; \
		else echo "		Failed"; \
	fi
