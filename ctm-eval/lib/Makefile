LIB=libpoker.a

OBJ=dump_cards.o dump_eval.o dump_rank.o hand_names.o n_bits_table.o \
	top_five_cards_table.o top_card_table.o mask_rank_table.o \
	str_and_flu_table.o straight_table.o rank_fsm_table.o \
	cards_to_counts_table.o

DERIVED_SRC=n_bits_table.c top_five_cards_table.c top_card_table.c \
	mask_rank_table.c str_and_flu_table.c straight_table.c \
	rank_fsm_table.c cards_to_counts_table.c

CC=gcc
CFLAGS=-g -O4 -Wall -finline-functions -Winline
RANLIB=ranlib

$(LIB):	$(OBJ)
	ar cr $(LIB) $(OBJ)
	$(RANLIB) $(LIB)

clean:
	rm -f $(OBJ) $(LIB) $(DERIVED_SRC) mktables core

$(DERIVED_SRC):	mktables
	./mktables $@

mktables:	mktables.c
	$(CC) $(CFLAGS) -I../include -o mktables mktables.c

.c.o:
	$(CC) $(CFLAGS) -I../include -c $<
