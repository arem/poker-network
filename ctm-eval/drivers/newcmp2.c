char rcsid_cmp2[] =
	"$Id$";

/*
 *  cmp2.c: a program to compare two pairs of hold'em hole cards at any
 *              point of the game (pre-flop, on the flop, turn or river).
 *              
 *  Example:
 *
 *      cmp2 tc ac 3h ah 8c 6h 7h
 *      517 439 (ties = 34)
 *
 *      Ten of Clubs and Ace of Clubs will beat
 *      Ace of Hearts and Three of Hearts 517 times, while it will
 *      lose 439 times and tie 34 times, if the flop comes Eight of Clubs,
 *      Six of Hearts, Seven of Hearts
 *
 *  Previous versions of cmp2 only allowed you to enter the two starting
 *  hole pairs.
 *
 *  Copyright (C) 1993, 1994  Clifford T. Matthews
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
#include "neweval.h"

PUBLIC int main( int argc, char *argv[] )
{
  uint8 i;
  uint8 n_cards;
  uint64 temp_card, dead_cards, pegged_cards1, pegged_cards2, peg1_or_peg2,
								 pegged_common;
  uint64 card1, card2, card3, card4, card5, card6, card7, card8, card9;
  uint64 n1, n2, n3, n4, n5, n6, n7, n8, n9;
  boolean_t seen_cards_already;
  uint32 val1, val2, val1_count, val2_count, tie_count;
  rank_set_t hearts, clubs, diamonds, spades;

  n_cards = 9;
  dead_cards = 0;
  pegged_cards1 = 0;
  pegged_cards2 = 0;
  pegged_common = 0;
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
	if (n_cards >= 8)
	    pegged_cards1 |= temp_card;
	else if (n_cards >= 6)
	    pegged_cards2 |= temp_card;
	else
	    pegged_common |= temp_card;
	dead_cards   |= temp_card;
	--n_cards;
      }
    }
  }
  n1    =    n2 =    n3 =    n4 =    n5 =    n6 =    n7 =    n8 =    n9 = 0;
  card1 = card2 = card3 = card4 = card5 = card6 = card7 = card8 = card9 = 0;
  peg1_or_peg2 = pegged_cards1 | pegged_cards2;

  switch (n_cards) {
  default:
    fprintf(stderr, "Number of cards to iterate must be "
		    "between 0 and 0 inclusive\n");
    exit(1);
  case 9:
    /* will set up normally below */
    break;
  case 8:
    card2 = (uint64) 1 << 51;
    n1    = pegged_cards1 | pegged_common;
    break;
  case 7:
    card3 = (uint64) 1 << 51;
    n2    = pegged_cards1 | pegged_common;
    break;
  case 6:
    card4 = (uint64) 1 << 51;
    n3    = pegged_cards1 | pegged_common;
    break;
  case 5:
    card5 = (uint64) 1 << 51;
    n4    = pegged_cards1 | pegged_common;
    break;
  case 4:
    card6 = (uint64) 1 << 51;
    n5    = pegged_cards1 | pegged_common;
    break;
  case 3:
    card7 = (uint64) 1 << 51;
    n6    = pegged_cards1 | pegged_common;
    break;
  case 2:
    card8 = (uint64) 1 << 51;
    n7    = pegged_cards1 | pegged_common;
    break;
  case 1:
    card9 = (uint64) 1 << 51;
    n8    = pegged_cards1 | pegged_common;
    break;
  case 0:
    n9    = pegged_cards1 | pegged_common;
    break;
  }
  switch (n_cards) {
  case 9:
    for (card1 =    (uint64) 1 << 51; card1 ; card1 >>= 1) {
      if (card1 & dead_cards)
/*-->*/ continue;
      n1 = card1 | pegged_cards1;
      for (card2 = card1 >> 1; card2 ; card2 >>= 1) {
  case 8:
	if (card2 & dead_cards)
/*-->*/   continue;
	n2 = n1 | card2;
	for (card3 = card2 >> 1; card3 ; card3 >>= 1) {
  case 7:
	  if (card3 & dead_cards)
/*-->*/     continue;
	  n3 = n2 | card3;
	  for (card4 = card3 >> 1; card4 ; card4 >>= 1) {
  case 6:
	    if (card4 & dead_cards)
/*-->*/       continue;
	    n4 = n3 | card4;
	    for (card5 = card4 >> 1; card5 ; card5 >>= 1) {
  case 5:
	      if (card5 & dead_cards)
/*-->*/         continue;
	      n5 = n4 | card5;
	      for (card6 = card5 >> 1; card6 ; card6 >>= 1) {
  case 4:
		if (card6 & dead_cards)
/*-->*/           continue;
		n6 = n5 | card6;
		for (card7 = card6 >> 1; card7 ; card7 >>= 1) {
  case 3:
		  if (card7 & dead_cards)
/*-->*/             continue;
		  n7 = n6 | card7;
		  for (card8 = card7 >> 1; card8 ; card8 >>= 1) {
  case 2:
		    if (card8 & dead_cards)
/*-->*/               continue;
		    n8 = n7 | card8;
		    for (card9 = card8 >> 1; card9 ; card9 >>= 1) {
  case 1:
		      if (card9 & dead_cards)
/*-->*/                 continue;
		      n9 = n8 | card9;
  case 0:
		      hearts   =  n9        & 0x1FFF;
		      diamonds = (n9 >> 13) & 0x1FFF;
		      clubs    = (n9 >> 26) & 0x1FFF;
		      spades   = (n9 >> 39) & 0x1FFF;
		      val1 = new_eval(hearts, diamonds, clubs, spades);

		      n9 ^= peg1_or_peg2;

		      hearts   =  n9        & 0x1FFF;
		      diamonds = (n9 >> 13) & 0x1FFF;
		      clubs    = (n9 >> 26) & 0x1FFF;
		      spades   = (n9 >> 39) & 0x1FFF;
		      val2 = new_eval(hearts, diamonds, clubs, spades);

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
      }
    }
  }
  printf("%d %d (ties = %d)\n", val1_count, val2_count, tie_count);
  return 0;
}
