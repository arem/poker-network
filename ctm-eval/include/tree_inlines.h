static inline uint32 flush_value(rank_set_t ranks)
{
    return FLUSH_VALUE | top_five_card_table[ranks];
}

/*
 * straight_value_table has STRAIGHT_VALUE or'd in
 */

static inline uint32 straight_value(rank_set_t ranks)
{
    return straight_value_table[ranks];
}

static inline uint32 high_hand_value(rank_set_t ranks)
{
    return HIGH_HAND_VALUE | top_five_card_table[ranks];
}

/*
 * three_of_a_kind_value_table is THREE_OF_A_KIND_VALUE ored with the top
 * 2 cards in the subscript.
 */

static inline uint32 three_of_a_kind_value(rank_set_t ranks, rank_set_t trips)
{
    return (trips << N_RANK) | three_of_a_kind_value_table[trips ^ ranks];
}

static inline uint32 two_pair_value(rank_set_t ranks, rank_set_t all_pairs)
{
    uint32 retval;

    retval = top_pair_table[all_pairs];
    return TWO_PAIR_VALUE | (retval << N_RANK) | top_card_table[ranks^retval];
}

/*
 * pair_value_table has PAIR_VALUE or'd in.
 */

static inline uint32 pair_value(rank_set_t ranks, rank_set_t all_pairs)
{
    uint32 retval;

    retval = top_card_table[all_pairs];
    return (retval << N_RANK) | pair_value_table[ranks^retval];
}
