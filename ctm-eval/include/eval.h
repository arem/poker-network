#if	!defined(__eval__)
#define	__eval__

#include "poker.h"

/*
 * When run over seven cards, here are the distribution of hands:
 *	  high hand: 23294460
 *	       pair: 58627800
 *	   two pair: 31433400
 *  three of a kind: 6461620
 *	   straight: 6180020
 *	      flush: 4047644
 *	 full house: 3473184
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

static inline uint32 is_straight( cards_u cards )
{
    unsigned int ranks;
    eval_u retval;

    retval.eval_n = 0;

    ranks = cards.cards_t.spades   |
	    cards.cards_t.clubs    |
	    cards.cards_t.diamonds |
	    cards.cards_t.hearts;

    if        ((ranks &   ACE_STRAIGHT_MASK) ==   ACE_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = ace;
    } else if ((ranks &  KING_STRAIGHT_MASK) ==  KING_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = king;
    } else if ((ranks & QUEEN_STRAIGHT_MASK) == QUEEN_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = queen;
    } else if ((ranks &  JACK_STRAIGHT_MASK) ==  JACK_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = jack;
    } else if ((ranks &   TEN_STRAIGHT_MASK) ==   TEN_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = ten;
    } else if ((ranks &  NINE_STRAIGHT_MASK) ==  NINE_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = nine;
    } else if ((ranks & EIGHT_STRAIGHT_MASK) == EIGHT_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = eight;
    } else if ((ranks & SEVEN_STRAIGHT_MASK) == SEVEN_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = seven;
    } else if ((ranks &   SIX_STRAIGHT_MASK) ==   SIX_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = six;
    } else if ((ranks &  FIVE_STRAIGHT_MASK) ==  FIVE_STRAIGHT_MASK) {
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

static inline uint32 three_helper( uint32 totest1, uint32 totest2,
				   uint32 totest3, uint32 slop )
{
    unsigned int ranks;
    eval_u retval;

    retval.eval_n = 0;
    ranks = totest1 & totest2 & totest3;
    if (ranks) {
	retval.eval_t.hand = three_of_a_kind;
	retval.eval_t.top_card = top_card_table[ranks];

	/* NOTE: we don't have to worry about there being a pair here, */
	/*	 because the full-house check won't care about anything */
	/*	 that we return except for hand and top_card */

	ranks ^= (totest1|totest2|totest3|slop);
	retval.eval_t.second_card = top_card_table[ranks];
	ranks ^= (1 << retval.eval_t.second_card);
	retval.eval_t.third_card = top_card_table[ranks];
    }
    return retval.eval_n;
}

static inline uint32 is_three_of_a_kind( cards_u cards )
{
    uint32 temp;
    eval_u retval;

    retval.eval_n = three_helper(cards.cards_t.spades,   cards.cards_t.clubs,
				 cards.cards_t.diamonds, cards.cards_t.hearts);

    temp = three_helper(cards.cards_t.spades, cards.cards_t.clubs,
			cards.cards_t.hearts, cards.cards_t.diamonds);
    if (temp > retval.eval_n)
	retval.eval_n = temp;

    temp = three_helper(cards.cards_t.spades,   cards.cards_t.hearts,
			cards.cards_t.diamonds, cards.cards_t.clubs);
    if (temp > retval.eval_n)
	retval.eval_n = temp;

    temp = three_helper(cards.cards_t.clubs,  cards.cards_t.diamonds,
			cards.cards_t.hearts, cards.cards_t.spades);
    if (temp > retval.eval_n)
	retval.eval_n = temp;

    return retval.eval_n;
}

static inline uint32 two_helper( uint32 totest1, uint32 totest2,
				 uint32 slop1,   uint32 slop2 )
{
    unsigned int ranks;
    eval_u retval;

    retval.eval_n = 0;
    ranks = totest1 & totest2;
    if (ranks) {
	retval.eval_t.hand     = pair;
	retval.eval_t.top_card = top_card_table[ranks];

	/* NOTE: we don't have to worry about there being anything better
		 than a pair left over */

	ranks ^= totest1 | totest2 | slop1 | slop2;
	retval.eval_t.second_card = top_card_table[ranks];
	ranks ^= (1 << retval.eval_t.second_card);
	retval.eval_t.third_card  = top_card_table[ranks];
	ranks ^= (1 << retval.eval_t.third_card);
	retval.eval_t.fourth_card = top_card_table[ranks];
    }

    return retval.eval_n;
}

