/*
 *  poker.h: a collection of definitions useful for writing poker programs
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

#if     !defined(__POKER__)
#define __POKER__

/*
 * NOTE:  This code was written to be useful for single deck poker games
 *        that don't involve wild cards and that never have eight cards or
 *        more in a given hand to evaluate.
 *
 *        Specifically I wrote this code for Hold 'em simulators running
 *        on our Alpha.  The data structures used are designed to permit
 *        quick hand evaluation.
 */

#define PRIVATE static
#define PUBLIC

typedef enum {
    high_hand,
    pair,
    two_pair,
    three_of_a_kind,
    straight,
    flush,
    full_house,
    four_of_a_kind,
    straight_flush,
} hand_t;

#define N_HAND  (straight_flush - high_hand + 1)

typedef enum {
    deuce = 0,
    trey,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king,
    ace,
} rank_t;

#define N_RANK  (ace - deuce + 1)

/*
 * These two are used by rank_eval which doesn't use the cards_u structure.
 * Be sure you understand the difference.
 */

#define SUIT_MASK       ( (1 << N_RANK) - 1 )   /* Two's complement! */
#define SUIT_SHIFT      ( N_RANK )

typedef unsigned char uint8;
typedef   signed char  int8;
typedef unsigned int uint32;

#if     !defined(__alpha)
typedef long long uint64;
#else
typedef unsigned long uint64;
#endif

typedef uint32 rank_set_t;

typedef union {
    uint64 cards_n;
    struct {
	uint32         :3;
	uint32 spades  :N_RANK;
	uint32         :3;
	uint32 clubs   :N_RANK;
	uint32         :3;
	uint32 diamonds:N_RANK;
	uint32         :3;
	uint32 hearts  :N_RANK;
    }
    cards_t;
} cards_u;

#define FIVE_STRAIGHT_MASK ((1 << ace  ) | \
			    (1 << five )| \
			    (1 << four )| \
			    (1 << trey )| \
			    (1 << deuce))

#define SIX_STRAIGHT_MASK ((1 << six  )| \
			   (1 << five )| \
			   (1 << four )| \
			   (1 << trey )| \
			   (1 << deuce))

#define SEVEN_STRAIGHT_MASK (  SIX_STRAIGHT_MASK << 1)
#define EIGHT_STRAIGHT_MASK (SEVEN_STRAIGHT_MASK << 1)
#define  NINE_STRAIGHT_MASK (EIGHT_STRAIGHT_MASK << 1)
#define   TEN_STRAIGHT_MASK ( NINE_STRAIGHT_MASK << 1)
#define  JACK_STRAIGHT_MASK (  TEN_STRAIGHT_MASK << 1)
#define QUEEN_STRAIGHT_MASK ( JACK_STRAIGHT_MASK << 1)
#define  KING_STRAIGHT_MASK (QUEEN_STRAIGHT_MASK << 1)
#define   ACE_STRAIGHT_MASK ( KING_STRAIGHT_MASK << 1)

/*
 * NOTE: Our use of bitfields below means that we need to know how
 *       things are laid out.
 */

#if     defined(__GNUC__) || defined(__alpha)

#if     defined(__alpha) || defined(i386)
#define SMALL_TO_BIG_BIT_FIELDS
#elif   defined(mc68000) || defined(sparc) || defined(hppa)
#define BIG_TO_SMALL_BIT_FIELDS
#endif

#endif  /* defined(__GNUC__) */

#if     !defined(__GNUC__)
#define inline
#endif

/*
 * The point of the eval_u union is to be able to fill in enough information
 * about a hand to generate a number that is guaranteed to compare
 * appropriately to similar, greater or lesser hands.  NOTE: for many types
 * of hands, it is not necessary to fill all the cards.  For example, a
 * straight would only need the hand and top_card field filled in.
 */

#define CARD_BIT_WIDTH  4


