/*
 *  eval.h: a fast poker hand evaluator
 *
 *  Copyright (C) 1993-99 Clifford T. Matthews, Keith Miyake, Brian Goetz
 *
 *  Cliff wrote the original code.
 *  Keith significantly sped up is_straight, is_three_of_a_kind and is_pair,
 *	made mask_rank_table a real table and added returns to eval.
 *  Brian sped up the whole thing a lot, and made it suitable for use on 
 *      machines without 64-bit support.  
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

#if        !defined(__EVAL__)
#define        __EVAL__

#include "poker.h"

/*
 * When run over seven cards, here are the distribution of hands:
 *        high hand: 23294460
 *             pair: 58627800
 *         two pair: 31433400
 *  three of a kind: 6461620
 *         straight: 6180020
 *            flush: 4047644
 *       full house: 3473184
 *   four of a kind: 224848
 *   straight flush: 41584
 *
 */

static inline uint32 is_flush(CardMask cards, uint32 *flushranks )
{
    eval_u retval;

    retval.eval_n = 0;

    if (n_bits_table[cards.cards_t.spades] >= HAND_SIZE) {
        *flushranks              = cards.cards_t.spades;
        retval.eval_t.hand       = flush;
        retval.eval_n           += top_five_cards_table[cards.cards_t.spades];
    } else if (n_bits_table[cards.cards_t.clubs   ] >= HAND_SIZE) {
        *flushranks              = cards.cards_t.clubs;
        retval.eval_t.hand       = flush;
        retval.eval_n           += top_five_cards_table[cards.cards_t.clubs];
    } else if (n_bits_table[cards.cards_t.diamonds] >= HAND_SIZE) {
        *flushranks              = cards.cards_t.diamonds;
        retval.eval_t.hand       = flush;
        retval.eval_n           += top_five_cards_table[cards.cards_t.diamonds];
    } else if (n_bits_table[cards.cards_t.hearts  ] >= HAND_SIZE) {
        *flushranks              = cards.cards_t.hearts;
        retval.eval_t.hand       = flush;
        retval.eval_n           += top_five_cards_table[cards.cards_t.hearts];
    }

    return retval.eval_n;
}

/*
 * is_straight checks for a straight by masking the ranks with four
 * copies of itself, each shifted one bit with respect to the
 * previous one.  So any sequence of five adjacent bits will still
 * be non-zero, but any gap will result in a zero value.  There's
 * a nice side-effect of leaving the top most bit set so we can use
 * it to set top_card.
 *
 */

extern uint8 is_straight_table[];

static inline uint32 is_straight( uint32 ranks )	/* Keith's */
{
    unsigned int ranks2;
    eval_u retval;

    retval.eval_n = 0;

    if ( (ranks2  = ranks & (ranks << 1)) &&
	 (ranks2 &=         (ranks << 2)) &&
	 (ranks2 &=         (ranks << 3)) &&
	 (ranks2 &=         (ranks << 4)) ) {
        retval.eval_t.hand     = straight;
        retval.eval_t.top_card = top_card_table[ranks2];
    } else if ((ranks & FIVE_STRAIGHT_MASK) ==  FIVE_STRAIGHT_MASK) {
        retval.eval_t.hand     = straight;
        retval.eval_t.top_card = five;
    }

    return retval.eval_n;
}


