char rcsid_hand_names[] =
        "$Id$";

/*
 *  hand_names.c: a poker helper table
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

/*
 * NOTE: these must correspond to the hand_t typedef in poker.h
 */

#include "poker.h"

PUBLIC const hand_info_t hand_info[] =
{
    { "high hand",	 5, }, 
    { "pair",		 4, },
    { "two pair",	 3, },
    { "three of a kind", 3, },
    { "straight",	 1, },
    { "flush",		 5, },
    { "full house",	 2, },
    { "four of a kind",	 2, },
    { "straight flush",	 1, },
};
