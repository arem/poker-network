#include "poker.h"

static inline uint32 is_flush( cards_u cards, cards_u *flushp )
{
    eval_u retval;

    retval.eval_n = 0;

    if        (n_bits_in_13[cards.cards_t.spades  ] >= 5) {
	flushp->cards_n     = cards.cards_t.spades;
	retval.eval_t.hand  = flush;
	retval.eval_n      += top_five_cards_in_13[cards.cards_t.spades];
    } else if (n_bits_in_13[cards.cards_t.clubs   ] >= 5) {
	flushp->cards_n     = cards.cards_t.clubs;
	retval.eval_t.hand  = flush;
	retval.eval_n      += top_five_cards_in_13[cards.cards_t.spades];
    } else if (n_bits_in_13[cards.cards_t.diamonds] >= 5) {
	flushp->cards_n     = cards.cards_t.diamonds;
	retval.eval_t.hand  = flush;
	retval.eval_n      += top_five_cards_in_13[cards.cards_t.spades];
    } else if (n_bits_in_13[cards.cards_t.hearts  ] >= 5) {
	flushp->cards_n     = cards.cards_t.hearts;
	retval.eval_t.hand  = flush;
	retval.eval_n      += top_five_cards_in_13[cards.cards_t.spades];
    }

    return retval.eval_n;
}

static inline uint32 is_straight( cards_u cards )
{
    unsigned int ranks;
    eval_u retval;

    retval.eval_n = 0;

    ranks = cards.cards_t.spades | cards.cards_t.clubs | cards.cards_t.diamonds | cards.cards_t.hearts;
    if        (ranks &   ACE_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = ace;
    } else if (ranks &  KING_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = king;
    } else if (ranks & QUEEN_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = queen;
    } else if (ranks &  JACK_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = jack;
    } else if (ranks &   TEN_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = ten;
    } else if (ranks &  NINE_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = nine;
    } else if (ranks & EIGHT_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = eight;
    } else if (ranks & SEVEN_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = seven;
    } else if (ranks &   SIX_STRAIGHT_MASK) {
	retval.eval_t.hand     = straight;
	retval.eval_t.top_card = six;
    } else if (ranks &  FIVE_STRAIGHT_MASK) {
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

    ranks = cards.cards_t.spades & cards.cards_t.clubs & cards.cards_t.diamonds & cards.cards_t.hearts;
    if (ranks) {
	retval.eval_t.hand = four_of_a_kind;
	retval.eval_t.top_card = top_card_in_13[ranks];
	retval.eval_t.second_card = top_card_in_13[ranks ^
		(cards.cards_t.spades | cards.cards_t.clubs | cards.cards_t.diamonds | cards.cards_t.hearts )];
    }

    return retval.eval_n;
}

static inline uint32 is_three_of_a_kind( cards_u cards )
{
    unsigned int ranks;
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

static inline uint32 is_pair( cards_u cards )
{
    unsigned int ranks;
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

static inline long long all_rank_in_13(uint32 rank)
{
    long long retval;
    retval = (1 << rank) | ( 1 << (rank + 13));
    retval |= retval << 26;
    return retval;
}

static inline uint32 eval( cards_u cards )
{
    cards_u tempcards;
    eval_u retval, tempeval, tempeval2;

    retval.eval_n = is_flush(cards, &tempcards);
    if (retval.eval_n) {
	tempeval.eval_n = is_straight(tempcards);
	if (tempeval.eval_n) {
	    retval.eval_t.hand = straight_flush;
	    retval.eval_t.top_card = tempeval.eval_t.top_card;
	} else {
	    tempeval.eval_n = is_four_of_a_kind(cards);
	    if (tempeval.eval_n)
		retval.eval_n = tempeval.eval_n;
	    else {
		tempeval.eval_n = is_three_of_a_kind(cards);
		if (tempeval.eval_n) {
		    tempcards.cards_n = cards.cards_n ^
				      all_rank_in_13(tempeval.eval_t.top_card);
		    tempeval2.eval_n = is_two_of_a_kind(tempcards);
		    if (tempeval2.eval_n) {
			retval.eval_t.hand = full_house;
			retval.eval_t.top_card = tempeval.eval_t.top_card;
			retval.eval_t.second_card = tempeval2.eval_t.top_card;
		    }
		}
	    }
	}
    } else {
	retval.eval_n = is_straight(tempcards);
	if (!retval.eval_n) {
	    retval.eval_n = is_three_of_a_kind(cards);
	    if (!retval.eval_n) {
		retval.eval_n = is_pair(cards);
		if (retval.eval_n) {
		    tempcards.cards_n = cards.cards_n ^
					all_rank_in_13(retval.eval_t.top_card);
		    tempeval.eval_n = is_pair(tempcards);
		    if (tempeval.eval_n) {
			retval.eval_t.hand = two_pair;
			retval.eval_t.second_card = tempeval.eval_t.top_card;
			tempcards.cards_n ^=
				      all_rank_in_13(tempeval.eval_t.top_card);
			retval.eval_t.third_card = top_card_in_13[
						   tempcards.cards_t.spades   |
						   tempcards.cards_t.clubs    |
						   tempcards.cards_t.diamonds |
						   tempcards.cards_t.hearts
						   ];
		    }
		} else {
		    retval.eval_n = top_five_cards_in_13[
						       cards.cards_t.spades   |
						       cards.cards_t.clubs    |
						       cards.cards_t.diamonds |
						       cards.cards_t.hearts
						       ];
		}
	    }
	}
    }

    return retval.eval_n;
}
