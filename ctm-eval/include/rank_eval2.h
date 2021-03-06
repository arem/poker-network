/*
 *  rank_eval2.h: a fast poker hand evaluator that doesn't care about
 *                 signficant cards (i.e. it only figures out the rank
 *                 of a hand).
 *
 *  This version of rank_eval takes four uint32 arguments instead of
 *	one uint64 argument.  This is a win on machines that don't handle
 *	64 bit ints quickly
 *
 *  Copyright (C) 1994  Clifford T. Matthews, Keith Miyake
 *
 *  Roy T. Hashimoto (hashimoto@sgi.com) is the inspiration for this
 *        evaluator.  Roy wrote the original fish.c, which had a Finite
 *        State Machine (FSM) based evaluator in it.  The code below doesn't
 *        look like Roy's, but the idea is the same.
 *
 *  Keith Miyake (kmiyake@cs.rutgers.edu) rearranged things to squeak
 *	some more speed out of this code.
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

#if        !defined(__FISH_EVAL__)
#define        __FISH_EVAL__

#include "poker.h"

static inline hand_t rank_eval(uint32 hearts,
			       uint32 diamonds,
			       uint32 clubs,
			       uint32 spades)
{
    hand_t retval, fsm_hand;
    unsigned long long n;
        
    n = cards_to_counts_table[spades  ] +
        cards_to_counts_table[clubs   ] +
        cards_to_counts_table[diamonds] +
        cards_to_counts_table[hearts  ];

    retval = straight_table[spades   |
                            clubs    |
                            diamonds |
                            hearts     ];

    retval = str_and_flu_table[retval][spades  ];
    retval = str_and_flu_table[retval][clubs   ];
    retval = str_and_flu_table[retval][diamonds];
    retval = str_and_flu_table[retval][hearts  ];

    fsm_hand = rank_fsm_table[high_hand][n & FSM_MASK];
    n >>= FSM_SHIFT;

    fsm_hand = rank_fsm_table[fsm_hand ][n & FSM_MASK];
    n >>= FSM_SHIFT;

    fsm_hand = rank_fsm_table[fsm_hand ][n & FSM_MASK];
    n >>= FSM_SHIFT;

    fsm_hand = rank_fsm_table[fsm_hand ][n];

    if (fsm_hand > retval)
        retval = fsm_hand;

    return retval;
}

#endif        /* !defined(__FISH_EVAL__) */
