#ifndef _MAKESWITCH_H_
#define _MAKESWITCH_H_

typedef enum
{
  CARDS_DEALT = 7
} cards_dealt_t;

typedef enum
{
  MAY_HAVE_FOUR_OF_A_KIND  = (1 << 0),
  MAY_HAVE_FLUSH           = (1 << 1),
  MUST_HAVE_STRAIGHT       = (1 << 2),
  MAY_HAVE_FULL_HOUSE      = (1 << 3),
  MAY_HAVE_THREE_OF_A_KIND = (1 << 4),
  MUST_HAVE_AT_LEAST_PAIR  = (1 << 5),
  MUST_HAVE_PAIR           = (1 << 6),
} may_have_t;

#define MAY_HAVE_COMBINATIONS 0x80

#endif  /* !_MAKESWITCH_H_ */
