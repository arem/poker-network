/*
 *  rank_eval.h: a fast poker hand evaluator that doesn't care about
 *		 signficant cards (i.e. it only figures out the rank
 *		 of a hand).
 *
 *  Copyright (C) 1993  Clifford T. Matthews
 *
 *  Roy T. Hashimoto (hashimoto@sgi.com) is the inspiration for this
 *	evaluator.  Roy wrote the original fish.c, which had a Finite
 *	State Machine (FSM) based evaluator in it.  The code below doesn't
 *	look like Roy's, but the idea is the same.
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

#if	!defined(__FISH_EVAL__)
#define	__FISH_EVAL__

#include "fsm.h"

static inline hand_t rank_eval(cards_u hand)
{
    hand_t retval, fsm_hand;
    unsigned long long n;

    retval = str_and_flu_table[high_hand][hand.cards_t.spades  ];
    retval = str_and_flu_table[retval   ][hand.cards_t.clubs   ];
    retval = str_and_flu_table[retval   ][hand.cards_t.diamonds];
    retval = str_and_flu_table[retval   ][hand.cards_t.hearts  ];

    if (retval < straight_flush) {
	n = cards_to_counts_table[hand.cards_t.spades  ] +
	    cards_to_counts_table[hand.cards_t.clubs   ] +
	    cards_to_counts_table[hand.cards_t.diamonds] +
	    cards_to_counts_table[hand.cards_t.hearts  ];

        fsm_hand = straight_table[hand.cards_t.spades   |
				  hand.cards_t.clubs    |
				  hand.cards_t.diamonds |
				  hand.cards_t.hearts     ];

	fsm_hand = rank_fsm_table[fsm_hand][n & FSM_MASK];
	n >>= FSM_SHIFT;
	fsm_hand = rank_fsm_table[fsm_hand][n & FSM_MASK];
	n >>= FSM_SHIFT;
	fsm_hand = rank_fsm_table[fsm_hand][n & FSM_MASK];
	n >>= FSM_SHIFT;
	fsm_hand = rank_fsm_table[fsm_hand][n];

	if (fsm_hand > retval)
	    retval = fsm_hand;
    }

    return retval;
}

#endif	/* !defined(__FISH_EVAL__) */
