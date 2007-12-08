/*
 * Copyright (C) 2005, 2006, 2007 Thomas Cort <code@member.fsf.org>
 *
 * This file is part of tinypokerd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <libdaemon/dlog.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include "deck.h"

struct card deck[52];

int deck_index;

char suits[4] = { 's', 'd', 'c', 'h' };
char ranks[13] = { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' };

/**
 *  struct card *get_card()
 *
 *  get the top card off the deck
 */
struct card *get_card()
{
	deck_index = (deck_index + 1) % 52;
	return &deck[deck_index];
}

/**
 *  char *card2str(struct card *c)
 *
 *  gives a null terminated string representation of a card c (ex: "9d\0")
 */
char *card2str(struct card *c)
{
	char *str;

	if ((str = (char *) malloc(3)) == NULL) {
		daemon_log(LOG_ERR, "[DECK] %s", strerror(errno));
		exit(1);
	}

	str[0] = c->rank;
	str[1] = c->suit;
	str[2] = '\0';

	return str;
}

/**
 *  void deck_shuffle()
 *
 *  randomize the order of the cards in the deck (Knuth shuffle)
 */
void deck_shuffle()
{
	int x, y, z, r;
	struct card temp;

	for (z = 0; z < 7; z++) {
		for (x = 0; x < 51; x++) {
			r = rand();
			if (r < 0) {	/* force 'r' to be positive */
				r = 0 - r;
			}

			/* pick a number between x and 51 (inclusive) */
			y = (r % (52 - x)) + x;

			/* swap them */
			temp = deck[x];
			deck[x] = deck[y];
			deck[y] = temp;
		}
	}

	/* when get_card is 1st called it will return card 0 */
	deck_index = 51;

}

/**
 *  void deck_init()
 *
 *  initializes a deck
 */
void deck_init()
{
	int x, y, z = 0;
	deck_index = 51;	/* when get_card is 1st called it will return card 0 */

	srand(getpid());

	for (x = 0; x < 4; x++) {
		for (y = 0; y < 13; y++) {
			deck[z].rank = ranks[y];
			deck[z].suit = suits[x];
			z++;
		}
	}

	deck_shuffle();
}
