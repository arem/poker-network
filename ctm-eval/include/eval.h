/*
 *  eval.h: a fast poker hand evaluator
 *
 *  Copyright (C) 1993  Clifford T. Matthews, Keith Miyake
 *
 *  Cliff wrote the original code.
 *
 *  Keith significantly sped up is_straight, is_three_of_a_kind and is_pair,
 *	made mask_rank_table a real table and added returns to eval.
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

#if        !defined(__EVAL__)
#define        __EVAL__

#include "poker.h"

/*
 * When run over seven cards, here are the distribution of hands:
 *        high hand: 23294460
 *             pair: 58627800
 *         two pair: 31433400
 *  three of a kind: 6461620
 *         straight: 6180020
 *            flush: 4047644
 *       full house: 3473184
 *   four of a kind: 224848
 *   straight flush: 41584
 *
 */

static inline uint32 is_flush( cards_u cards, cards_u *flushp )
{
    eval_u retval;

    retval.eval_n = 0;

    if        (n_bits_table[cards.cards_t.spades  ] >= HAND_SIZE) {
        flushp->cards_n          = 0;
        flushp->cards_t.spades   = cards.cards_t.spades;
        retval.eval_t.hand       = flush;
        retval.eval_n           += top_five_cards_table[cards.cards_t.spades];
    } else if (n_bits_table[cards.cards_t.clubs   ] >= HAND_SIZE) {
        flushp->cards_n          = 0;
        flushp->cards_t.clubs    = cards.cards_t.clubs;
        retval.eval_t.hand       = flush;
        retval.eval_n           += top_five_cards_table[cards.cards_t.clubs];
    } else if (n_bits_table[cards.cards_t.diamonds] >= HAND_SIZE) {
        flushp->cards_n          = 0;
        flushp->cards_t.diamonds = cards.cards_t.diamonds;
        retval.eval_t.hand       = flush;
        retval.eval_n           += top_five_cards_table[cards.cards_t.diamonds];
    } else if (n_bits_table[cards.cards_t.hearts  ] >= HAND_SIZE) {
        flushp->cards_n          = 0;
        flushp->cards_t.hearts   = cards.cards_t.hearts;
        retval.eval_t.hand       = flush;
        retval.eval_n           += top_five_cards_table[cards.cards_t.hearts];
    }

    return retval.eval_n;
}

/*
 * is_straight checks for a straight by masking the ranks with four
 * copies of itself, each shifted one bit with respect to the
 * previous one.  So any sequence of five adjacent bits will still
 * be non-zero, but any gap will result in a zero value.  There's
 * a nice side-effect of leaving the top most bit set so we can use
 * it to set top_card.
 *
 */

static inline uint32 is_straight( cards_u cards )	/* Keith's */
{
    unsigned int ranks;
    unsigned int ranks2;
    eval_u retval;

    retval.eval_n = 0;

    ranks = cards.cards_t.spades   |
            cards.cards_t.clubs    |
            cards.cards_t.diamonds |
            cards.cards_t.hearts;

    if ( (ranks2  = ranks & (ranks << 1)) &&
	 (ranks2 &=         (ranks << 2)) &&
	 (ranks2 &=         (ranks << 3)) &&
	 (ranks2 &=         (ranks << 4)) ) {
        retval.eval_t.hand     = straight;
        retval.eval_t.top_card = top_card_table[ranks2];
    } else if ((ranks & FIVE_STRAIGHT_MASK) ==  FIVE_STRAIGHT_MASK) {
        retval.eval_t.hand     = straight;
        retval.eval_t.top_card = five;
    }

    return retval.eval_n;
}

static inline uint32 is_four_of_a_kind( cards_u cards )
{
    unsigned int ranks;
    eval_u retval;

    retval.eval_n = 0;

    ranks = cards.cards_t.spades   &
            cards.cards_t.clubs    &
            cards.cards_t.diamonds &
            cards.cards_t.hearts;

    if (ranks) {
        retval.eval_t.hand = four_of_a_kind;
        retval.eval_t.top_card = top_card_table[ranks];
        retval.eval_t.second_card = top_card_table[ranks ^
                                                      (cards.cards_t.spades   |
                                                       cards.cards_t.clubs    |
                                                       cards.cards_t.diamonds |
                                                       cards.cards_t.hearts )];
    }

    return retval.eval_n;
}

