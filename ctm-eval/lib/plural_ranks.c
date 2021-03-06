char rcsid_plural_ranks[] =
        "$Id$";

/*
 *  plural_ranks.c: a poker helper table
 *
 *  Copyright (C) 1998  Clifford T. Matthews
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
 * NOTE: these must correspond to the rank_t typedef in poker.h
 */

#include "poker.h"

PUBLIC const char *plural_ranks[] = {
    "deuces",
    "treys",
    "fours",
    "fives",
    "sixes",
    "sevens",
    "eights",
    "nines",
    "tens",
    "jacks",
    "queens",
    "kings",
    "aces",
};
