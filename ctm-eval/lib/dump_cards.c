/*
 *  dump_cards.c: a poker hand helper routine
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
#include <stdio.h>

PUBLIC void dump_cards( cards_u cards )
{
    dump_rank(cards.cards_t.hearts,   'H');
    dump_rank(cards.cards_t.diamonds, 'D');
    dump_rank(cards.cards_t.spades,   'S');
    dump_rank(cards.cards_t.clubs,    'C');
    putchar('\n');
}
