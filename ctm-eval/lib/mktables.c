char rcsid_mktables[] =
        "$Id";

/*
 *  mktables.c: a program to generate the tables required by eval.h
 *		and rank_eval.h
 *
 *  Copyright (C) 1993  Clifford T. Matthews
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * usage: mktables [table_name.c] [table_name_2.c] ...
 *
 * Run with no arguments, mktables builds all the tables it knows how.
 *	optional arguments are filenames (complete with ".c" suffix),
 *	which restrict mktables to building only those tables.
 *
 * WARNING:  This program will write on read-only files if it can
 *		do a chmod(2) on them to allow writing.  This is
 *		so we can chmod -w tables after they're built so they
 *		won't be confused with source code that has been checked
 *		out, but hasn't been checked in.  You should never fiddle
 *		with tables by hand anyway.
 */

#include <stdio.h>
#include <assert.h>

#include "poker.h"

PRIVATE uint64 n_bits_func( uint32 n, uint32 ignored )
{
    int retval;

    assert(ignored == 0);
    retval = 0;
    while (n) {
	if (n & 1)
	    ++retval;
	n >>= 1;
    }
    return retval;
}

PRIVATE uint64 top_card_func( uint32 n, uint32 ignored )
{
    int retval, bit;

    assert(ignored == 0);
    if (n)
	for ( bit = (1 << ace), retval = ace; !(n & bit); bit >>= 1, --retval)
	    ;
    else
	return 0;
    return retval;
}

PRIVATE uint64 top_five_cards_func( uint32 n, uint32 ignored )
{
    eval_u eval;

    assert(ignored == 0);
    eval.eval_n = 0;
    eval.eval_t.top_card    = top_card_func(n, 0);
    n &= ~(1 << eval.eval_t.top_card);
    eval.eval_t.second_card = top_card_func(n, 0);
    n &= ~(1 << eval.eval_t.second_card);
    eval.eval_t.third_card  = top_card_func(n, 0);
    n &= ~(1 << eval.eval_t.third_card);
    eval.eval_t.fourth_card = top_card_func(n, 0);
    n &= ~(1 << eval.eval_t.fourth_card);
    eval.eval_t.fifth_card  = top_card_func(n, 0);
    return eval.eval_n;
}

PRIVATE uint64 mask_rank_func( uint32 rank, uint32 ignored )
{
    assert(ignored == 0);
    return mask_rank(rank);
}

PRIVATE boolean_t is_straight( uint32 cards )
{
    uint8 nseq;

    nseq = (cards & (1 << ace)) ? 1 : 0;
    while (cards && nseq < HAND_SIZE) {
	if (cards & 1)
	    ++nseq;
	else
	    nseq = 0;
	cards >>= 1;
    }
    return nseq >= HAND_SIZE;
}

PRIVATE boolean_t is_flush( uint32 cards )
{
    uint8 nbits;

    nbits = 0;

    while (cards && nbits < HAND_SIZE) {
	if (cards & 1)
	    ++nbits;
	cards >>= 1;
    }
    return nbits >= HAND_SIZE;
}

PRIVATE uint64 str_and_flu_func( uint32 cur_state, uint32 cards )
{
    hand_t retval;

    if (is_flush(cards)) {
	if (is_straight(cards))
	    retval = straight_flush;
	else
	    retval = flush;
    } else if (is_straight(cards))
	retval = straight;
    else
	retval = high_hand;
    return cur_state > retval ? cur_state : retval;
}

PRIVATE uint64 straight_func( uint32 cards, uint32 ignored )
{
    assert(ignored == 0);
    return is_straight(cards) ? straight : high_hand;
}

PRIVATE uint64 rank_fsm_func( uint32 cur_state, uint32 chunk )
{
    hand_t retval;
    uint8 npairs, ntrips, nquads;

    npairs = 0;
    ntrips = 0;
    nquads = 0;

    while (chunk) {
	switch (chunk & 7) {
	case 0:
	case 1:
	    break;
	case 2:
	    ++npairs;
	    break;
	case 3:
	    ++ntrips;
	    break;
	case 4:
	    ++nquads;
	    break;
	default:
	    /* it would be nice to put a spooey value in the table in */
	    /* all the slots that should never be examined.  This is one */
	    break;
	}
	chunk >>= 3;
    }

    retval = cur_state;
    switch (cur_state) {
	case four_of_a_kind:
	    break;		/* do nothing */
	case full_house:
	    if (nquads)
		retval = four_of_a_kind;
	    break;
	case three_of_a_kind:
	    if (nquads)
		retval = four_of_a_kind;
	    else if (npairs || ntrips)
		retval = full_house;
	    break;
	case two_pair:
	    if (nquads)
		retval = four_of_a_kind;
	    else if (ntrips)
		retval = full_house;
	    break;
	case pair:
	    if (nquads)
		retval = four_of_a_kind;
	    else if (ntrips)
		retval = full_house;
	    else if (npairs)
		retval = two_pair;
	    break;
	case high_hand:
	    if (nquads)
		retval = four_of_a_kind;
	    else if (ntrips) {
		if ((ntrips > 1) || npairs)
		    retval = full_house;
		else
		    retval = three_of_a_kind;
	    } else if (npairs > 1)
		retval = two_pair;
	    else if (npairs == 1)
		retval = pair;
	    break;
	case straight_flush:
	case flush:
	case straight:
	default:
	    /* This is another slot that should never be hit */
	    break;
    }
    return retval;
}

