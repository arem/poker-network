#include <stdio.h>
#include "poker.h"

static int n_bits_in_13_func( int n )
{
    int retval;

    retval = 0;
    while (n) {
	if (n & 1)
	    ++retval;
	n >>= 1;
    }
    return retval;
}

static int top_card_in_13_func( int n )
{
    int retval, bit;

    if (n)
	for ( bit = (1 << ace), retval = ace; !(n & bit); bit >>= 1, --retval)
	    ;
    else
	return 0;
    return retval;
}

static int top_five_cards_in_13_func( int n )
{
    int temp, retval, shiftval, i;

    retval = 0;
    shiftval = (5 - 1) * CARD_BIT_WIDTH;
    for (i = 5; --i >= 0;) {
	temp = top_card_in_13_func(n);
	n ^= (1 << temp);
	retval |= (temp << shiftval);
	shiftval -= CARD_BIT_WIDTH;
    }
    return retval;
}

static void output_table(const char *tabname, int (*fp)(int))
{
    int i;
    printf("int %s[1 << 13] = {\n", tabname);
    for (i = 0; i < (1 << 13); ++i)
	printf("    %d,\n", fp(i));
    printf("};\n");
}

int main( void )
{
    output_table("n_bits_in_13",         n_bits_in_13_func);
    output_table("top_five_cards_in_13", top_five_cards_in_13_func);
    output_table("top_card_in_13",       top_card_in_13_func);
    return 0;
}
