char rcsid_caro2[] =
	"$Id$";

/*
 *  caro2b.c: a variation of caro2 that crunches through the pattern that
 *	Keith Miyake's best entry to date uses.
 *              
 *  Copyright (C) 1993 - 1997 Clifford T. Matthews
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
#include <string.h>
#include <ctype.h>

#include "poker.h"
#include "eval7.h"

/* Speed doesn't matter for poker_lcd.
   Barbara Yoon pointed out that there's no point in worrying
   about pots split more than 4 ways (other than a complete n-way
   split), so instead of getting the lcd of the first positive
   integers, we only do the lcd of (2, 3, 4, n). */

PRIVATE int
poker_lcd (int n)
{
  int retval, i;
  boolean_t done;

  retval = n;
  done = false;

  while (!done)
    {
      done = true;
      for (i = 2; done && i <= n; ++i)
	if ((i <= 4 || i == n) && retval % i)
	  {
	    done = false;
	    ++retval;
	  }
    }
  return retval;
}

#define MACRO_INSTEAD_OF_INLINE
#if !defined (MACRO_INSTEAD_OF_INLINE)

PRIVATE inline void
extr_suits (uint64 cards, rank_set_t *heartsp, rank_set_t *diamondsp,
	                  rank_set_t *clubsp,  rank_set_t *spadesp)
{
  *heartsp   =  cards        & 0x1FFF;
  *diamondsp = (cards >> 13) & 0x1FFF;
  *clubsp    = (cards >> 26) & 0x1FFF;
  *spadesp   = (cards >> 39) & 0x1FFF;
}

#else

#define extr_suits(cards, heartsp, diamondsp, clubsp, spadesp)	\
do								\
{								\
  *(heartsp)   =  (cards)        & 0x1FFF;			\
  *(diamondsp) = ((cards) >> 13) & 0x1FFF;			\
  *(clubsp)    = ((cards) >> 26) & 0x1FFF;			\
  *(spadesp)   = ((cards) >> 39) & 0x1FFF;			\
}								\
while (false)

#endif

PRIVATE int
card_compare (const void *p1, const void *p2)
{
  uint64 cards1, cards2;
  int retval;

  cards1 = *(uint64 *)p1;
  cards2 = *(uint64 *)p2;
  if (cards1 < cards2)
    retval = -1;
  else if (cards1 > cards2)
    retval = 1;
  else
    retval = 0;

  return retval;
}

enum
{
  HEART_TO_HEART = 0,
  HEART_TO_DIAMOND = 13,
  HEART_TO_CLUB = 26,
  HEART_TO_SPADE = 39,

  DIAMOND_TO_HEART = 0,
  DIAMOND_TO_DIAMOND = 13,
  DIAMOND_TO_CLUB = 26,
  DIAMOND_TO_SPADE = 39,

  CLUB_TO_HEART = 0,
  CLUB_TO_DIAMOND = 13,
  CLUB_TO_CLUB = 26,
  CLUB_TO_SPADE = 39,

  SPADE_TO_HEART = 0,
  SPADE_TO_DIAMOND = 13,
  SPADE_TO_CLUB = 26,
  SPADE_TO_SPADE = 39,
};

PRIVATE inline void
normalize_suits (uint64 *cardsp, int hs, int ds, int cs, int ss)
{
  rank_set_t temp_hearts, temp_diamonds, temp_clubs, temp_spades;

  extr_suits (*cardsp, &temp_hearts, &temp_diamonds,
	               &temp_clubs , &temp_spades);

  *cardsp = (((uint64) temp_hearts    << hs) |
	      ((uint64) temp_diamonds << ds) |
	      ((uint64) temp_clubs    << cs) |
	      ((uint64) temp_spades   << ss));
}

PRIVATE int
count_bits_32 (uint32 val)
{
  int retval;
  int bit;

  retval = 0;
  while ((bit = __builtin_ffs (val)))
    {
      ++retval;
      val ^= (1 << (bit-1));
    }
  return retval;
}

PRIVATE int
count_bits_64 (uint64 val)
{
  int retval;
  retval = count_bits_32 (val) + count_bits_32 (val >> 32);
  return retval;
}

enum { HASH_SIZE = 504149 };

PRIVATE uint32
hash_func (const uint64 hands[9])
{
  uint32 retval;
  int i;
  
  retval = 0;
  for (i = 0; i < 9; ++i)
    {
      retval = ((retval >> 29) | (retval << 3)) ^ (hands[i] >> 32);
      retval = ((retval >> 29) | (retval << 3)) ^ (hands[i] >> 0);
    }
  return retval % HASH_SIZE;
}

typedef struct hash_entry_str
{
  struct hash_entry_str *next;
  uint64 hands[9];
  uint32 high, low;
}
hash_entry_t;

PRIVATE hash_entry_t *hash_table[HASH_SIZE];

