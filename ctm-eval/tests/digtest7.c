/*
 *  digtest7: a test driver for eval, eval_n, and eval_7; computes MD5 checksum
 *
 *  Copyright (C) 1993-99 Clifford T. Matthews, Brian Goetz
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
#include	"poker.h"
#include        "eval.h"
#include        "eval_n.h"
#include	"eval7.h"
#include        "md5c.h"

int main( void )
{
  cards_u cards;
  eval_u evalu1, evalu2, evalu3;
  uint64 card1, card2, card3, card4, card5, pegged_cards;
  uint64 n2, n3, n4, n5;
  Md5Context ctx;
  Md5RawDigest raw;
  Md5CodedDigest coded;
  unsigned char hashvals[6];

  /* For this test, we loop through all combinations of seven card hands
     which include these two cards: Ad Ah.  These cards are arbitrary; 
     it just would take a long time to run through all seven-card combinations,
     so we make the search space smaller.  
   */

  pegged_cards = string_to_card("Ad") | string_to_card("Ah");

  ctx = MD5Begin();
  for (card1 =    (uint64) 1 << 51; card1 ; card1 >>= 1) {
    if (card1 & pegged_cards)
      continue;
    for (card2 = card1 >> 1; card2 ; card2 >>= 1) {
      if (card2 & pegged_cards)
        continue;
      n2 = pegged_cards | card1 | card2;
      for (card3 = card2 >> 1; card3 ; card3 >>= 1) {
        if (card3 & pegged_cards)
          continue;
	n3 = n2 | card3;
	for (card4 = card3 >> 1; card4 ; card4 >>= 1) {
          if (card4 & pegged_cards)
            continue;
	  n4 = n3 | card4;
	  for (card5 = card4 >> 1; card5 ; card5 >>= 1) {
            if (card5 & pegged_cards)
              continue;
	    n5 = n4 | card5;
	    cards.cards_n = 0;
	    cards.cards_t.hearts   =  n5 & 0x1FFF;
	    cards.cards_t.diamonds = (n5 >> 13) & 0x1FFF;
	    cards.cards_t.clubs    = (n5 >> 26) & 0x1FFF;
	    cards.cards_t.spades   = (n5 >> 39) & 0x1FFF;
            evalu1.eval_n = eval(cards);
	    evalu2.eval_n = new_eval_to_old_eval (
			      eval_exactly_7_cards (cards.cards_t.hearts,
						    cards.cards_t.spades,
						    cards.cards_t.diamonds,
						    cards.cards_t.clubs));
            evalu3.eval_n = eval_n(cards, 7);

            if (evalu1.eval_n != evalu2.eval_n) {
              fprintf(stderr, "eval() and eval7() do not agree\n");
              printf("0\n");
              dump_cards(cards);
              dump_eval(evalu1);
              dump_eval(evalu2);
              exit(0);
            };
	    if (evalu1.eval_n != evalu3.eval_n)
	      {
		fprintf(stderr, "eval() and eval_n() do not agree\n");
                printf("0\n");
		dump_cards(cards);
		dump_eval(evalu1);
		dump_eval(evalu2);
                exit(0);
	      }

            hashvals[0] = evalu1.eval_t.hand;
            hashvals[1] = evalu1.eval_t.top_card;
            hashvals[2] = evalu1.eval_t.second_card;
            hashvals[3] = evalu1.eval_t.third_card;
            hashvals[4] = evalu1.eval_t.fourth_card;
            hashvals[5] = evalu1.eval_t.fifth_card;
            MD5DigestBytes(ctx, hashvals, 6);
	  }
	}
      }
    }
  }
  MD5End(ctx, raw);
  MD5EncodeDigest(raw, coded);
  printf("%s\n", coded);
  exit(0);
}
