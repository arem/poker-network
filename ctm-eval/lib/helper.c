#include "poker.h"
#include <stdio.h>

PUBLIC void dump_rank( int ranks, char suitchar )
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

PUBLIC void dump_cards( cards_u cards )
{
    dump_rank(cards.cards_t.hearts,   'H');
    dump_rank(cards.cards_t.diamonds, 'D');
    dump_rank(cards.cards_t.spades,   'S');
    dump_rank(cards.cards_t.clubs,    'C');
    putchar('\n');
}

PUBLIC const char *hand_names[] = {
    "internal error",
    "high hand",
    "pair",
    "two pair",
    "three of a kind",
    "straight",
    "flush",
    "full house",
    "four of a kind",
    "straight flush",
};

PUBLIC void dump_eval( eval_u eval )
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
