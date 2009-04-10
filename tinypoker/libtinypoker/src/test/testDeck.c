/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <tcort@tomcort.com>
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
#include <stdio.h>

int main()
{
	int i;
	ipp_deck *deck;

	ipp_init();

	deck = ipp_new_deck();
	assertNotNull("ipp_new_deck() failed", deck);

	for (i = 0; i < DECKSIZE; i++) {
		ipp_card *card;
		card = ipp_deck_next_card(deck);
		assertNotNull("ipp_deck_next_card() failed", card);
		printf("%c%c\n", card->rank, card->suit);
	}

	ipp_free_deck(deck);

	printf("\n\n--\n\n");

	deck = ipp_new_deck();
	assertNotNull("ipp_new_deck() failed", deck);

	ipp_shuffle_deck(deck);

	for (i = 0; i < DECKSIZE; i++) {
		ipp_card *card;
		card = ipp_deck_next_card(deck);
		assertNotNull("ipp_deck_next_card() failed", card);
		printf("%c%c\n", card->rank, card->suit);
	}

	ipp_free_deck(deck);

	ipp_exit();
	return PASS;
}