PRIVATE boolean_t
hands_equal (const uint64 hands1[9], const uint64 hands2[9])
{
  boolean_t retval;

  retval = memcmp (hands1, hands2, sizeof (uint64) * 9) == 0;
  return retval;
}

PRIVATE hash_entry_t **
hash_pp (const uint64 hands[9])
{
  hash_entry_t **retval;

  retval = &hash_table[hash_func (hands)];
  while (*retval && !hands_equal ((*retval)->hands, hands))
    retval = &(*retval)->next;
  return retval;
}

PRIVATE float
hash_ratio (const uint64 hands[9])
{
  float retval;
  hash_entry_t **pp;

  pp = hash_pp (hands);
  retval = *pp ? (float) (*pp)->high / (*pp)->low : 0;
  return retval;
}

PRIVATE void
hash_insert (const uint64 hands[9], uint32 high, uint32 low)
{
  hash_entry_t **pp;
  hash_entry_t *entryp;

  pp = hash_pp (hands);
  entryp = malloc (sizeof *entryp);
  entryp->next = *pp;
  memcpy (entryp->hands, hands, sizeof entryp->hands);
  entryp->high = high;
  entryp->low = low;
  *pp = entryp;
  fwrite (hands, sizeof hands[0] * 9, 1, stdout);
  fwrite (&high, sizeof high,  1, stdout);
  fwrite (&low,  sizeof low,   1, stdout);
  fflush (stdout);
}

/* globals -- ick */
PRIVATE int lcd;
PRIVATE int reward_table[10]; /* NOTE: 1-based array, not 0-based */

PRIVATE void
score_hands (uint32 score[9], uint64 hands[9],
	     uint64 *dead_cardsp, uint64 pegged_cards, uint8 n_cards)
{
  uint32 val, high_val;
  int to_reward[9], *rewardp;
  rank_set_t card_diffs[8 * 4], *card_diffp;
  int reward;
  int i;
  uint64 card1, card2, card3, card4, card5;
  uint64 n1, n2, n3, n4, n5;
  uint64 dead_cards;

  memset (score, 0, sizeof score[0] * 9);
  dead_cards = *dead_cardsp;
  card_diffp = card_diffs;
  for (i = 0; i < 8; ++i)
    {
      rank_set_t h0, h1, c0, c1, d0, d1, s0, s1;

      extr_suits (hands[i]  , &h0, &d0, &c0, &s0);
      extr_suits (hands[i+1], &h1, &d1, &c1, &s1);
      *card_diffp++ = h0 ^ h1;
      *card_diffp++ = d0 ^ d1;
      *card_diffp++ = c0 ^ c1;
      *card_diffp++ = s0 ^ s1;
      dead_cards |= hands[i];
    }
  dead_cards |= hands[i];

     n1 =    n2 =    n3 =    n4 =    n5 = 0;
  card1 = card2 = card3 = card4 = card5 = 0;

  switch (n_cards)
    {
    default:
      fprintf(stderr, "Number of cards to iterate must be "
	      "between 0 and 5 inclusive\n");
      exit(1);
    case 5:
      /* will set up normally below */
      break;
    case 4:
      card2 = (uint64) 1 << 51;
      n1    = hands[0] | pegged_cards;
      break;
    case 3:
      card3 = (uint64) 1 << 51;
      n2    = hands[0] | pegged_cards;
      break;
    case 2:
      card4 = (uint64) 1 << 51;
      n3    = hands[0] | pegged_cards;
      break;
    case 1:
      card5 = (uint64) 1 << 51;
      n4    = hands[0] | pegged_cards;
      break;
    case 0:
      n5    = hands[0] | pegged_cards;
      break;
    }
  switch (n_cards)
    {
    case 5:
      for (card1 = (uint64) 1 << 51; card1 ; card1 >>= 1)
	{
	  if (card1 & dead_cards)
/*-->*/	    continue;
	  n1 = hands[0] | card1;
	  for (card2 = card1 >> 1; card2 ; card2 >>= 1)
	    {
    case 4:
	      if (card2 & dead_cards)
/*-->*/		continue;
	      n2 = n1 | card2;
	      for (card3 = card2 >> 1; card3 ; card3 >>= 1)
		{
    case 3:
		  if (card3 & dead_cards)
/*-->*/		    continue;
		  n3 = n2 | card3;
		  for (card4 = card3 >> 1; card4 ; card4 >>= 1)
		    {
    case 2:
		      if (card4 & dead_cards)
/*-->*/			continue;
		      n4 = n3 | card4;
		      for (card5 = card4 >> 1; card5 ; card5 >>= 1)
			{
			  rank_set_t hearts, clubs, diamonds, spades;

    case 1:
			  if (card5 & dead_cards)
/*-->*/			    continue;
			  n5 = n4 | card5;
    case 0:
                          extr_suits (n5, &hearts, &diamonds, &clubs, &spades);
			  high_val = 0;
			  rewardp = to_reward;
			  card_diffp = card_diffs;
			  i = 0;
			  {
loop:
			    val = eval_exactly_7_cards(hearts, diamonds,
						       clubs,  spades);
			    if (val >= high_val)
			      {
				if (val > high_val)
				  {
				    high_val = val;
				    rewardp = to_reward;
				  }
				*rewardp++ = i;
			      }
			    if (++i <= 8)
			      {
				hearts   ^= *card_diffp++;
				diamonds ^= *card_diffp++;
				clubs    ^= *card_diffp++;
				spades   ^= *card_diffp++;
				goto loop;
			      }
			  }
			  reward = reward_table[rewardp - to_reward];
			  do
			    score[*--rewardp] += reward;
			  while (rewardp > to_reward);
			}
		    }
		}
	    }
	}
    }
  *dead_cardsp = dead_cards;
}

