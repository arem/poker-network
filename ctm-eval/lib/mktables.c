#include <stdio.h>
#include "poker.h"

static int n_bits_table_func( int n )
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

static int top_card_table_func( int n )
{
    int retval, bit;

    if (n)
	for ( bit = (1 << ace), retval = ace; !(n & bit); bit >>= 1, --retval)
	    ;
    else
	return 0;
    return retval;
}

static int top_five_cards_table_func( int n )
{
    eval_u eval;

    eval.eval_n = 0;
    eval.eval_t.top_card    = top_card_table_func(n);
    n &= ~(1 << eval.eval_t.top_card);
    eval.eval_t.second_card = top_card_table_func(n);
    n &= ~(1 << eval.eval_t.second_card);
    eval.eval_t.third_card  = top_card_table_func(n);
    n &= ~(1 << eval.eval_t.third_card);
    eval.eval_t.fourth_card = top_card_table_func(n);
    n &= ~(1 << eval.eval_t.fourth_card);
    eval.eval_t.fifth_card  = top_card_table_func(n);
    return eval.eval_n;
}

static void output_table(const char *tabname, int (*fp)(int))
{
    int i;
    printf("int %s[1 << N_RANK] = {\n", tabname);
    for (i = 0; i < (1 << N_RANK); ++i)
	printf("    %d,\n", fp(i));
    printf("};\n");
}

int main( void )
{
    printf("#include \"poker.h\"\n");
    output_table("n_bits_table",         n_bits_table_func);
    output_table("top_five_cards_table", top_five_cards_table_func);
    output_table("top_card_table",       top_card_table_func);
    return 0;
}
