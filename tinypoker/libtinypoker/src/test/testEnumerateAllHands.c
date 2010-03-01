/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010 Thomas Cort <linuxgeek@gmail.com>
 *
 * This file is part of TinyPoker.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../main/tinypoker.h"
#include "test.h"

#define N (52)
#define R (5)

int main()
{
	int straightflush = 0, fourofakind = 0, fullhouse = 0;
	int flush = 0, straight = 0, threeofakind = 0;
	int twopair = 0, onepair = 0, highcard = 0;
	int i;
	int a, b, c, d, e;
	ipp_deck *deck;
	ipp_card *cards[R];
	ipp_message *msg;

	ipp_init();
	deck = ipp_new_deck();
	assertNotNull("ipp_new_deck() failed", deck);

	for (a = 0; a < N - R + 1; a++) {
		for (b = a + 1; b < N - R + 2; b++) {
			for (c = b + 1; c < N - R + 3; c++) {
				for (d = c + 1; d < N - R + 4; d++) {
					for (e = d + 1; e < N - R + 5; e++) {

						cards[0] = deck->cards[a];
						cards[1] = deck->cards[b];
						cards[2] = deck->cards[c];
						cards[3] = deck->cards[d];
						cards[4] = deck->cards[e];

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

					}
				}
			}
		}
	}

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
