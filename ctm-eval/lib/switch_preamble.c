#include "poker.h"
#include "tree_inlines.h"

uint32
new_eval (rank_set_t clubs, rank_set_t diamonds, rank_set_t hearts,
      rank_set_t spades)
{
  rank_set_t ranks, flush_suit, three_info, all_pairs;
  uint32 retval;

  ranks = clubs | diamonds | hearts | spades;

  switch (ranks)
    {