PRIVATE uint64 cards_to_counts_func( uint32 cards, uint32 ignored )
{
    uint64 retval;
    uint32 shift;

    assert(ignored == 0);
    shift = 0;
    retval = 0;
    while (cards) {
	if (cards & 1)
	    retval += (1LL << shift);
	cards >>= 1;
	shift += 3;

    }
    return retval;
}

typedef struct {
    char *table_type;
    char *table_name;
    char *source_name;
    int table_dim1;
    int table_dim2;
    int item_size;
    uint64 (*table_func)( uint32, uint32 );
} table_entry_t;

/*
 * return 0 for success, 1 for failure.
 */

PRIVATE int output_table(const table_entry_t *tp)
{
    FILE *fp;
    uint32 i, j, i_end, j_end;
    uint64 u64;
    uint32 high, low;
    int retval;

    chmod(tp->source_name, 0666);
    fp = fopen(tp->source_name, "w");
    if (!fp) {
	fprintf(stderr, "Could not open \"%s\" -- "
		"will try to process other arguments\n", tp->source_name);
	retval = 1;
    } else {
	retval = 0;
	fprintf(fp, "#include \"poker.h\"\n");
	fprintf(fp, "%s %s[%d]", tp->table_type, tp->table_name,
							       tp->table_dim1);
	if (tp->table_dim2)
	    fprintf(fp, "[%d]", tp->table_dim2);
	fprintf(fp, " = ");
	if (tp->table_dim2) {
	    fprintf(fp, "{\n");
	    i_end = tp->table_dim1;
	    j_end = tp->table_dim2;
	} else {
	    i_end = 1;
	    j_end = tp->table_dim1;
	}

	for (i = 0; i < i_end; ++i) {
	    fprintf(fp, " {\n");
	    for (j = 0; j < j_end; ++j) {
		if (tp->table_dim2)
		    u64  = tp->table_func(i, j);
		else
		    u64  = tp->table_func(j, 0);
		switch (tp->item_size) {
		default:
		    assert(0);
		    break;
		case sizeof(uint8):
		    assert(u64 < 0x100);
		    fprintf(fp, "    0x%02x,\n", (int) u64);
		    break;
		case sizeof(uint32):
		    assert(u64 < 0x100000000LL);
		    fprintf(fp, "    0x%08x,\n", (int) u64);
		    break;
		case sizeof(uint64):
		    high = u64 >> 32;
		    low  = u64;
		    fprintf(fp, "    0x%08x%08xLL,\n", high, low);
		    break;
		}
	    }
	    fprintf(fp, "}%s\n", i < i_end-1 ? "," : "");
	}

	fprintf(fp, "%s;\n", tp->table_dim2 ? "}" : "");

	if (fclose(fp) != 0) {
	    fprintf(stderr, "Trouble closing \"%s\" -- "
		"will try to process other arguments\n", tp->source_name);
	    retval = 1;
	}
	chmod(tp->source_name, 0444);
    }
    return retval;
}

#define	TABLE_ENTRY(type, name, dim1, dim2) 	\
    {						\
	#type,					\
	#name ## "_table",			\
	#name ## "_table.c",			\
	dim1,					\
	dim2,					\
	sizeof(type),				\
	name ## _func				\
    }


#define	NELEM(x)	(sizeof(x) / sizeof((x)[0]))

int main( int argc, char *argv[] )
{
    int retval;
    uint32 i, j;
    uint64 to_output;
    static table_entry_t tables[] = {
	TABLE_ENTRY(uint64, mask_rank      ,      N_RANK, 0),
	TABLE_ENTRY(uint8 , n_bits         , 1 << N_RANK, 0),
	TABLE_ENTRY(uint8 , straight       , 1 << N_RANK, 0),
	TABLE_ENTRY(uint32, top_five_cards , 1 << N_RANK, 0),
	TABLE_ENTRY(uint32, top_card       , 1 << N_RANK, 0),
	TABLE_ENTRY(uint64, cards_to_counts, 1 << N_RANK, 0),
	TABLE_ENTRY(uint8 , str_and_flu    ,      N_HAND, 1 << N_RANK),
	TABLE_ENTRY(uint8 , rank_fsm       ,      N_HAND, 1 << FSM_SHIFT),
    };

    assert(NELEM(tables) <= 64);

    if (argc == 1)
      to_output = -1;
    else {
	to_output = 0;
	for (i = 1; i < argc; ++i) {
	    for (j = 0; j < NELEM(tables); ++j)
		if (strcmp(argv[i], tables[j].source_name) == 0)
/*-->*/		    break;
	    if (j == NELEM(tables)) {
		fprintf(stderr, "Unknown table: \"%s\" -- "
				"all arguments ignored\n", argv[i]);
		retval = 1;
/*-->*/		goto DONE;
	    } else
		to_output |= (1 << j);
	}
    }

    retval = 0;
    for (i = 0; i < NELEM(tables); ++i) {
	if (to_output & (1 << i))
	    retval |= output_table(&tables[i]);
    }

DONE:
    return retval;
}
