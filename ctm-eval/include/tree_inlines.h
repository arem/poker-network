#if	!defined(_TREE_INLINES_H_)
#define	_TREE_INLINES_H_

/*
 *  tree_inlines.h: The inline functions that are used by neweval.h
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

static inline uint32 flush_value(rank_set_t ranks)
{
    return FLUSH_VALUE | top_five_bits_table[ranks];
}

/*
 * straight_value_table has STRAIGHT_VALUE or'd in
 */

static inline uint32 straight_value(rank_set_t ranks)
{
    return straight_value_table[ranks];
}

static inline uint32 high_hand_value(rank_set_t ranks)
{
    return HIGH_HAND_VALUE | top_five_bits_table[ranks];
}

/*
 * three_of_a_kind_value_table is THREE_OF_A_KIND_VALUE ored with the top
 * 2 cards in the subscript.
 */

static inline uint32 three_of_a_kind_value(rank_set_t ranks, rank_set_t trips)
{
    return (trips << N_RANK) | three_of_a_kind_value_table[trips ^ ranks];
}

static inline uint32 two_pair_value(rank_set_t ranks, rank_set_t all_pairs)
{
    uint32 retval;

    retval = top_pair_table[all_pairs];
    return TWO_PAIR_VALUE | (retval << N_RANK) | top_bit_table[ranks^retval];
}

/*
 * pair_value_table has PAIR_VALUE or'd in.
 */

static inline uint32 pair_value(rank_set_t ranks, rank_set_t all_pairs)
{
    uint32 retval;

    retval = top_bit_table[all_pairs];
    return (retval << N_RANK) | pair_value_table[ranks^retval];
}

#define c clubs
#define d diamonds
#define h hearts
#define s spades

#endif
