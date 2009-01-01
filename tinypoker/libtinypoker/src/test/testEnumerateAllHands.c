/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <linuxgeek@gmail.com>
 * 
 * This file is part of libtinypoker.
 * 
 * libtinypoker is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * libtinypoker is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * libtinypoker.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../main/tinypoker.h"
#include "test.h"

#include <gsl/gsl_combination.h>

#define N (52)
#define R (5)

int main()
{
	int straightflush = 0, fourofakind = 0, fullhouse = 0;
	int flush = 0, straight = 0, threeofakind = 0;
	int twopair = 0, onepair = 0, highcard = 0;
	int i;
	size_t *cdata;
	ipp_deck *deck;
	ipp_card *cards[R];
	ipp_message *msg;
	gsl_combination *c;

	ipp_init();
	deck = ipp_new_deck();
	assertNotNull("ipp_new_deck() failed", deck);

	c = gsl_combination_calloc(N, R);
	do {
		cdata = gsl_combination_data(c);
		for (i = 0; i < R; i++) {
			cards[i] = deck->cards[cdata[i]];
		}

		msg = ipp_eval(cards);
		assertNotNull("ipp_eval() really screwed up", msg);

		switch (msg->type) {
		case MSG_STRAIGHTFLUSH:
			straightflush++;
			break;
		case MSG_FOUROFAKIND:
			fourofakind++;
			break;
		case MSG_FULLHOUSE:
			fullhouse++;
			break;
		case MSG_FLUSH:
			flush++;
			break;
		case MSG_STRAIGHT:
			straight++;
			break;
		case MSG_THREEOFAKIND:
			threeofakind++;
			break;
		case MSG_TWOPAIR:
			twopair++;
			break;
		case MSG_ONEPAIR:
			onepair++;
			break;
		case MSG_HIGHCARD:
			highcard++;
			break;
		default:
			assertNotNull("Unknown message type.", NULL);
			break;
		}

		ipp_free_message(msg);
	} while (gsl_combination_next(c) == GSL_SUCCESS);
	gsl_combination_free(c);
	c = NULL;

	ipp_free_deck(deck);
	deck = NULL;

	printf("+-----------------+----------+\n");
	printf("| Straight flush  | %8d |\n", straightflush);
	printf("| Four of a kind  | %8d |\n", fourofakind);
	printf("| Full house      | %8d |\n", fullhouse);
	printf("| Flush           | %8d |\n", flush);
	printf("| Straight        | %8d |\n", straight);
	printf("| Three of a kind | %8d |\n", threeofakind);
	printf("| Two Pair        | %8d |\n", twopair);
	printf("| One Pair        | %8d |\n", onepair);
	printf("| High Card       | %8d |\n", highcard);
	printf("+-----------------+----------+\n");
	printf("| Total           | %8d |\n", straightflush + fourofakind + fullhouse + flush + straight + threeofakind + twopair + onepair + highcard);
	printf("+-----------------+----------+\n");

	assertEqual("Wrong count", straightflush, 40);
	assertEqual("Wrong count", fourofakind, 624);
	assertEqual("Wrong count", fullhouse, 3744);
	assertEqual("Wrong count", flush, 5108);
	assertEqual("Wrong count", straight, 10200);
	assertEqual("Wrong count", threeofakind, 54912);
	assertEqual("Wrong count", twopair, 123552);
	assertEqual("Wrong count", onepair, 1098240);
	assertEqual("Wrong count", highcard, 1302540);

	ipp_exit();
	return PASS;
}
