#include <stdio.h>

#include "poker.h"

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

PRIVATE void
dump_hand (uint64 hand)
{
  int i, j;

  i = ffs_uint64 (hand) - 1;
  hand ^= (uint64) 1 << i;
  j = ffs_uint64 (hand) - 1;
  dump_card (j);
  printf (" ");
  dump_card (i);
}

PRIVATE void
dump_hands (uint64 hands[9])
{
  int i;

  for (i = 0; i < 9; ++i)
    {
      dump_hand (hands[i]);
      if (i < 8)
	printf ("  ");
    }
}

PUBLIC int 
main (int argc, char *argv[])
{
  FILE *fp;
  uint64 hands[9];
  uint32 high, low;
  int retval;

  fp = fopen ("/tmp/caro2.data", "r");
  while (!feof (fp))
    {
      fread (hands, sizeof hands, 1, fp);
      dump_hands (hands);
      fread (&high, sizeof high,  1, fp);
      fread (&low,  sizeof low,   1, fp);
      printf (" %f\n", (float) high / low);
    }
  retval = 0;
  return retval;
}
