#include	<stdio.h>
#include	<signal.h>
#include	"poker.h"
#include	"eval.h"

uint32 totals[straight_flush+1];

void dump_totals(void)
{
    int i;

    putchar('\r');
    for (i = high_hand; i <= straight_flush; ++i)
	printf("%s:\t%d\n", hand_names[i], totals[i]);
}

int main( void )
{
  cards_u cards;
  eval_u evalu1;
  int ones;
  int millions;
  uint64 card1, card2, card3, card4, card5;
  uint64 n2, n3, n4, n5;

  ones = 0;
  millions = 0;
  signal(SIGINT, (void *) dump_totals);
  for (card1 =    (uint64) 1 << 51; card1 ; card1 >>= 1) {
    for (card2 = card1 >> 1; card2 ; card2 >>= 1) {
      n2 = card1 | card2;
      for (card3 = card2 >> 1; card3 ; card3 >>= 1) {
	n3 = n2 | card3;
	for (card4 = card3 >> 1; card4 ; card4 >>= 1) {
	  n4 = n3 | card4;
	  for (card5 = card4 >> 1; card5 ; card5 >>= 1) {
	    n5 = n4 | card5;
	    cards.cards_n = 0;
	    cards.cards_t.hearts   =  n5 & 0x1FFF;
	    cards.cards_t.diamonds = (n5 >> 13) & 0x1FFF;
	    cards.cards_t.clubs    = (n5 >> 26) & 0x1FFF;
	    cards.cards_t.spades   = (n5 >> 39) & 0x1FFF;
	    evalu1.eval_n = eval(cards);
	    ++totals[evalu1.eval_t.hand];
	  }
	}
      }
    }
  }
  dump_totals();
  exit(0);
}
