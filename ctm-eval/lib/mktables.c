/*
 *  mktables.c: a program to generate the tables required by eval.h
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

#include <stdio.h>
#include "poker.h"

static uint64 n_bits_func( int n )
{
    int retval;

    retval = 0;
    while (n) {
	if (n & 1)
	    ++retval;
	n >>= 1;
    }
    return retval;
}

static uint64 top_card_func( int n )
{
    int retval, bit;

    if (n)
	for ( bit = (1 << ace), retval = ace; !(n & bit); bit >>= 1, --retval)
	    ;
    else
	return 0;
    return retval;
}

static uint64 top_five_cards_func( int n )
{
    eval_u eval;

    eval.eval_n = 0;
    eval.eval_t.top_card    = top_card_func(n);
    n &= ~(1 << eval.eval_t.top_card);
    eval.eval_t.second_card = top_card_func(n);
    n &= ~(1 << eval.eval_t.second_card);
    eval.eval_t.third_card  = top_card_func(n);
    n &= ~(1 << eval.eval_t.third_card);
    eval.eval_t.fourth_card = top_card_func(n);
    n &= ~(1 << eval.eval_t.fourth_card);
    eval.eval_t.fifth_card  = top_card_func(n);
    return eval.eval_n;
}

static uint64 mask_rank_func( int rank )
{
    return mask_rank(rank);
}

static void output_table(const char *typename, const char *tabname, int len,
							     uint64 (*fp)(int))
{
    int i;
    uint64 u64;
    uint32 high, low;

    printf("%s %s_table[] = {\n", typename, tabname);
    for (i = 0; i < len; ++i) {
	u64  = fp(i);
	high = u64 >> 32;
	low  = u64;
	printf("    0x%08x%08x%s,\n", high, low, high ? "LL" : "");
    }
    printf("};\n");
}

int main( void )
{
    printf("#include \"poker.h\"\n");

    output_table("int",    "n_bits",         1 << N_RANK, n_bits_func);
    output_table("int",    "top_five_cards", 1 << N_RANK, top_five_cards_func);
    output_table("int",    "top_card",       1 << N_RANK, top_card_func);
    output_table("uint64", "mask_rank",           N_RANK, mask_rank_func);

    return 0;
}
