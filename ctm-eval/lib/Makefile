OBJ=dump_cards.o dump_eval.o dump_rank.o hand_names.o
LIB=libpoker.a
CFLAGS=-g -O4 -Wall
CC=gcc
RANLIB=ranlib

$(LIB):	$(OBJ)
	ar cur $(LIB) $(OBJ)
	$(RANLIB) $(LIB)

clean:
	rm -f $(OBJ) $(LIB)

.c.o:
	$(CC) $(CFLAGS) -I../include -c $<
