char rcsid_string_to_card[] =
        "$Id";


/*
 *  string_to_card.c: a poker hand helper routine
 *
 *  Copyright (C) 1993, 1994  Clifford T. Matthews
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

PUBLIC uint64 string_to_card(const char *str)
{
    rank_t rank;
    suit_t suit;

    rank = N_RANK;	/* illegal */
    suit = N_SUIT;	/* illegal */

    if (strlen(str) == 2) {
	switch (str[0]) {
	case '2':
	    rank = deuce;
	    break;
	case '3':
	    rank = trey;
	    break;
	case '4':
	    rank = four;
	    break;
	case '5':
	    rank = five;
	    break;
	case '6':
	    rank = six;
	    break;
	case '7':
	    rank = seven;
	    break;
	case '8':
	    rank = eight;
	    break;
	case '9':
	    rank = nine;
	    break;
	case 'T': case 't':
	    rank = ten;
	    break;
	case 'J': case 'j':
	    rank = jack;
	    break;
	case 'Q': case 'q':
	    rank = queen;
	    break;
	case 'K': case 'k':
	    rank = king;
	    break;
	case 'A': case 'a':
	    rank = ace;
	    break;
	}
	switch (str[1]) {
	case 'H': case 'h':
	    suit = hearts;
	    break;
	case 'D': case 'd':
	    suit = diamonds;
	    break;
	case 'C': case 'c':
	    suit = clubs;
	    break;
	case 'S': case 's':
	    suit = spades;
	    break;
	}
    }

    return (rank == N_RANK || suit == N_SUIT) ?
	0
    :
	((uint64) 1 << rank) << (suit * 13);
}
