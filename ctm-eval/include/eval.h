#include	<stdio.h>
#include	<signal.h>
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

    ranks = cards.cards_t.spades & cards.cards_t.clubs & cards.cards_t.diamonds & cards.cards_t.hearts;
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

static inline uint64 mask_rank_table(uint32 rank)
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

static void dump_rank( int ranks, char suitchar )
{
    int bit;
    const char *namep;
    static char names[] = "AKQJT98765432";

    for (bit = 1 << (N_RANK - 1), namep = names; bit; bit >>= 1, ++namep) {
	if (ranks & bit) {
	    putchar(*namep);
	    putchar(suitchar);
	    putchar(' ');
	}
    }
}

static void dump_cards( cards_u cards )
{
    dump_rank(cards.cards_t.hearts,   'H');
    dump_rank(cards.cards_t.diamonds, 'D');
    dump_rank(cards.cards_t.spades,   'S');
    dump_rank(cards.cards_t.clubs,    'C');
    putchar('\n');
}

static const char *hand_names[] = {
    "internal error*",
    "      high hand",
    "           pair",
    "       two pair",
    "three of a kind",
    "       straight",
    "          flush",
    "     full house",
    " four of a kind",
    " straight flush",
};

static void dump_eval( eval_u eval )
{
    static const char *rank_names[] = {
	"deuce",
	"trey",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine",
	"ten",
	"jack",
	"queen",
	"king",
	"ace",
    };
    printf("%s: %s", hand_names[eval.eval_t.hand],
					     rank_names[eval.eval_t.top_card]);
    if (eval.eval_t.second_card)
	printf(", %s", rank_names[eval.eval_t.second_card]);
    if (eval.eval_t.third_card)
	printf(", %s", rank_names[eval.eval_t.third_card]);
    if (eval.eval_t.fourth_card)
	printf(", %s", rank_names[eval.eval_t.fourth_card]);
    if (eval.eval_t.fifth_card)
	printf(", %s", rank_names[eval.eval_t.fifth_card]);

    printf("\n");
}

uint64 totals[straight_flush+1];

void dump_totals(void)
{
    int i;

    putchar('\r');
    for (i = high_hand; i <= straight_flush; ++i)
	printf("%s:\t%9ld\n", hand_names[i], totals[i]);
}

int main( void )
{
    extern uint32 eval2(cards_u);

#if	defined(RANDOMTEST)
    cards_u cards, random_card;
    eval_u evalu1, evalu2;
    int i, n, bit;
    int ones;
    int millions;

    ones = 0;
    millions = 0;
    while (true) {
	cards.cards_n = 0;
	for (i = 7; --i >= 0;) {
	    do {
		do
		    n = random() & 63;
		while (n >= 4 * N_RANK);
		bit = 1 << n % N_RANK;
		random_card.cards_n = 0;
		if (n < 26)
		    if (n < 13)
			random_card.cards_t.hearts   = bit;
		    else
			random_card.cards_t.diamonds = bit;
		else
		    if (n < 39)
			random_card.cards_t.clubs    = bit;
		    else
			random_card.cards_t.spades   = bit;
	    } while (cards.cards_n & random_card.cards_n);
	    cards.cards_n |= random_card.cards_n;
	}
	evalu1.eval_n = eval(cards);
	evalu2.eval_n = eval2(cards);
	if (evalu1.eval_n != evalu2.eval_n) {
	    printf("%d,%d bad match for ", millions, ones);
	    dump_cards(cards);
	    dump_eval(evalu1);
	    dump_eval(evalu2);
	}
	if (++ones == 1000000) {
	    ++millions;
	    printf("%d\r", millions);
	    fflush(stdout);
	    ones = 0;
	}
    }
#else	/* !defined(RANDOMTEST) */
  cards_u cards;
  eval_u evalu1, evalu2;
  int ones;
  int millions;
  uint64 card1, card2, card3, card4, card5, card6, card7;
  uint64 n2, n3, n4, n5, n6, n7;

  ones = 0;
  millions = 0;
  signal(SIGINT, (void *) dump_totals);
  for (card1 =    1L << 51; card1 ; card1 >>= 1) {
    for (card2 = card1 >> 1; card2 ; card2 >>= 1) {
      n2 = card1 | card2;
      for (card3 = card2 >> 1; card3 ; card3 >>= 1) {
	n3 = n2 | card3;
	for (card4 = card3 >> 1; card4 ; card4 >>= 1) {
	  n4 = n3 | card4;
	  for (card5 = card4 >> 1; card5 ; card5 >>= 1) {
	    n5 = n4 | card5;
	    for (card6 = card5 >> 1; card6 ; card6 >>= 1) {
	      n6 = n5 | card6;
	      for (card7 = card6 >> 1; card7 ; card7 >>= 1) {
		n7 = n6 | card7;
		cards.cards_n = 0;
		cards.cards_t.hearts   =  n7 & 0x1FFF;
		cards.cards_t.diamonds = (n7 >> 13) & 0x1FFF;
		cards.cards_t.clubs    = (n7 >> 26) & 0x1FFF;
		cards.cards_t.spades   = (n7 >> 39) & 0x1FFF;
		evalu1.eval_n = eval(cards);
		evalu2.eval_n = eval2(cards);
		if (evalu1.eval_n != evalu2.eval_n) {
		    printf("%d,%d bad match for ", millions, ones);
		    dump_cards(cards);
		    dump_eval(evalu1);
		    dump_eval(evalu2);
		}
		if (++ones == 1000000) {
		    ++millions;
		    printf("%d\r", millions);
		    fflush(stdout);
		    ones = 0;
		}
		++totals[evalu1.eval_t.hand];
	      }
	    }
	  }
	}
      }
    }
  }
  dump_totals();
#endif	/* !defined(RANDOMTEST) */
}

