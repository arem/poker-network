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

static int n_bits_table_func( int n )
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

static int top_card_table_func( int n )
{
    int retval, bit;

    if (n)
	for ( bit = (1 << ace), retval = ace; !(n & bit); bit >>= 1, --retval)
	    ;
    else
	return 0;
    return retval;
}

static int top_five_cards_table_func( int n )
{
    eval_u eval;

    eval.eval_n = 0;
    eval.eval_t.top_card    = top_card_table_func(n);
    n &= ~(1 << eval.eval_t.top_card);
    eval.eval_t.second_card = top_card_table_func(n);
    n &= ~(1 << eval.eval_t.second_card);
    eval.eval_t.third_card  = top_card_table_func(n);
    n &= ~(1 << eval.eval_t.third_card);
    eval.eval_t.fourth_card = top_card_table_func(n);
    n &= ~(1 << eval.eval_t.fourth_card);
    eval.eval_t.fifth_card  = top_card_table_func(n);
    return eval.eval_n;
}

static void output_table(const char *tabname, int (*fp)(int))
{
    int i;
    printf("int %s[1 << N_RANK] = {\n", tabname);
    for (i = 0; i < (1 << N_RANK); ++i)
	printf("    %d,\n", fp(i));
    printf("};\n");
}

int main( void )
{
    printf("#include \"poker.h\"\n");
    output_table("n_bits_table",         n_bits_table_func);
    output_table("top_five_cards_table", top_five_cards_table_func);
    output_table("top_card_table",       top_card_table_func);
    return 0;
}
