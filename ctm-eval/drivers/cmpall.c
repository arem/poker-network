char rcsid_cmpall[] =
	"$Id$";

/*
 *  cmpall.c: a program to compare a set of hole cards to every possible
 *              other set of hole cards with every possible board.
 *
 *              NOTE:  This takes a lot of CPU time.
 *
 *  Copyright (C) 1994, 1995  Clifford T. Matthews
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

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "poker.h"
#include "eval7.h"

PUBLIC int main( int argc, char *argv[] )
{
  uint8 i;
  uint8 n_cards;
  uint64 temp_card, dead_cards, pegged_cards1, card3_or_card4,
			       card3_or_card4_or_pegged_cards1, new_dead_cards;
  uint64 card1, card2, card3, card4, card5, card6, card7, card8, card9;
  uint64 n1, n2, n3, n4, n5, n6, n7, n8, n9;
  boolean_t seen_cards_already;
  uint32 val1, val2, val1_count, val2_count, tie_count;
  cards_u cards;

  n_cards = 7;
  dead_cards = 0;
  pegged_cards1 = 0;
  val1_count = 0;
  val2_count = 0;
  tie_count = 0;
  seen_cards_already = false;
  for (i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if (seen_cards_already) {
	  fprintf(stderr, "Cards must come before options\n");
	  exit(1);
      }
      if (strcmp(argv[i], "-d") == 0) {
	if (++i == argc) {
	  fprintf(stderr, "Missing card portion of -d\n");
	  exit(1);
	}
	temp_card = string_to_card(argv[i]);
	if (!temp_card) {
	  fprintf(stderr, "Malformed card \"%s\"\n", argv[i]);
	  exit(1);
	} else {
	  dead_cards |= temp_card;
	}
      } else {
	  fprintf(stderr, "Unknown switch \"%s\"\n", argv[i]);
	  exit(1);
      }
    } else {
      temp_card = string_to_card(argv[i]);
      if (!temp_card) {
	fprintf(stderr, "Malformed card \"%s\"\n", argv[i]);
	exit(1);
      } else {
	if (n_cards >= 6)
	    pegged_cards1 |= temp_card;
	dead_cards   |= temp_card;
	--n_cards;
      }
    }
  }
  if (n_cards != 5) {
      fprintf(stderr, "Exactly four cards should have been given\n");
      exit(1);
  }
  n_cards = 7;
  n1    =    n2 =    n3 =    n4 =    n5 =    n6 =    n7 =    n8 =    n9 = 0;
  card1 = card2 = card3 = card4 = card5 = card6 = card7 = card8 = card9 = 0;
#if     0
  printf("%08x%08x\n", (int) (pegged_cards1 >> 32), (int) pegged_cards1);
#endif
  for (card3 = (uint64) 1 << 51; card3 ; card3 >>= 1) {
    if (card3 & dead_cards)
  /*-->*/           continue;
    for (card4 = card3 >> 1; card4 ; card4 >>= 1) {
      if (card4 & dead_cards)
  /*-->*/             continue;
      card3_or_card4 = card3|card4;
      card3_or_card4_or_pegged_cards1 = card3_or_card4 | pegged_cards1;
      new_dead_cards = card3_or_card4 | dead_cards;
      for (card5 = (uint64) 1 << 51; card5 ; card5 >>= 1) {
	if (card5 & new_dead_cards)
  /*-->*/               continue;
	n5 = card5 | pegged_cards1;
	for (card6 = card5 >> 1; card6 ; card6 >>= 1) {
	  if (card6 & new_dead_cards)
  /*-->*/                 continue;
	  n6 = n5 | card6;
	  for (card7 = card6 >> 1; card7 ; card7 >>= 1) {
	    if (card7 & new_dead_cards)
  /*-->*/                   continue;
	    n7 = n6 | card7;
	    for (card8 = card7 >> 1; card8 ; card8 >>= 1) {
	      if (card8 & new_dead_cards)
  /*-->*/                     continue;
	      n8 = n7 | card8;
	      for (card9 = card8 >> 1; card9 ; card9 >>= 1) {
		if (card9 & new_dead_cards)
  /*-->*/                       continue;
		n9 = n8 | card9;
		cards.cards_n = 0;
		val1  =  eval_exactly_7_cards( n9        & 0x1FFF,
			       (n9 >> 13) & 0x1FFF,
			       (n9 >> 26) & 0x1FFF,
			       (n9 >> 39) & 0x1FFF );

		n9 ^= card3_or_card4_or_pegged_cards1;
	       
		cards.cards_n = 0;
		val2 = eval_exactly_7_cards( n9        & 0x1FFF,
			     (n9 >> 13) & 0x1FFF,
			     (n9 >> 26) & 0x1FFF,
			     (n9 >> 39) & 0x1FFF );

		if (val1 > val2)
		    ++val1_count;
		else if (val2 > val1)
		    ++val2_count;
		else
		    ++tie_count;
	      }
	    }
	  }
	}
      }
    }
  }
  printf("%d %d (ties = %d)\n", val1_count, val2_count, tie_count);
  return 0;
}