typedef union {
    uint32 eval_n;
    struct {
#if     defined(SMALL_TO_BIG_BIT_FIELDS)
	rank_t fifth_card :CARD_BIT_WIDTH;
	rank_t fourth_card:CARD_BIT_WIDTH;
	rank_t third_card :CARD_BIT_WIDTH;
	rank_t second_card:CARD_BIT_WIDTH;
	rank_t top_card   :CARD_BIT_WIDTH;
	hand_t hand       :4;
	unsigned int zeros:8;
#elif   defined(BIG_TO_SMALL_BIT_FIELDS)
	unsigned int zeros:8;
	hand_t hand       :4;
	rank_t top_card   :CARD_BIT_WIDTH;
	rank_t second_card:CARD_BIT_WIDTH;
	rank_t third_card :CARD_BIT_WIDTH;
	rank_t fourth_card:CARD_BIT_WIDTH;
	rank_t fifth_card :CARD_BIT_WIDTH;
#else
#error "Not enough information to known about this machine"
#endif
    } eval_t;
} eval_u;

typedef enum { false, true } boolean_t;

extern uint8          n_bits_table[1 << N_RANK];
extern uint32 top_five_cards_table[1 << N_RANK];
extern uint32  top_five_bits_table[1 << N_RANK];
extern uint32       top_card_table[1 << N_RANK];
extern uint32        top_bit_table[1 << N_RANK];

extern uint64   mask_rank_table[N_RANK];

/*
 * NOTE: mask_rank is provided here so that mktable can
 *       use it.  Duplicated code is evil...
 */

static inline uint64 mask_rank( uint32 rank )
{
    cards_u temp;
    uint32 bit;

    bit = (1 << rank);
    temp.cards_t.spades   = bit;
    temp.cards_t.clubs    = bit;
    temp.cards_t.diamonds = bit;
    temp.cards_t.hearts   = bit;
    return ~temp.cards_n;
}

typedef enum { hearts, diamonds, clubs, spades } suit_t;

#define N_SUIT  (spades + 1)

#define VALUE_SHIFT     (2 * N_RANK)
#define VALUE(hand)     ((hand) << VALUE_SHIFT)

typedef enum {
    HIGH_HAND_VALUE =           VALUE(high_hand),
    PAIR_VALUE =                VALUE(pair),
    TWO_PAIR_VALUE =            VALUE(two_pair),
    THREE_OF_A_KIND_VALUE =     VALUE(three_of_a_kind),
    STRAIGHT_VALUE =            VALUE(straight),
    FLUSH_VALUE =               VALUE(flush),
    FULL_HOUSE_VALUE =          VALUE(full_house),
    FOUR_OF_A_KIND_VALUE =      VALUE(four_of_a_kind),
    STRAIGHT_FLUSH_VALUE =      VALUE(straight_flush),
} value_t;

#define HAND_SIZE       5       /* number of significant cards in showdown */

extern const char *hand_names[];
extern const char *rank_names[];
extern void dump_rank( uint32 ranks, char suitchar );
extern void dump_cards( cards_u cards );
extern void dump_eval( eval_u eval );
extern uint32 new_eval_to_old_eval( uint32 new_eval );

#define FSM_SHIFT       12
#define FSM_MASK        ((1 << FSM_SHIFT) - 1)  /* two's complement! */

extern uint8 str_and_flu_table[N_HAND][1 << N_RANK];
extern uint8 straight_table[1 << N_RANK];
extern uint8 rank_fsm_table[N_HAND][1 << FSM_SHIFT];
extern uint64 cards_to_counts_table[1 << N_RANK];
extern uint64 string_to_card(const char *str);

extern uint32 top_five_cards[1 << N_RANK];
extern uint32 straight_value_table[1 << N_RANK];
extern uint32 three_of_a_kind_value_table[1 << N_RANK];
extern uint32 top_pair_table[1 << N_RANK];
extern uint32 pair_value_table[1 << N_RANK];
extern uint32 flush_cards_table[1 << N_RANK];

#endif  /* !defined(__POKER__) */
