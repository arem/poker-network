#include "poker.h"


unsigned long
eval (rank_set_t clubs, rank_set_t diamons, rank_set_t hearts,
      rank_set_t spades)
{
  rank_set_t ranks;

  ranks = clubs | diamonds | hearts | spades;

  switch (ranks)
    {