#if	0
static uint64 stats[13][13][10];

int main( void )
{
#if	0
    eval_u to_eval;
    cards_u cards;

    cards.cards_n = 0;
    cards.cards_t.spades = (1 << ten) | (1 << nine) | (1 << eight) |
				(1 << seven) | (1 << six);
    cards.cards_t.clubs =  (1 << trey);
    cards.cards_t.hearts = (1 << trey) | (1 << five);
    to_eval.eval_n = eval(cards);
    dump_eval(to_eval);	/* straight flush */

    cards.cards_n = 0;
    cards.cards_t.spades = (1 << ten) | (1 << nine) | (1 << eight) |
				(1 << seven) | (1 << four);
    cards.cards_t.clubs =  (1 << four);
    cards.cards_t.hearts = (1 << four);
    cards.cards_t.diamonds = (1 << four);
    to_eval.eval_n = eval(cards);
    dump_eval(to_eval);	/* four of a kind */

    cards.cards_n = 0;
    cards.cards_t.spades = (1 << ten) | (1 << nine) | (1 << eight) |
				(1 << seven) | (1 << four);
    cards.cards_t.clubs =  (1 << four);
    cards.cards_t.hearts = (1 << four);
    cards.cards_t.diamonds = (1 << nine);
    to_eval.eval_n = eval(cards);
    dump_eval(to_eval);	/* full house */

    cards.cards_n = 0;
    cards.cards_t.spades = (1 << ace) | (1 << nine) | (1 << eight) |
				(1 << seven) | (1 << six) | (1 << deuce);
    cards.cards_t.hearts = (1 << trey) | (1 << five);
    to_eval.eval_n = eval(cards);
    dump_eval(to_eval);	/* flush */

    cards.cards_n = 0;
    cards.cards_t.spades = (1 << seven) | (1 << six) | (1 << deuce);
    cards.cards_t.hearts = (1 << trey) | (1 << five);
    cards.cards_t.clubs = (1 << ace) | (1 << nine) | (1 << eight);
    to_eval.eval_n = eval(cards);
    dump_eval(to_eval);	/* straight */

    cards.cards_t.spades = (1 << deuce) | (1 << trey);
    cards.cards_t.clubs =  (1 << trey);
    cards.cards_t.hearts = (1 << trey) | (1 << five);
    cards.cards_t.diamonds = (1 << ace) | (1 << king);

    to_eval.eval_n = eval(cards);
    dump_eval(to_eval);	/* three of a kind */
    
    cards.cards_t.spades = (1 << deuce) | (1 << trey);
    cards.cards_t.clubs =  (1 << king);
    cards.cards_t.hearts = (1 << trey) | (1 << five);
    cards.cards_t.diamonds = (1 << ace) | (1 << king);

    to_eval.eval_n = eval(cards);
    dump_eval(to_eval);	/* two pair */
    
    cards.cards_t.spades = (1 << deuce) | (1 << trey);
    cards.cards_t.clubs =  (1 << king);
    cards.cards_t.hearts = (1 << six) | (1 << five);
    cards.cards_t.diamonds = (1 << ace) | (1 << king);

    to_eval.eval_n = eval(cards);
    dump_eval(to_eval);	/* pair */
    
    cards.cards_t.spades = (1 << deuce) | (1 << trey);
    cards.cards_t.clubs =  (1 << queen);
    cards.cards_t.hearts = (1 << six) | (1 << five);
    cards.cards_t.diamonds = (1 << ace) | (1 << king);

    to_eval.eval_n = eval(cards);
    dump_eval(to_eval);	/* high hand */
#else
  uint64 card1, card2, card3, card4, card5, card6, card7, card1or2,
			  raw_cards;
  uint64 *statp;
  int i, j, k;
  cards_u cards;
  eval_u to_eval;

/* first suited cards */
  for (card1 = 1 << 12, i = 0; card1 ; card1 >>= 1, ++i) {
    for (card2 = card1 >> 1, j = i + 1; card2 ; card2 >>= 1, ++j) {
      card1or2 = card1 | card2;
      statp = stats[i][j];
      for (card3 = 1LL << 51; card3 ; card3 >>= 1) {
	if (card3 & card1or2)
	  continue;
	for (card4 = card3 >> 1; card4 ; card4 >>= 1) {
	  if (card4 & card1or2)
	    continue;
	  for (card5 = card4 >> 1; card5 ; card5 >>= 1) {
	    if (card5 & card1or2)
	      continue;
	    for (card6 = card5 >> 1; card6 ; card6 >>= 1) {
	      if (card6 & card1or2)
		continue;
	      for (card7 = card6 >> 1; card7 ; card7 >>= 1) {
		if (card7 & card1or2)
		  continue;
		raw_cards = card1|card2|card3|card4|card5|card6|card7;
		cards.cards_t.spades   = (raw_cards >>  0) << 1;
		cards.cards_t.clubs    = (raw_cards >> 13) << 1;
		cards.cards_t.diamonds = (raw_cards >> 26) << 1;
		cards.cards_t.hearts   = (raw_cards >> 39) << 1;
		to_eval.eval_n = eval(cards);
		++statp[to_eval.eval_t.hand];
	      }
	    }
	  }
	}
      }
    }
  }
  for (i = 0; i < 13; ++i)
      for (j = i+1; j < 13; ++j) {
	  printf("(%d, %d) ", i, j);
	  for (k = 1; k < 10; ++k)
	      printf("%ld%c", stats[i][j][k], k == 9 ? '\n' : '\t');
      }
#endif
    
  return 0;
}
#endif
