/*
 *  random_test.c: another test driver for eval.h
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
#include "eval.h"

extern int random( void );

long zzz;

int main( void )
{
    cards_u cards, random_card;
    eval_u evalu1;
    int i, n, bit;
    int ones;
    int millions;

    ones = 0;
    millions = 0;
    while (true) {
	cards.cards_n = 0;
	for (i = 7; --i >= 0;) {
	    do {
		do
		    n = random() & 63;
		while (n >= 4 * N_RANK);
		bit = 1 << n % N_RANK;
		random_card.cards_n = 0;
		if (n < 26)
		    if (n < 13)
			random_card.cards_t.hearts   = bit;
		    else
			random_card.cards_t.diamonds = bit;
		else
		    if (n < 39)
			random_card.cards_t.clubs    = bit;
		    else
			random_card.cards_t.spades   = bit;
	    } while (cards.cards_n & random_card.cards_n);
	    cards.cards_n |= random_card.cards_n;
	}
	evalu1.eval_n = eval(cards);
	zzz += evalu1.eval_n;
	if (++ones == 1000000) {
	    ++millions;
	    printf("%d %d\r", millions, zzz);
	    fflush(stdout);
	    ones = 0;
	}
    }
    /* NEVER REACHED */
    return 0;
}
