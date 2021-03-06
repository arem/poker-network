char rcsid_switch_preamble[] =
	"$Id$";

/*
 *  switch_preamble.c: the unchanging beginning part of neweval.h
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

#include "poker.h"
#include "tree_inlines.h"

static inline uint32
eval_exactly_%d_cards (rank_set_t clubs, rank_set_t diamonds, rank_set_t hearts,
      rank_set_t spades)
{
  rank_set_t ranks, flush_suit, three_info, all_pairs;
  uint32 retval;

  ranks = clubs | diamonds | hearts | spades;

  switch (ranks)
    {