static inline uint32 is_pair( cards_u cards )
{
    uint32 temp;
    eval_u retval;

    retval.eval_n = two_helper(cards.cards_t.spades,   cards.cards_t.clubs,
			       cards.cards_t.diamonds, cards.cards_t.hearts);

    temp = two_helper(cards.cards_t.spades, cards.cards_t.diamonds,
		      cards.cards_t.clubs,  cards.cards_t.hearts);
    if (temp > retval.eval_n)
	retval.eval_n = temp;

    temp = two_helper(cards.cards_t.spades, cards.cards_t.hearts,
		      cards.cards_t.clubs,  cards.cards_t.diamonds);
    if (temp > retval.eval_n)
	retval.eval_n = temp;

    temp = two_helper(cards.cards_t.clubs,  cards.cards_t.diamonds,
		      cards.cards_t.spades, cards.cards_t.hearts);
    if (temp > retval.eval_n)
	retval.eval_n = temp;

    temp = two_helper(cards.cards_t.clubs,  cards.cards_t.hearts,
		      cards.cards_t.spades, cards.cards_t.diamonds);
    if (temp > retval.eval_n)
	retval.eval_n = temp;

    temp = two_helper(cards.cards_t.diamonds, cards.cards_t.hearts,
		      cards.cards_t.spades,   cards.cards_t.clubs);
    if (temp > retval.eval_n)
	retval.eval_n = temp;

    return retval.eval_n;
}

static inline uint64 mask_rank_table( uint32 rank )
{
    cards_u temp;
    uint32 bit;

    bit = (1 << rank);
    temp.cards_t.spades   = bit;
    temp.cards_t.clubs    = bit;
    temp.cards_t.diamonds = bit;
    temp.cards_t.hearts   = bit;
    return ~temp.cards_n;
}

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
	}
    }
    if (retval.eval_t.hand != straight_flush) {
	tempeval.eval_n = is_four_of_a_kind(cards);
	if (tempeval.eval_n)
	    retval.eval_n = tempeval.eval_n;
	else {
	    tempeval.eval_n = is_three_of_a_kind(cards);
	    if (tempeval.eval_n) {
		tempcards.cards_n = cards.cards_n &
				  mask_rank_table(tempeval.eval_t.top_card);
		tempeval2.eval_n = is_pair(tempcards);
		if (tempeval2.eval_n) {
		    retval.eval_n = 0;
		    retval.eval_t.hand = full_house;
		    retval.eval_t.top_card = tempeval.eval_t.top_card;
		    retval.eval_t.second_card = tempeval2.eval_t.top_card;
		}
	    }
	}
	if (!retval.eval_n) {
	    retval.eval_n = is_straight(cards);
	    if (!retval.eval_n) {
		retval.eval_n = tempeval.eval_n;	/* three of a kind? */
		if (!retval.eval_n) {
		    retval.eval_n = is_pair(cards);
		    if (retval.eval_n) {
			tempcards.cards_n = cards.cards_n &
				       mask_rank_table(retval.eval_t.top_card);
			tempeval.eval_n = is_pair(tempcards);
			if (tempeval.eval_n) {
			    retval.eval_t.hand = two_pair;
			    retval.eval_t.second_card =
						      tempeval.eval_t.top_card;
			    tempcards.cards_n &=
				     mask_rank_table(tempeval.eval_t.top_card);
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
			retval.eval_n = top_five_cards_table[
						       cards.cards_t.spades   |
						       cards.cards_t.clubs    |
						       cards.cards_t.diamonds |
						       cards.cards_t.hearts
						       ];
			retval.eval_t.hand = high_hand;
		    }
		}
	    }
	}
    }

    return retval.eval_n;
}
