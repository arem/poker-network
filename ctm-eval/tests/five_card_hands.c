/*
 *  five_card_hands.c: a test driver for eval.h
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


#include	<stdio.h>
#include	<signal.h>
#include	"eval5.h"
#include	"poker.h"
#include	"eval.h"

uint32 totals[N_HAND];

void dump_totals(void)
{
    int i;

    putchar('\r');
    for (i = high_hand; i <= straight_flush; ++i)
	printf("%s:\t%d\n", hand_info[i].hand_name, totals[i]);
}

int main( void )
{
  cards_u cards;
  eval_u evalu1, evalu2;
  uint64 card1, card2, card3, card4, card5;
  uint64 n2, n3, n4, n5;

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
	    evalu2.eval_n = new_eval_to_old_eval (
			      eval_exactly_5_cards (cards.cards_t.hearts,
						    cards.cards_t.spades,
						    cards.cards_t.diamonds,
						    cards.cards_t.clubs));
	    if (evalu1.eval_n != evalu2.eval_n)
	      {
		printf("bad match");
		dump_cards(cards);
		dump_eval(evalu1);
		dump_eval(evalu2);
	      }
	    ++totals[evalu1.eval_t.hand];
	  }
	}
      }
    }
  }
  dump_totals();
  exit(0);
}
