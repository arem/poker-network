char rcsid_new_dump_eval[] =
	"$Id$";

/*
 *  new_dump_eval.c: a poker helper routine
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

#include "poker.h"
#include <stdio.h>
#include <assert.h>

PUBLIC void dump_sig( uint32 cards )
{
    char *ranksp;    
    boolean_t need_space;
    uint32 mask;

    need_space = false;
    for (ranksp = "AKQJT98765432", mask = 1 << ace; 
		mask; mask >>= 1, ++ranksp)
	if (cards & mask) {
	    if (need_space)
		putchar(' ');
	    putchar(*ranksp);
	}
}

#define RANK_MASK       ((1 << N_RANK) - 1)

PUBLIC void dump_significant_cards( uint32 eval, uint8 sig[2] )
{
    uint32 ls_cards, ms_cards;

    ms_cards = (eval >> N_RANK) & RANK_MASK;
    ls_cards =  eval            & RANK_MASK;
    assert(n_bits_table[ms_cards] == sig[0]);
    assert(n_bits_table[ls_cards] == sig[1]);
    if (sig[0]) {
	dump_sig(ms_cards);
	printf(" / ");
    }
    dump_sig(ls_cards);
}

static uint8 significant_cards[][2] = {
    {0, 5},     /* high hand */
    {1, 3},     /* pair */
    {2, 1},     /* two pair */
    {1, 2},     /* three of a kind */
    {0, 1},     /* straight */
    {0, 5},     /* flush */
    {1, 1},     /* full house */
    {1, 1},     /* four of a kind */
    {0, 5},     /* straight flush */
};

PUBLIC void new_dump_eval( uint32 eval )
{
    uint8 hand_value;

    hand_value =  eval >> VALUE_SHIFT;
    assert(/* (hand_value >= high_hand) && */ (hand_value <= straight_flush));
    dump_significant_cards(eval, significant_cards[hand_value]);
}

PUBLIC int8 find_and_clear(uint32 *cardsp)
{
    int8 retval;
    uint32 mask;
    
    for (mask = 1 << ace, retval = ace; mask && !(*cardsp & mask);
		--retval, mask >>= 1)
	;
    if (mask)
	*cardsp &= ~mask;
    return retval;
}

PUBLIC uint8 next_card(uint32 *ms_cardsp, uint32 *ls_cardsp)
{
    int8 retval;

    retval = find_and_clear(ms_cardsp);
    if (retval == -1)
	retval = find_and_clear(ls_cardsp);
    return retval == -1 ? 0 : retval;
}

PUBLIC uint32 new_eval_to_old_eval( uint32 new_eval )
{
    eval_u retval;
    uint32 ls_cards, ms_cards;

    retval.eval_n = 0;
    retval.eval_t.hand        = new_eval >> VALUE_SHIFT;
    
#define sig     significant_cards

    ms_cards = (new_eval >> N_RANK) & RANK_MASK;
    ls_cards =  new_eval            & RANK_MASK;
#if     1    
    assert(n_bits_table[ms_cards] == sig[retval.eval_t.hand][0]);
    assert(n_bits_table[ls_cards] == sig[retval.eval_t.hand][1]);
#endif

    retval.eval_t.top_card    = next_card(&ms_cards, &ls_cards);
    retval.eval_t.second_card = next_card(&ms_cards, &ls_cards);
    retval.eval_t.third_card  = next_card(&ms_cards, &ls_cards);
    retval.eval_t.fourth_card = next_card(&ms_cards, &ls_cards);
    retval.eval_t.fifth_card  = next_card(&ms_cards, &ls_cards);
    return retval.eval_n;
}
