char rcsid_caro2[] =
	"$Id$";

/*
 *  caro2.c: a program to explore some of the search space of Mike Caro's
 *	r.g.p. challenge #2.
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

#include "poker.h"
#include "eval7.h"

/* speed doesn't matter for lcd_of_first_n_positive_integers */

PRIVATE int
lcd_of_first_n_positive_integers (int n)
{
  int retval, i;
  boolean_t done;

  retval = n;
  done = false;

  while (!done)
    {
      done = true;
      for (i = 2; done && i <= n; ++i)
	if (retval % i)
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

PRIVATE void
canon (uint64 hands[9], uint64 *deadp, uint64 *peggedp)
{
  int i;
  rank_set_t hearts, diamonds, clubs, spades;
  int hs, ds, cs, ss; /* heart_shift, diamond_shift, club_shift, spade_shift */
  boolean_t doit;

  hearts   = 0;
  clubs    = 0;
  diamonds = 0;
  spades   = 0;
  for (i = 0; i < 9; ++i)
    {
      rank_set_t temp_hearts, temp_diamonds, temp_clubs, temp_spades;

      extr_suits (hands[i], &temp_hearts, &temp_diamonds,
		            &temp_clubs , &temp_spades);
      hearts   |= temp_hearts;
      diamonds |= temp_diamonds;
      clubs    |= temp_clubs;
      spades   |= temp_spades;
    }
  doit = true;
  if (hearts >= diamonds)
    {
      if (clubs >= spades)
	{
	  if (diamonds >= clubs)
	    { /* hearts, diamonds, clubs, spades */
#if !defined (LETGCCWAIL)
	      hs = HEART_TO_HEART;
	      ds = DIAMOND_TO_DIAMOND;
	      cs = CLUB_TO_CLUB;
	      ss = SPADE_TO_SPADE;
#endif
	      doit = false; /* already canonical */
	    }
	  else if (hearts >= clubs)
	    if (diamonds >= spades)
	      { /* hearts, clubs, diamonds, spades */
		hs = HEART_TO_HEART;
		ds = DIAMOND_TO_CLUB;
		cs = CLUB_TO_DIAMOND;
		ss = SPADE_TO_SPADE;
	      }
	    else
	      { /* hearts, clubs, spades, diamonds */
		hs = HEART_TO_HEART;
		ds = DIAMOND_TO_SPADE;
		cs = CLUB_TO_DIAMOND;
		ss = SPADE_TO_CLUB;
	      }
	  else
	    {
	      if (spades >= hearts)
		{ /* clubs, spades, hearts, diamonds */
		  hs = HEART_TO_CLUB;
		  ds = DIAMOND_TO_SPADE;
		  cs = CLUB_TO_HEART;
		  ss = SPADE_TO_DIAMOND;
		}
	      else if (diamonds >= spades)
		{ /* clubs, hearts, diamonds, spades */
		  hs = HEART_TO_DIAMOND;
		  ds = DIAMOND_TO_CLUB;
		  cs = CLUB_TO_HEART;
		  ss = SPADE_TO_SPADE;
		}
	      else
		{ /* clubs, hearts, spades, diamonds */
		  hs = HEART_TO_DIAMOND;
		  ds = DIAMOND_TO_SPADE;
		  cs = CLUB_TO_HEART;
		  ss = SPADE_TO_CLUB;
		}
	    }
	}
      else
	{
	  if (diamonds >= spades)
	    { /* hearts, diamonds, spades, clubs */
	      hs = HEART_TO_HEART;
	      ds = DIAMOND_TO_DIAMOND;
	      cs = CLUB_TO_SPADE;
	      ss = SPADE_TO_CLUB;
	    }
	  else if (hearts >= spades)
	    if (diamonds >= clubs)
	      { /* hearts, spades, diamonds, clubs */
		hs = HEART_TO_HEART;
		ds = DIAMOND_TO_CLUB;
		cs = CLUB_TO_SPADE;
		ss = SPADE_TO_DIAMOND;
	      }
	    else
	      { /* hearts, spades, clubs, diamonds */
		hs = HEART_TO_HEART;
		ds = DIAMOND_TO_SPADE;
		cs = CLUB_TO_CLUB;
		ss = SPADE_TO_DIAMOND;
	      }
	  else
	    {
	      if (clubs >= hearts)
		{ /* spades, clubs, hearts, diamonds */
		  hs = HEART_TO_CLUB;
		  ds = DIAMOND_TO_SPADE;
		  cs = CLUB_TO_DIAMOND;
		  ss = SPADE_TO_HEART;
		}
	      else if (diamonds >= clubs)
		{ /* spades, hearts, diamonds, clubs */
		  hs = HEART_TO_DIAMOND;
		  ds = DIAMOND_TO_CLUB;
		  cs = CLUB_TO_SPADE;
		  ss = SPADE_TO_DIAMOND;
		}
	      else
		{ /* spades, hearts, clubs, diamonds */
		  hs = HEART_TO_DIAMOND;
		  ds = DIAMOND_TO_SPADE;
		  cs = CLUB_TO_CLUB;
		  ss = SPADE_TO_HEART;
		}
	    }
	}
    }
  else
    {
      if (clubs >= spades)
	{
	  if (hearts >= clubs)
	    { /* diamonds, hearts, clubs, spades */
	      hs = HEART_TO_DIAMOND;
	      ds = DIAMOND_TO_HEART;
	      cs = CLUB_TO_CLUB;
	      ss = SPADE_TO_SPADE;
	    }
	  else if (diamonds >= clubs)
	    if (hearts >= spades)
	      { /* diamonds, clubs, hearts, spades */
		hs = HEART_TO_CLUB;
		ds = DIAMOND_TO_HEART;
		cs = CLUB_TO_DIAMOND;
		ss = SPADE_TO_SPADE;
	      }
	    else
	      { /* diamonds, clubs, spades, hearts */
		hs = HEART_TO_SPADE;
		ds = DIAMOND_TO_HEART;
		cs = CLUB_TO_DIAMOND;
		ss = SPADE_TO_CLUB;
	      }
	  else
	    {
	      if (spades >= diamonds)
		{ /* clubs, spades, diamonds, hearts */
		  hs = HEART_TO_SPADE;
		  ds = DIAMOND_TO_CLUB;
		  cs = CLUB_TO_HEART;
		  ss = SPADE_TO_DIAMOND;
		}
	      else if (hearts >= spades)
		{ /* clubs, diamonds, hearts, spades */
		  hs = HEART_TO_CLUB;
		  ds = DIAMOND_TO_DIAMOND;
		  cs = CLUB_TO_HEART;
		  ss = SPADE_TO_SPADE;
		}
	      else
		{ /* clubs, diamonds, spades, hearts */
		  hs = HEART_TO_SPADE;
		  ds = DIAMOND_TO_DIAMOND;
		  cs = CLUB_TO_HEART;
		  ss = SPADE_TO_CLUB;
		}
	    }
	}
      else
	{
	  if (hearts >= spades)
	    { /* diamonds, hearts, spades, clubs */
	      hs = HEART_TO_DIAMOND;
	      ds = DIAMOND_TO_HEART;
	      cs = CLUB_TO_SPADE;
	      ss = SPADE_TO_CLUB;
	    }
	  else if (diamonds >= spades)
	    if (hearts >= clubs)
	      { /* diamonds, spades, hearts, clubs */
		hs = HEART_TO_CLUB;
		ds = DIAMOND_TO_HEART;
		cs = CLUB_TO_SPADE;
		ss = SPADE_TO_DIAMOND;
	      }
	    else
	      { /* diamonds, spades, clubs, hearts */
		hs = HEART_TO_SPADE;
		ds = DIAMOND_TO_HEART;
		cs = CLUB_TO_CLUB;
		ss = SPADE_TO_DIAMOND;
	      }
	  else
	    {
	      if (clubs >= diamonds)
		{ /* spades, clubs, diamonds, hearts */
		  hs = HEART_TO_SPADE;
		  ds = DIAMOND_TO_CLUB;
		  cs = CLUB_TO_DIAMOND;
		  ss = SPADE_TO_HEART;
		}
	      else if (hearts >= clubs)
		{ /* spades, diamonds, hearts, clubs */
		  hs = HEART_TO_CLUB;
		  ds = DIAMOND_TO_DIAMOND;
		  cs = CLUB_TO_SPADE;
		  ss = SPADE_TO_HEART;
		}
	      else
		{ /* spades, diamonds, clubs, hearts */
		  hs = HEART_TO_SPADE;
		  ds = DIAMOND_TO_DIAMOND;
		  cs = CLUB_TO_CLUB;
		  ss = SPADE_TO_HEART;
		}
	    }
	}
    }
  if (doit)
    {
      for (i = 0; i < 9; ++i)
	normalize_suits (&hands[i], hs, ds, cs, ss);
      normalize_suits (deadp, hs, ds, cs, ss);
      normalize_suits (peggedp, hs, ds, cs, ss);
    }
  qsort (hands, 9, sizeof hands[0], card_compare); /* hand-coded insertion
						      sort might be faster */
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

PRIVATE FILE *outfile;

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

/*
 * replace_hand is fairly important, but this first implementation
 * is pretty poor (watching movies with my wife)
 */

enum { TRY_MAX = 500 };

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
replace_hand (uint64 hands[9], uint64 *dead_cardsp, int to_replace)
{
  uint64 dead_cards;
  int try_count;

  try_count = 0;
  dead_cards = *dead_cardsp;
  dead_cards ^= hands[to_replace];
  do
    hands[to_replace] = random_hole_cards();
  while (((hands[to_replace] & dead_cards) || hash_ratio (hands))
	 && ++try_count <= TRY_MAX);
  *dead_cardsp = dead_cards | hands[to_replace];
  if (try_count > TRY_MAX)
    {
      /* What do we do here?  replace all 9 hole cards? */
      int i;

      for (i = 0; i < 9; ++i)
	{
	  if (i != to_replace)
	    dead_cards ^= hands[i];
	}
      for (i = 0; i < 9; ++i)
	{
	  uint64 new_cards;

	  do
	    new_cards = random_hole_cards();
	  while (dead_cards & new_cards);
	  hands[i] = new_cards;
	  dead_cards |= new_cards;
	}
    }
}

PUBLIC int
main (int argc, char *argv[])
{
  uint64 hands[9];
  uint32 score[9];
  int i;
  uint8 n_cards;
  uint64 temp_card, dead_cards, pegged_common;
  boolean_t seen_cards_already;
  int hole_card_no;
  boolean_t auto_flag;
  uint32 high, low;
  int to_replace;

  lcd = lcd_of_first_n_positive_integers (9);

  hole_card_no = 0;
  memset (hands, 0, sizeof hands);
  for (i = 1; i <= 9; ++i)
    reward_table[i] = lcd / i;

  auto_flag = false;
  n_cards = 5;
  dead_cards = 0;
  pegged_common = 0;
  seen_cards_already = false;
  for (i = 1; i < argc; ++i)
    {
      if (argv[i][0] == '-')
	{
	  if (seen_cards_already)
	    {
	      fprintf(stderr, "Cards must come before options\n");
	      exit(1);
	    }
	  if (strcmp (argv[i], "-a") == 0)
	    auto_flag = true;
	  else if (strcmp (argv[i], "-d") == 0 ||
	      strcmp (argv[i], "-c") == 0)
	    {
	      if (i == argc)
		{
		  fprintf(stderr, "Missing card portion of -d\n");
		  exit(1);
		}
	      temp_card = string_to_card(argv[i]);
	      if (!temp_card)
		{
		  fprintf(stderr, "Malformed card \"%s\"\n", argv[i]);
		  exit(1);
		}
	      else
		{
		  if (argv[i][1] == 'c')
		    pegged_common |= temp_card;
		  dead_cards |= temp_card;
		  --n_cards;
		  ++i;
		}
	    }
	  else
	    {
	      fprintf(stderr, "Unknown switch \"%s\"\n", argv[i]);
	      exit(1);
	    }
	}
      else
	{
	  temp_card = string_to_card(argv[i]);
	  if (!temp_card)
	    {
	      fprintf(stderr, "Malformed card \"%s\"\n", argv[i]);
	      exit(1);
	    }
	  else
	    hands[hole_card_no++/2] |= temp_card;
	}
    }

  if (hole_card_no != 18)
    {
      fprintf (stderr, "Wrong number of hole cards\n");
      exit (1);
    }

  do
    {
      canon (hands, &dead_cards, &pegged_common);
      score_hands (score, hands, &dead_cards, pegged_common, n_cards);
      {
	uint32 second_low, second_high;
	int low_i, high_i, second_low_i, second_high_i;
	int low_count, high_count;
      
	low = second_low = 0x7fffffff;
	high = second_high = 0;
	low_count = 0;
	high_count = 0;
  
	for (i = 0; i < 9; ++i)
	  {
	    if (!auto_flag)
	      printf ("        %9d\n", score[i]);
	    if (score[i] < low)
	      {
		second_low = low;
		second_low_i = low_i;
		low = score[i];
		low_i = i;
		low_count = 1;
	      }
	    else if (score[i] == low)
	      ++low_count;
	    else if (score[i] < second_low)
	      {
		second_low = score[i];
		second_low_i = i;
	      }
	    
	    if (score[i] > high)
	      {
		second_high = high;
		second_high_i = high_i;
		high = score[i];
		high_i = i;
		high_count = 1;
	      }
	    else if (score[i] == high)
	      ++high_count;
	    else if (score[i] > second_high)
	      {
		second_high = score[i];
		second_high_i = i;
	      }
	  }
	if (!auto_flag)
	  printf (" high = %9d\n  low = %9d\nratio = %f\n", high, low,
		  (float) high / low);
	else
	  {
#if 0
	    fprintf (stderr, "%f\n", (float) high / low);
#endif
	    hash_insert (hands, high, low);
	    if (low_count < high_count)
	      to_replace = low_i;
	    else if (high_count < low_count)
	      to_replace = high_i;
	    else
	      {
		uint32 high_diff, low_diff;

		high_diff = high - second_high;
		low_diff = second_low - low;

		if (high_diff > low_diff)
		  to_replace = high_i;
		else
		  to_replace = low_i;
	      }
	    replace_hand (hands, &dead_cards, to_replace);
	  }
      }
    } while (auto_flag && high != low);
  
  return 0;
}