static inline uint32 is_three_of_a_kind( cards_u cards )
{
    unsigned int ranks;
    eval_u retval;

#define SC cards.cards_t.clubs
#define SD cards.cards_t.diamonds
#define SH cards.cards_t.hearts
#define SS cards.cards_t.spades

    retval.eval_n = 0;
    ranks = (( SC&SD )|( SH&SS )) & (( SC&SH )|( SD&SS ));
    if (ranks) {
        retval.eval_t.hand = three_of_a_kind;
        retval.eval_t.top_card = top_card_table[ranks];

        /* NOTE: we don't have to worry about there being a pair here, */
        /*       because the full-house check won't care about anything */
        /*       that we return except for hand and top_card */

        ranks ^= ( SC | SD | SH | SS );
        retval.eval_t.second_card = top_card_table[ranks];
        ranks ^= (1 << retval.eval_t.second_card);
        retval.eval_t.third_card = top_card_table[ranks];
    }

#undef SS
#undef SH
#undef SD
#undef SC

    return retval.eval_n;
}

static inline uint32 is_pair( cards_u cards )
{
    unsigned int ranks;
    eval_u retval;

#define SC cards.cards_t.clubs
#define SD cards.cards_t.diamonds
#define SH cards.cards_t.hearts
#define SS cards.cards_t.spades

    retval.eval_n = 0;
    ranks = ( SC&SD )|( SC&SH )|( SC&SS )|( SD&SH )|( SD&SS )|( SH&SS );
    if (ranks) {
        retval.eval_t.hand     = pair;
        retval.eval_t.top_card = top_card_table[ranks];

        /* NOTE: we don't have to worry about there being anything better
                 than a pair left over */

        ranks ^= ( SC | SD | SH | SS );
        retval.eval_t.second_card = top_card_table[ranks];
        ranks ^= (1 << retval.eval_t.second_card);
        retval.eval_t.third_card  = top_card_table[ranks];
        ranks ^= (1 << retval.eval_t.third_card);
        retval.eval_t.fourth_card = top_card_table[ranks];
    }

#undef SS
#undef SH
#undef SD
#undef SC

    return retval.eval_n;
}

#define	mask_rank(x)	(mask_rank_table[(x)])

static inline uint32 eval( cards_u cards )
{
    cards_u tempcards;
    eval_u retval, tempeval, tempeval2;

    retval.eval_n = is_flush(cards, &tempcards);
    if (retval.eval_n) {
        tempeval.eval_n = is_straight(tempcards);
        if (tempeval.eval_n) {
            retval.eval_n = 0;
            retval.eval_t.hand = straight_flush;
            retval.eval_t.top_card = tempeval.eval_t.top_card;
/*-->*/     return retval.eval_n;
        }
    } else
        retval.eval_n = is_straight(cards);

    tempeval.eval_n = is_four_of_a_kind(cards);
    if (tempeval.eval_n) {
        retval.eval_n = tempeval.eval_n;
/*-->*/ return retval.eval_n;
    }

    tempeval.eval_n = is_three_of_a_kind(cards);
    if (tempeval.eval_n) {
        tempcards.cards_n = cards.cards_n &
					   mask_rank(tempeval.eval_t.top_card);
        tempeval2.eval_n = is_pair(tempcards);
        if (tempeval2.eval_n) {
            retval.eval_n = 0;
            retval.eval_t.hand = full_house;
            retval.eval_t.top_card = tempeval.eval_t.top_card;
            retval.eval_t.second_card = tempeval2.eval_t.top_card;
/*-->*/     return retval.eval_n;
        }
    }

    if (retval.eval_n) /* flush and straight */
/*-->*/ return retval.eval_n;


    if (tempeval.eval_n) { /* three of a kind */
        retval.eval_n = tempeval.eval_n;
/*-->*/ return retval.eval_n;
    }

    retval.eval_n = is_pair(cards);
    if (retval.eval_n) {
        tempcards.cards_n = cards.cards_n & mask_rank(retval.eval_t.top_card);
        tempeval.eval_n = is_pair(tempcards);
        if (tempeval.eval_n) {
            retval.eval_t.hand = two_pair;
            retval.eval_t.second_card = tempeval.eval_t.top_card;
            tempcards.cards_n &= mask_rank(tempeval.eval_t.top_card);
            retval.eval_t.third_card = top_card_table[
                                       tempcards.cards_t.spades   |
                                       tempcards.cards_t.clubs    |
                                       tempcards.cards_t.diamonds |
                                       tempcards.cards_t.hearts
                                       ];
            retval.eval_t.fourth_card = 0;
            retval.eval_t.fifth_card = 0;
        }
    } else {
        retval.eval_n = top_five_cards_table[cards.cards_t.spades   |
                                             cards.cards_t.clubs    |
                                             cards.cards_t.diamonds |
                                             cards.cards_t.hearts
                                             ];
        retval.eval_t.hand = high_hand;
    }

    return retval.eval_n;
}

#endif        /* !defined(__EVAL__) */
