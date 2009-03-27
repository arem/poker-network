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

int main()
{
	ipp_message *msg;
	ipp_card *As, *Ad, *Ah;
	ipp_card *Ks, *Kd, *Kh, *Kc;
	ipp_card *Qs, *Qh, *Qc;
	ipp_card *Jh, *Jc, *Jd;
	guint8 *result;

	ipp_player *player1, *player2, *player3, *player4;
	ipp_table *table;
	int rc, i;

	ipp_init();

	table = ipp_new_table();
	player1 = ipp_new_player();
	player2 = ipp_new_player();
	player3 = ipp_new_player();
	player4 = ipp_new_player();

	assertNotNull("Table should not be null.", table);
	assertNotNull("Player 1 should not be null.", player1);
	assertNotNull("Player 2 should not be null.", player2);
	assertNotNull("Player 3 should not be null.", player3);
	assertNotNull("Player 4 should not be null.", player4);
	assertEqual("Table shouldn't have any players", table->nplayers, 0);

	rc = ipp_add_player(table, player1);
	assertNotEqual("Should have been able to add player", rc, -1);
	assertEqual("Should have one player", table->nplayers, 1);

	rc = ipp_add_player(table, player2);
	assertNotEqual("Should have been able to add player", rc, -1);
	assertEqual("Should have two players", table->nplayers, 2);

	rc = ipp_add_player(table, player3);
	assertNotEqual("Should have been able to add player", rc, -1);
	assertEqual("Should have three players", table->nplayers, 3);

	rc = ipp_add_player(table, player4);
	assertNotEqual("Should have been able to add player", rc, -1);
	assertEqual("Should have four players", table->nplayers, 4);

	Jh = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Jh);
	Jh->rank = JACK;
	Jh->suit = HEARTS;

	Jc = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Jc);
	Jc->rank = JACK;
	Jc->suit = CLUBS;

	Jd = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Jd);
	Jd->rank = JACK;
	Jd->suit = DIAMONDS;

	Qh = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Qh);
	Qh->rank = QUEEN;
	Qh->suit = HEARTS;

	Qc = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Qc);
	Qc->rank = QUEEN;
	Qc->suit = CLUBS;

	Qs = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Qs);
	Qs->rank = QUEEN;
	Qs->suit = SPADES;

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
	table->players[0]->hole[0] = Qs;
	table->players[0]->hole[1] = Qh;
	table->players[1]->hole[0] = As;
	table->players[1]->hole[1] = Ks;
	table->players[2]->hole[0] = Jh;
	table->players[2]->hole[1] = Jd;
	table->players[3]->hole[0] = Qc;
	table->players[3]->hole[1] = Jc;

	table->stage = SHOWDOWN;

	for (i = 0; i < 4; i++) {
		msg = ipp_best_combination(table, i);
		assertNotNull("ipp_best_combination() should have return an answer", msg);

		printf("Player #%d Hand => '%s'\n", i, msg->payload);

		ipp_free_message(msg);
	}

	result = NULL;

	result = ipp_rank_players(table);
	assertNotNull("ipp_rank_players() should have return an answer.", result);

	for (i = 0; i < HOLDEM_PLAYERS_PER_TABLE; i++) {
		int playerid;

		playerid = result[i];

		if (table->players[playerid]) {
			msg = ipp_best_combination(table, playerid);
			assertNotNull("ipp_best_combination() should have return an answer", msg);

			printf("(%d) Player #%d Hand => '%s'\n", i, playerid, msg->payload);

			ipp_free_message(msg);
		}
	}

	/*
	   (0) Player #1 Hand => 'FOUROFAKIND K A'
	   (1) Player #0 Hand => 'FULLHOUSE K Q'
	   (2) Player #2 Hand => 'FULLHOUSE K J'
	   (3) Player #3 Hand => 'THREEOFAKIND K Q J'
	 */

	assertEqual("Player #1 should be 1st", result[0], 1);
	assertEqual("Player #0 should be 2nd", result[1], 0);
	assertEqual("Player #2 should be 3rd", result[2], 2);
	assertEqual("Player #3 should be 4th", result[3], 3);

	ipp_free_table(table);	/* will also free players */
	free(result);

	ipp_exit();
	return PASS;
}
