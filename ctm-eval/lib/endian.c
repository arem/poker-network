#include <stdio.h>

typedef struct
{
  union
    {
      struct
	{
	unsigned int first: 1;
	unsigned int second: 2;
	}
      bit_field;
      unsigned int ui;
    }
  u;
}
test_t;

int
main (void)
{
  test_t test;

  test.u.ui = 0;
  test.u.bit_field.first = 1;
  if (test.u.ui == 1)
    printf ("#define SMALL_TO_BIG_BIT_FIELDS\n");
  else
    printf ("#define BIG_TO_SMALL_BIT_FIELDS\n");
  return 0;
}
