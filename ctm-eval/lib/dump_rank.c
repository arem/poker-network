char rcsid_dump_rank[] =
        "$Id";

/*
 *  dump_rank.c: a poker helper routine
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

PUBLIC void dump_rank( uint32 ranks, char suitchar )
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