enum { TRY_MAX = 500 };

#if defined __MINGW32__
#define random rand
#endif

PRIVATE inline uint64
random_hole_cards (void)
{
  uint64 retval;
  int i, j;

  i = random() % 52;
  do
    j = random () % 52;
  while (i == j);
  retval = ((uint64) 1 << i) | ((uint64) 1 << j);
  return retval;
}


PRIVATE void
dump_card (int i)
{
  int suit, card;
  static char *cards = "23456789tjqka";
  static char *suits = "hdcs";

  suit = i / 13;
  card = i % 13;
  putchar (cards[card]);
  putchar (suits[suit]);
}

PRIVATE int
ffs_uint64 (uint64 val)
{
  int retval;

  retval = __builtin_ffs (val);
  if (!retval)
    retval = __builtin_ffs (val >> 32) + 32;
  return retval;
}

PUBLIC int
main (int argc, char *argv[])
{
  uint64 hands[9];
  uint32 score[9];
  int i;
  uint8 n_cards;
  uint64 dead_cards, pegged_common;
  int card0, card1, card2, card3, card4, card5;
  float low_ratio;

  lcd = poker_lcd (9);

  for (i = 1; i <= 9; ++i)
    reward_table[i] = lcd / i;

  pegged_common = 0;
  n_cards = 5;
  low_ratio = 99;
  for (card0 = (1 << 12); card0 >= (1 << 5); card0 >>= 1)
    {
      for (card1 = card0 >> 1; card1 > 0; card1 >>= 1)
	{
	  for (card2 = card0 >> 1; card2 >= (1 << 4); card2 >>= 1)
	    {
	      if (card2 != card1)
		{
		  for (card3 = card2 >> 1; card3 > 0; card3 >>= 1)
		    {
		      for (card4 = card2 >> 1; card4 >= (1 << 3); card4 >>= 1)
			{
			  if (card4 != card2 && card4 != card1)
			    {
			      for (card5 = card4 >> 1; card5 > 0; card5 >>= 1)
				{
				  hands[0] = (((uint64) card0 << HEART_TO_SPADE) |
					      ((uint64) card1 << HEART_TO_HEART));
				  hands[1] = (((uint64) card0 << HEART_TO_HEART) |
					      ((uint64) card1 << HEART_TO_DIAMOND));
				  hands[2] = (((uint64) card0 << HEART_TO_DIAMOND) |
					      ((uint64) card1 << HEART_TO_SPADE));

				  hands[3] = (((uint64) card2 << HEART_TO_SPADE)|
					      ((uint64) card3 << HEART_TO_HEART));
				  hands[4] = (((uint64) card2 << HEART_TO_HEART)|
					      ((uint64) card3 << HEART_TO_DIAMOND));
				  hands[5] = (((uint64) card2 << HEART_TO_DIAMOND)|
					      ((uint64) card3 << HEART_TO_SPADE));

				  hands[6] = (((uint64) card4 << HEART_TO_SPADE)|
					      ((uint64) card5 << HEART_TO_SPADE));
				  hands[7] = (((uint64) card4 << HEART_TO_HEART)|
					      ((uint64) card5 << HEART_TO_HEART));
				  hands[8] = (((uint64) card4 << HEART_TO_DIAMOND)|
					      ((uint64) card5 << HEART_TO_DIAMOND));
				  dead_cards = 0;
				  score_hands (score, hands, &dead_cards,
					       pegged_common, n_cards);
				  {
				    uint32 low, high;
				    int i;
				    float ratio;
      
				    low = 0x7fffffff;
				    high = 0;
	    
				    for (i = 0; i < 9; ++i)
				      {
					if (score[i] < low)
					  low = score[i];

					if (score[i] > high)
					  high = score[i];
				      }
				    ratio = (float) high / low;
				    if (ratio < low_ratio)
				      {
					low_ratio = ratio;
					hash_insert (hands, high, low);
					fprintf (stderr, "%f\n", low_ratio);
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
  return 0;
}