static inline uint32 eval( CardMask cards, int n_cards )
{
    eval_u retval, tempeval, tempeval2;
    uint32 ranks, four_mask=0, three_mask=0, two_mask=0, 
      n_dups, n_ranks, flushranks;
    
#define SC cards.cards_t.clubs
#define SD cards.cards_t.diamonds
#define SH cards.cards_t.hearts
#define SS cards.cards_t.spades

    retval.eval_n = 0;
    ranks = SC | SD | SH | SS;
    n_ranks = n_bits_table[ranks];
    n_dups = n_cards - n_ranks;
    switch (n_dups)
      {
      case 0:
        break;

      case 1:
        two_mask   = ~(SC ^ SD ^ SH ^ SS) & ranks;
        break;

      case 2:
        two_mask   = ~(SC ^ SD ^ SH ^ SS) & ranks;
        three_mask = (( SC&SD )|( SH&SS )) & (( SC&SH )|( SD&SS ));
        break;

      default:
        four_mask  = SH & SD & SC & SS;
        two_mask   = (~(SC ^ SD ^ SH ^ SS) & ranks) ^ four_mask;
        three_mask = (( SC&SD )|( SH&SS )) & (( SC&SH )|( SD&SS ));
        break;
      };

    tempeval.eval_n = 0;
    tempeval2.eval_n = 0;

    if (n_ranks >= HAND_SIZE)
      {
        retval.eval_n = is_flush(cards, &flushranks);
        if (retval.eval_n) {
          tempeval.eval_n = is_straight(flushranks);
          if (tempeval.eval_n) {
            retval.eval_n = 0;
            retval.eval_t.hand = straight_flush;
            retval.eval_t.top_card = tempeval.eval_t.top_card;
/*-->*/     return retval.eval_n;
          }
        } else
          retval.eval_n = is_straight(ranks);
      };

    if (four_mask) 
      {
        retval.eval_n = 0;
        retval.eval_t.hand = four_of_a_kind;
        retval.eval_t.top_card = top_card_table[four_mask];
        retval.eval_t.second_card = 
          top_card_table[ranks ^ (1 << retval.eval_t.top_card)];
        return retval.eval_n;
      };

    if (three_mask && (n_dups >= 3) 
                   && n_bits_table[two_mask | three_mask] >= 2)
      {
        int t;

        retval.eval_n = 0;
        retval.eval_t.hand = full_house;
        retval.eval_t.top_card = top_card_table[three_mask];
        t = (two_mask | three_mask) ^ (1 << retval.eval_t.top_card);
        retval.eval_t.second_card = top_card_table[t];
        return retval.eval_n;
      };

    if (retval.eval_n) /* flush and straight */
      return retval.eval_n;

    if (three_mask) 
      {
        int t;
        
        retval.eval_t.hand = three_of_a_kind;
        retval.eval_t.top_card = top_card_table[three_mask];

        t = ranks ^ three_mask; /* Only one bit set in three_mask */
        retval.eval_t.second_card = top_card_table[t];
        t ^= (1 << retval.eval_t.second_card);
        retval.eval_t.third_card = top_card_table[t];
        return retval.eval_n;
      };

    /* Now, all that's left is pairs, if even that.  */
    switch (n_dups)
      {
      case 0:
        retval.eval_n = top_five_cards_table[ranks];
        retval.eval_t.hand = high_hand;
        break;
        
      case 1:
        {
          int t;

          retval.eval_t.hand     = pair;
          retval.eval_t.top_card = top_card_table[two_mask];
          t = ranks ^ two_mask;      /* Only one bit set in two_mask */
          tempeval.eval_n = top_five_cards_table[t];
          retval.eval_t.second_card = tempeval.eval_t.top_card;
          retval.eval_t.third_card  = tempeval.eval_t.second_card;
          retval.eval_t.fourth_card = tempeval.eval_t.third_card;
        }
        break;

      case 2:
        {
          int t;
          
          retval.eval_t.hand = two_pair;
          tempeval.eval_n = top_five_cards_table[two_mask];
          retval.eval_t.top_card    = tempeval.eval_t.top_card;
          retval.eval_t.second_card = tempeval.eval_t.second_card;
          t = ranks ^ two_mask; /* Exactly two bits set in two_mask */
          retval.eval_t.third_card = top_card_table[t];
        }
        break;

      default:
        {
          int t;
          
          retval.eval_t.hand = two_pair;
          retval.eval_t.top_card = top_card_table[two_mask];
          retval.eval_t.second_card = 
            top_card_table[two_mask ^ (1 << retval.eval_t.top_card)];
          t = ranks ^ (1 << retval.eval_t.top_card)
                    ^ (1 << retval.eval_t.second_card);
          retval.eval_t.third_card = top_card_table[t];
        }
        break;
      };

    return retval.eval_n;
}

#endif        /* !defined(__EVAL__) */
