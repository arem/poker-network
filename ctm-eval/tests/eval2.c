/*
 *  eval2.c: a slow poker hand evaluator with which to compare eval.h
 *
 *  Copyright (C) 1993  Clifford T. Matthews
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

/*
 * Slow, brute force hand evaluator to compare my fast routines to
 */

#define	EXTRACT(base)						\
    do {							\
	base ## p->cards_n = 0;					\
	base ## p->cards_t. ## base = hand.cards_t. ## base;	\
    } while (0)

PRIVATE void split_into_suits(cards_u hand, cards_u *heartsp,
			 cards_u *diamondsp, cards_u *spadesp, cards_u *clubsp)
{
    EXTRACT(hearts);
    EXTRACT(diamonds);
    EXTRACT(spades);
    EXTRACT(clubs);
}

/*
 * STRAIGHT_MASK is the mask that we use to see if we have a straight.
 * It's five cards next to each other shifted left to the beginning
 * of a rank plus one more since we artificially expand things to
 * handle five-four-trey-deuce-ace.
 */

#define STRAIGHT_MASK	(0x1F << (N_RANK - 5 + 1))

/*
 * Tell whether or not we have a straight, and give
 * the top card of the straight, if we do.
 */

PRIVATE boolean_t is_straight(cards_u hand, eval_u *evalp)
{
    int ranks;
    boolean_t retval;
    int i;
    rank_t top_card;

    retval = false;
    ranks = hand.cards_t.hearts   | 
	    hand.cards_t.diamonds | 
	    hand.cards_t.spades   | 
	    hand.cards_t.clubs;

    ranks <<= 1;
    if (ranks & (1 << N_RANK))	/* see if they have an ace */
	ranks |= 1;
    for (i = 0, top_card = ace; i <= N_RANK - 5 + 1 && !retval ;
						++i, ranks <<= 1, --top_card) {
	if ((ranks & STRAIGHT_MASK) == STRAIGHT_MASK) {
	    evalp->eval_n = 0;
	    evalp->eval_t.hand = straight;
	    evalp->eval_t.top_card = top_card;
	    retval = true;
	}
    }
    return retval;
}

PRIVATE int ncards_in_hand(cards_u hand)
{
    uint64 ull;
    int retval;

    retval = 0;
    for (ull = 1; ull ; ull <<= 1)
	if (hand.cards_n & ull)
	    ++retval;
    return retval;
}

PRIVATE void fill_top_cards(eval_u *evalp, cards_u hand, int ntofill,
								     int start)
{
    int ranks, bit;
    rank_t rank;


    bit = 1 << (N_RANK-1);
    rank = ace;
    while (--ntofill >= 0) {
	ranks = hand.cards_t.hearts   |
		hand.cards_t.diamonds |
		hand.cards_t.spades   |
		hand.cards_t.clubs;
	while ((ranks & bit) == 0) {
	    bit >>= 1;
	    --rank;
	}
	if (hand.cards_t.hearts & bit) {
	    hand.cards_t.hearts &= ~bit;
	} else if (hand.cards_t.diamonds & bit) {
	    hand.cards_t.diamonds &= ~bit;
	} else if (hand.cards_t.clubs    & bit) {
	    hand.cards_t.clubs &= ~bit;
	} else if (hand.cards_t.spades   & bit) {
	    hand.cards_t.spades &= ~bit;
	}
	switch (start++) {
	case 1:
	    evalp->eval_t.top_card    = rank;
	    break;
	case 2:
	    evalp->eval_t.second_card = rank;
	    break;
	case 3:
	    evalp->eval_t.third_card  = rank;
	    break;
	case 4:
	    evalp->eval_t.fourth_card = rank;
	    break;
	case 5:
	    evalp->eval_t.fifth_card  = rank;
	    break;
	}
    }
}

PRIVATE boolean_t is_flush(cards_u hand, eval_u *evalp)
{
    boolean_t retval;
    cards_u hearts, diamonds, spades, clubs;

    split_into_suits(hand, &hearts, &diamonds, &spades, &clubs);
    retval = false;
    if (ncards_in_hand(hearts) >= 5) {
	evalp->eval_n = 0;
	fill_top_cards(evalp, hearts, 5, 1);
	retval = true;
    } else if (ncards_in_hand(diamonds) >= 5) {
	evalp->eval_n = 0;
	fill_top_cards(evalp, diamonds, 5, 1);
	retval = true;
    } else if (ncards_in_hand(spades) >= 5) {
	evalp->eval_n = 0;
	fill_top_cards(evalp, spades, 5, 1);
	retval = true;
    } else if (ncards_in_hand(clubs) >= 5) {
	evalp->eval_n = 0;
	fill_top_cards(evalp, clubs, 5, 1);
	retval = true;
    }
    if (retval)
	evalp->eval_t.hand = flush;
    return retval;
}

