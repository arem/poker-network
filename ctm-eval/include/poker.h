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

typedef union {
    long long cards_n;
    struct {
	unsigned long long spades  :13;
	unsigned long long clubs   :13;
	unsigned long long diamonds:13;
	unsigned long long hearts  :13;
    }
    cards_t;
} cards_u;

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

#if	defined(__alpha) && defined(__GNUC__)

#define	SMALL_TO_BIG_BIT_FIELDS
typedef	unsigned int uint32;

#endif

#if	defined(SMALL_TO_BIG_BIT_FIELDS)

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
	rank_t fifth_card :CARD_BIT_WIDTH;
	rank_t fourth_card:CARD_BIT_WIDTH;
	rank_t third_card :CARD_BIT_WIDTH;
	rank_t second_card:CARD_BIT_WIDTH;
	rank_t top_card   :CARD_BIT_WIDTH;
	hand_t hand       :4;
	unsigned int zeros:8;
    } eval_t;
} eval_u;
#else
#error "Not enough information to known about this machine"
#endif

typedef enum { false, true } boolean_t;

extern int         n_bits_in_13[1 << 13];
extern int top_five_cards_in_13[1 << 13];
extern int       top_card_in_13[1 << 13];

#endif	/* !defined(__POKER__) */
