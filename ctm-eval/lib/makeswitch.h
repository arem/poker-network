#ifndef _MAKESWITCH_H_
#define _MAKESWITCH_H_

typedef enum
{
  MAY_HAVE_FOUR_OF_A_KIND  = 0x1,
  MAY_HAVE_FLUSH           = 0x2,
  MAY_HAVE_STRAIGHT        = 0x4,
  MAY_HAVE_FULL_HOUSE      = 0x8,
  MAY_HAVE_THREE_OF_A_KIND = 0x10,
  MAY_HAVE_TWO_PAIR        = 0x20,
  MAY_HAVE_PAIR            = 0x40
} may_have_t;

#define MAY_HAVE_COMBINATIONS 0x80

#endif  /* !_MAKESWITCH_H_ */