PRIVATE boolean_t is_straight_flush(cards_u hand, eval_u *evalp)
{
    boolean_t retval;
    cards_u hearts, diamonds, spades, clubs;

    retval = false;
    split_into_suits(hand, &hearts, &diamonds, &spades, &clubs);
    if (is_straight(hearts, evalp)) {
	retval = true;
    } else if (is_straight(diamonds, evalp)) {
	retval = true;
    } else if (is_straight(spades, evalp)) {
	retval = true;
    } else if (is_straight(clubs, evalp)) {
	retval = true;
    }
    if (retval)
	evalp->eval_t.hand = straight_flush;
    return retval;
}

PRIVATE int nrank_of_hand(cards_u hand, rank_t *top_cardp)
{
    rank_t card;
    int cardcount, tempcount;
    int i;

    for (i = N_RANK-1, card = ace, cardcount = 0; i >= 0; --i, --card) {
	tempcount = ((hand.cards_t.hearts   >> i) & 1) +
		    ((hand.cards_t.diamonds >> i) & 1) +
		    ((hand.cards_t.spades   >> i) & 1) +
		    ((hand.cards_t.clubs    >> i) & 1);
	if (tempcount > cardcount) {
	    cardcount = tempcount;
	    *top_cardp = card;
	}
    }
    return cardcount;
}

#define	ZAP(handp, suit, rank)				\
    do {						\
	handp->cards_t. ## suit &= ~(1 << rank);	\
    } while (0)

PRIVATE void subtract_rank(cards_u *handp, rank_t rank)
{
    ZAP(handp, hearts,   rank);
    ZAP(handp, diamonds, rank);
    ZAP(handp, spades,   rank);
    ZAP(handp, clubs,    rank);
}


PRIVATE boolean_t is_four_of_a_kind(cards_u hand, eval_u *evalp)
{
    rank_t rank;
    boolean_t retval;

    if (nrank_of_hand(hand, &rank) >= 4) {
	evalp->eval_n = 0;
	evalp->eval_t.hand = four_of_a_kind;
	evalp->eval_t.top_card = rank;
	subtract_rank(&hand, rank);
	fill_top_cards(evalp, hand, 1, 2);
	retval = true;
    } else
	retval = false;
    return retval;
}

PRIVATE boolean_t is_full_house(cards_u hand, eval_u *evalp)
{
    boolean_t retval;
    rank_t rank, rank2;
    
    retval = false;
    if (nrank_of_hand(hand, &rank) >= 3) {
	subtract_rank(&hand, rank);
	if (nrank_of_hand(hand, &rank2) >= 2) {
	    subtract_rank(&hand, rank2);
	    evalp->eval_n = 0;
	    evalp->eval_t.hand = full_house;
	    evalp->eval_t.top_card = rank;
	    evalp->eval_t.second_card = rank2;
	    retval = true;
	}
    }
    return retval;
}

PRIVATE boolean_t is_three_of_a_kind(cards_u hand, eval_u *evalp)
{
    boolean_t retval;
    rank_t rank;

    if (nrank_of_hand(hand, &rank) >= 3) {
	evalp->eval_n = 0;
	evalp->eval_t.hand = three_of_a_kind;
	evalp->eval_t.top_card = rank;
	subtract_rank(&hand, rank);
	fill_top_cards(evalp, hand, 2, 2);
	retval = true;
    } else
	retval = false;
    return retval;
}

PRIVATE boolean_t is_two_pair(cards_u hand, eval_u *evalp)
{
    boolean_t retval;
    rank_t rank, rank2;

    retval = false;
    if (nrank_of_hand(hand, &rank) >= 2) {
	subtract_rank(&hand, rank);
	if (nrank_of_hand(hand, &rank2) >= 2) {
	    evalp->eval_n = 0;
	    evalp->eval_t.hand = two_pair;
	    evalp->eval_t.top_card = rank;
	    evalp->eval_t.second_card = rank2;
	    subtract_rank(&hand, rank2);
	    fill_top_cards(evalp, hand, 1, 3);
	    retval = true;
	}
    }
    return retval;
}

PRIVATE boolean_t is_pair(cards_u hand, eval_u *evalp)
{
    boolean_t retval;
    rank_t rank;

    if (nrank_of_hand(hand, &rank) >= 2) {
	evalp->eval_n = 0;
	evalp->eval_t.hand = pair;
	evalp->eval_t.top_card = rank;
	subtract_rank(&hand, rank);
	fill_top_cards(evalp, hand, 3, 2);
	retval = true;
    } else
	retval = false;
    return retval;
}

PRIVATE boolean_t is_high_hand(cards_u hand, eval_u *evalp)
{
    evalp->eval_n = 0;
    evalp->eval_t.hand = high_hand;
    fill_top_cards(evalp, hand, 5, 1);
    return true;
}

PUBLIC uint32 eval2(cards_u hand)
{
    typedef boolean_t (*helper_func_t)(cards_u, eval_u *);
    helper_func_t evaluators[] = {
	is_straight_flush,
	is_four_of_a_kind,
	is_full_house,
	is_flush,
	is_straight,
	is_three_of_a_kind,
	is_two_pair,
	is_pair,
	is_high_hand,
    };
    eval_u retval;
    helper_func_t *hp;

    for (hp = evaluators; !(*hp)(hand, &retval) ; ++hp)
	;
    return retval.eval_n;
}
