#include <stdio.h>

#include "poker.h"
#include "eval.h"

extern int random( void );

int main( void )
{
    extern uint32 eval2(cards_u);

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
    /* NEVER REACHED */
    return 0;
}
