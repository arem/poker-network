#ifndef _MAKESWITCH_H_
#define _MAKESWITCH_H_

/*
 *  makeswitch.h: the include file for makeswitch.c
 *
 *  Copyright (C) 1994  Clifford T. Matthews
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

typedef enum
{
  /* CARDS_DEALT is passed in from Makefile */
  /* CARDS_DEALT = 7 */
} cards_dealt_t;

typedef enum
{
  MAY_HAVE_FOUR_OF_A_KIND  = (1 << 0),
  MAY_HAVE_FLUSH           = (1 << 1),
  MUST_HAVE_STRAIGHT       = (1 << 2),
  MAY_HAVE_FULL_HOUSE      = (1 << 3),
  MAY_HAVE_THREE_OF_A_KIND = (1 << 4),
  MUST_HAVE_AT_LEAST_PAIR  = (1 << 5),
  MUST_HAVE_PAIR           = (1 << 6),
} may_have_t;

#define MAY_HAVE_COMBINATIONS 0x80

#endif  /* !_MAKESWITCH_H_ */
