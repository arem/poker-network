char rcsid_fish[] =
	"$Id$";

/*
 *  fish.c: a rewrite of Roy T. Hashimoto's classic "fish" poker hand
 *		evaluator.
 *
 *  This program is a rewrite of Roy T. Hashimoto's (hashimoto@sgi.com)
 *	fish.c program.  It primarily exists to test rank_eval, which
 *	is a faster version of the hand evaluator that was present in Roy's
 *	fish.c.  Since I had already written an evaluator that took significant
 *	cards into consideration, Roy's fish.c inspired me to write rank_eval
 *	as well, hence this test program, which provides the same functionality
 *	that the original fish.c program did.
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

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "poker.h"
#include "rank_eval.h"

PRIVATE uint32 totals[N_HAND];

PRIVATE boolean_t raw_flag = false;

PRIVATE void dump_totals(int sig)
{
    int i;
    uint32 grand_total;

    grand_total = 0;
    for (i = high_hand; i <= straight_flush; ++i)
	grand_total += totals[i];
    if (sig)
	putchar('\r');
    for (i = high_hand; i <= straight_flush; ++i)
	if (raw_flag)
	    printf("%d ", totals[i]);
	else
	    printf("%.2f ", 100.0 * totals[i] / grand_total);
    printf(": %d\n", grand_total);
}

PUBLIC int main( int argc, char *argv[] )
{
  uint8 i;
  uint8 n_cards;
  uint64 temp_card, dead_cards, pegged_cards;
  uint64 card1, card2, card3, card4, card5, card6, card7, card8, card9;
  uint64 n1, n2, n3, n4, n5, n6, n7, n8, n9;
  boolean_t seen_cards_already;

  n_cards = 7;
  dead_cards = 0;
  pegged_cards = 0;
  seen_cards_already = false;
  for (i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if (seen_cards_already) {
	  fprintf(stderr, "Cards must come before options\n");
	  exit(1);
      }
      if        (strcmp(argv[i], "-r") == 0) {
	raw_flag = true;
      } else if (strcmp(argv[i], "-n") == 0) {
	if (++i == argc) {
	  fprintf(stderr, "Missing numerical portion of -n\n");
	  exit(1);
	}
	n_cards = atoi(argv[i]);
      } else if (strcmp(argv[i], "-d") == 0) {
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
	pegged_cards |= temp_card;
	dead_cards   |= temp_card;
	--n_cards;
      }
    }
  }
  signal(SIGINT, (void *) dump_totals);
  n1    =    n2 =    n3 =    n4 =    n5 =    n6 =    n7 =    n8 =    n9 = 0;
  card1 = card2 = card3 = card4 = card5 = card6 = card7 = card8 = card9 = 0;
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
    n1    = pegged_cards;
    break;
  case 7:
    card3 = (uint64) 1 << 51;
    n2    = pegged_cards;
    break;
  case 6:
    card4 = (uint64) 1 << 51;
    n3    = pegged_cards;
    break;
  case 5:
    card5 = (uint64) 1 << 51;
    n4    = pegged_cards;
    break;
  case 4:
    card6 = (uint64) 1 << 51;
    n5    = pegged_cards;
    break;
  case 3:
    card7 = (uint64) 1 << 51;
    n6    = pegged_cards;
    break;
  case 2:
    card8 = (uint64) 1 << 51;
    n7    = pegged_cards;
    break;
  case 1:
    card9 = (uint64) 1 << 51;
    n8    = pegged_cards;
    break;
  case 0:
    n9    = pegged_cards;
    break;
  }
  switch (n_cards) {
  case 9:
    for (card1 =    (uint64) 1 << 51; card1 ; card1 >>= 1) {
      if (card1 & dead_cards)
/*-->*/	continue;
      n1 = card1 | pegged_cards;
      for (card2 = card1 >> 1; card2 ; card2 >>= 1) {
  case 8:
	if (card2 & dead_cards)
/*-->*/	  continue;
	n2 = n1 | card2;
	for (card3 = card2 >> 1; card3 ; card3 >>= 1) {
  case 7:
	  if (card3 & dead_cards)
/*-->*/	    continue;
	  n3 = n2 | card3;
	  for (card4 = card3 >> 1; card4 ; card4 >>= 1) {
  case 6:
	    if (card4 & dead_cards)
/*-->*/	      continue;
	    n4 = n3 | card4;
	    for (card5 = card4 >> 1; card5 ; card5 >>= 1) {
  case 5:
	      if (card5 & dead_cards)
/*-->*/	        continue;
	      n5 = n4 | card5;
	      for (card6 = card5 >> 1; card6 ; card6 >>= 1) {
  case 4:
		if (card6 & dead_cards)
/*-->*/	          continue;
		n6 = n5 | card6;
		for (card7 = card6 >> 1; card7 ; card7 >>= 1) {
  case 3:
		  if (card7 & dead_cards)
/*-->*/	            continue;
		  n7 = n6 | card7;
		  for (card8 = card7 >> 1; card8 ; card8 >>= 1) {
  case 2:
		    if (card8 & dead_cards)
/*-->*/	              continue;
		    n8 = n7 | card8;
		    for (card9 = card8 >> 1; card9 ; card9 >>= 1) {
  case 1:
		      if (card9 & dead_cards)
/*-->*/	                continue;
		      n9 = n8 | card9;
  case 0:
		      ++totals[rank_eval(n9)];
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
  dump_totals(0);
  return 0;
}
