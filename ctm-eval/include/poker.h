#if	!defined(__POKER__)
#define	__POKER__

/*
 * NOTE:  This code was written to be useful for single deck poker games
 *	  that don't involve wild cards and that never have eight cards or
 *	  more in a given hand to evaluate.
 *
 *	  Specifically I wrote this code for Hold 'em simulators running
 *	  on our Alpha.  The data structures used are designed to permit
 *	  quick hand evaluation.
 */

#define	PRIVATE	static
#define PUBLIC

typedef enum {
    high_hand = 1,
    pair,
    two_pair,
    three_of_a_kind,
    straight,
    flush,
    full_house,
    four_of_a_kind,
    straight_flush,
} hand_t;

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

#define	N_RANK	(ace - deuce + 1)

#if	!defined(__alpha)
typedef	long long uint64;
#else
typedef	unsigned long uint64;
#endif

typedef union {
    uint64 cards_n;
    struct {
	uint64         :3;
	uint64 spades  :N_RANK;
	uint64         :3;
	uint64 clubs   :N_RANK;
	uint64         :3;
	uint64 diamonds:N_RANK;
	uint64         :3;
	uint64 hearts  :N_RANK;
    }
    cards_t;
} cards_u;

#define	FIVE_STRAIGHT_MASK ((1 << ace  ) | \
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
 *	 things are laid out.
 */

#if	defined(__GNUC__) || defined(__alpha)

typedef	unsigned int uint32;

#if	defined(__alpha) || defined(i386)
#define	SMALL_TO_BIG_BIT_FIELDS
#elif	defined(mc68000)
#define	BIG_TO_SMALL_BIT_FIELDS
#endif

#endif	/* defined(__GNUC__) */

#if	!defined(__GNUC__)
#define	inline
#endif

/*
 * The point of the eval_u union is to be able to fill in enough information
 * about a hand to generate a number that is guaranteed to compare
 * appropriately to similar, greater or lesser hands.  NOTE: for many types
 * of hands, it is not necessary to fill all the cards.  For example, a
 * straight would only need the hand and top_card field filled in.
 */

#define	CARD_BIT_WIDTH	4


typedef union {
    uint32 eval_n;
    struct {
#if	defined(SMALL_TO_BIG_BIT_FIELDS)
	rank_t fifth_card :CARD_BIT_WIDTH;
	rank_t fourth_card:CARD_BIT_WIDTH;
	rank_t third_card :CARD_BIT_WIDTH;
	rank_t second_card:CARD_BIT_WIDTH;
	rank_t top_card   :CARD_BIT_WIDTH;
	hand_t hand       :4;
	unsigned int zeros:8;
#elif	defined(BIG_TO_SMALL_BIT_FIELDS)
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

extern int         n_bits_table[1 << N_RANK];
extern int top_five_cards_table[1 << N_RANK];
extern int       top_card_table[1 << N_RANK];

#define	HAND_SIZE	5	/* number of significant cards in showdown */

#endif	/* !defined(__POKER__) */
