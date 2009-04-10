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

int main()
{
	ipp_message *msg;
	ipp_card *Ks, *Kd, *Kh, *Kc;
	ipp_card *As, *Ad, *Ah;
	ipp_player *player;
	ipp_table *table;
	int rc, i;

	ipp_init();

	table = ipp_new_table();
	player = ipp_new_player();

	assertNotNull("Table should not be null.", table);
	assertNotNull("Player should not be null.", player);
	assertEqual("Table shouldn't have any players", table->nplayers, 0);

	rc = ipp_add_player(table, player);
	assertNotEqual("Should have been able to add player", rc, -1);
	assertEqual("Should have one player", table->nplayers, 1);

	Kh = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Kh);
	Kh->rank = KING;
	Kh->suit = HEARTS;

	Kc = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Kc);
	Kc->rank = KING;
	Kc->suit = CLUBS;

	Ks = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Ks);
	Ks->rank = KING;
	Ks->suit = SPADES;

	Kd = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Kd);
	Kd->rank = KING;
	Kd->suit = DIAMONDS;

	Ah = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Ah);
	Ah->rank = ACE;
	Ah->suit = HEARTS;

	As = ipp_new_card();
	assertNotNull("ipp_new_card() failed", As);
	As->rank = ACE;
	As->suit = SPADES;

	Ad = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Ad);
	Ad->rank = ACE;
	Ad->suit = DIAMONDS;

	table->board[0] = Ad;
	table->board[1] = Ah;
	table->board[2] = Kd;
	table->board[3] = Kh;
	table->board[4] = Kc;
	table->players[0]->hole[0] = As;
	table->players[0]->hole[1] = Ks;
	table->stage = SHOWDOWN;

	msg = ipp_best_combination(table, 0);
	assertNotNull("ipp_best_combination() should have return an answer", msg);

	printf("%s\n", msg->payload);
	assertStringEqual("Out of Ad Ah As Kd Ks Kh Kc, FOUROFAKIND K A should be the best hand.", "FOUROFAKIND K A", msg->payload);

	ipp_free_message(msg);
	ipp_free_table(table);	/* will also free player */

	ipp_exit();
	return PASS;
}
