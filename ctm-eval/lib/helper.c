/*
 *  helper.c: a collection of poker hand helper routines
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


#include "poker.h"
#include <stdio.h>

PUBLIC void dump_rank( int ranks, char suitchar )
{
    int bit;
    const char *namep;
    static char names[] = "AKQJT98765432";

    for (bit = 1 << (N_RANK - 1), namep = names; bit; bit >>= 1, ++namep) {
	if (ranks & bit) {
	    putchar(*namep);
	    putchar(suitchar);
	    putchar(' ');
	}
    }
}

PUBLIC void dump_cards( cards_u cards )
{
    dump_rank(cards.cards_t.hearts,   'H');
    dump_rank(cards.cards_t.diamonds, 'D');
    dump_rank(cards.cards_t.spades,   'S');
    dump_rank(cards.cards_t.clubs,    'C');
    putchar('\n');
}

PUBLIC const char *hand_names[] = {
    "internal error",
    "high hand",
    "pair",
    "two pair",
    "three of a kind",
    "straight",
    "flush",
    "full house",
    "four of a kind",
    "straight flush",
};

PUBLIC void dump_eval( eval_u eval )
{
    static const char *rank_names[] = {
	"deuce",
	"trey",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine",
	"ten",
	"jack",
	"queen",
	"king",
	"ace",
    };
    printf("%s: %s", hand_names[eval.eval_t.hand],
					     rank_names[eval.eval_t.top_card]);
    if (eval.eval_t.second_card)
	printf(", %s", rank_names[eval.eval_t.second_card]);
    if (eval.eval_t.third_card)
	printf(", %s", rank_names[eval.eval_t.third_card]);
    if (eval.eval_t.fourth_card)
	printf(", %s", rank_names[eval.eval_t.fourth_card]);
    if (eval.eval_t.fifth_card)
	printf(", %s", rank_names[eval.eval_t.fifth_card]);

    printf("\n");
}
