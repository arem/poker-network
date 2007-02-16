/*
 * BagelBot - Trivial client for pokerd
 * Copyright (C) 2005, 2007 Thomas Cort <code@member.fsf.org>
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

#include "play.h"
#include "byte.h"
#include "net.h"

#include <strings.h>

void start_new_game(struct byte_array *ba) {
	int i;

	bet_size     = byte_array_read_int(ba);
	num_players  = byte_array_read_int(ba);
	button       = byte_array_read_int(ba);
	position     = byte_array_read_int(ba);
	game_id      = byte_array_read_int(ba);
	in_game      = (position != -1) ? 1 : 0;
	stage        = PREFLOP;
	games_played = games_played + 1;

	printf("Begin new game...\n");

	/* read in the details for each player */
	for (i = 0; i < num_players; i++) {
		if (players[i].username)
			free(players[i].username);

		players[i].username = byte_array_read_string(ba);
		players[i].bankroll = byte_array_read_int(ba);
		players[i].face     = byte_array_read_int(ba);
		players[i].position = i;
		players[i].folded   = 0;
		players[i].raises   = 0;
		players[i].amt_in   = 0;

		printf("\t%d) %s %d\n",i,players[i].username,players[i].bankroll);
	}
}

void hole_cards(struct byte_array *ba) {
	int   who        = byte_array_read_int(ba);
	char *hole_cards = byte_array_read_string(ba);

	if (who == position) /* my cards */ {
		hand[0].rank = hole_cards[0];
		hand[0].suit = hole_cards[1];
		hand[1].rank = hole_cards[3];
		hand[1].suit = hole_cards[4];
		printf("Your hole cards... %s\n",hole_cards);
	} else {
		printf("Their hole cards... %s\n",hole_cards);
	}

	free(hole_cards);
}

void new_stage(struct byte_array *ba) {
	int round       = byte_array_read_int(ba);
	char *new_cards = byte_array_read_string(ba);

	stage = round;

	switch (stage) {
		case PREFLOP:
			break;

		case FLOP:
			board_cards[0].rank = new_cards[ 0];
			board_cards[0].suit = new_cards[ 1];
			/* space            = new_cards[ 2]; */
			board_cards[1].rank = new_cards[ 3];
			board_cards[1].suit = new_cards[ 4];
			/* space            = new_cards[ 5]; */
			board_cards[2].rank = new_cards[ 6];
			board_cards[2].suit = new_cards[ 7];
			printf("Flop: %s\n",new_cards);
			break;

		case TURN:
			/* space            = new_cards[ 8]; */
			board_cards[3].rank = new_cards[ 9];
			board_cards[3].suit = new_cards[10];
			printf("Turn: %s\n",new_cards);
			break;

		case RIVER:
			/* space            = new_cards[11]; */
			board_cards[4].rank = new_cards[12];
			board_cards[4].suit = new_cards[13];
			printf("River: %s\n",new_cards);
			break;

		default:
			break;
	}
}

void fold(struct byte_array *ba) {
	int who = byte_array_read_int(ba);
	players[who].folded = 1;
	printf("Fold: %s\n",players[who].username);
}

void call(struct byte_array *ba) {
	int who = byte_array_read_int(ba);
	int amt = byte_array_read_int(ba);

	players[who].amt_in   += amt;
	players[who].bankroll -= amt;
	printf("Call: %s ($%d)\n",players[who].username,amt);
}

void blind(struct byte_array *ba) {
	int who = byte_array_read_int(ba);
	int amt = byte_array_read_int(ba);

	players[who].amt_in   += amt;
	players[who].bankroll -= amt;
	printf("Blind: %s ($%d)\n",players[who].username,amt);
}

void __raise(struct byte_array *ba) {
	int who = byte_array_read_int(ba);
	int amt = byte_array_read_int(ba);

	players[who].amt_in   += amt;
	players[who].bankroll -= amt;
	players[who].raises   +=   1;
	printf("Raise: %s ($%d)\n",players[who].username,amt);
}

void winners(struct byte_array *ba) {
	int num = byte_array_read_int(ba), who, share, i;

	for (i = 0; i < num; i++) {
		who   = byte_array_read_int(ba);
		share = byte_array_read_int(ba);

		players[who].bankroll += share;
		players[who].wins++;
		printf("%s won $%d\n",players[who].username,share);
	}
}

void next_to_act(struct byte_array *ba) {
	int action;
	struct byte_array *b;
	int next;

	next    = byte_array_read_int(ba);
	to_call = byte_array_read_int(ba);
	min_bet = byte_array_read_int(ba);
	max_bet = byte_array_read_int(ba);

	if (next == position) {
		printf("It is on you... $%d to call, bet size is $%d\n",to_call,min_bet);


		/* YOUR CODE HERE -- set 'action' to one of FOLD CALL or RAISE */
		action = FOLD;

		b = new_byte_array(1);

		switch (action) {
			case FOLD:
				byte_array_append_int(b, FOLD);
				break;
			case CALL:
				byte_array_append_int(b, CALL);
				break;
			case RAISE:
				byte_array_append_int(b, RAISE);
				break;
			default:
				byte_array_append_int(b, FOLD);
				break;
		}

		write_message(ACTION,b);
		byte_array_destroy(b);
	}
}

void play() {
	int id;
	struct byte_array *ba;

	bzero(players,sizeof(struct players[MAX_PLAYERS]));

	while (!done) {
		ba = read_message(&id);

		switch (id) {
			case START_NEW_GAME:
				start_new_game(ba);
				break;

			case HOLE_CARDS:
				hole_cards(ba);
				break;

			case FOLD:
				fold(ba);
				break;

			case CALL:
				call(ba);
				break;

			case RAISE:
				__raise(ba);
				break;

			case BLIND:
				blind(ba);
				break;

			case NEW_STAGE:
				new_stage(ba);
				break;

			case WINNERS:
				winners(ba);
				break;

			case NEXT_TO_ACT:
				next_to_act(ba);
				break;

			case PING:
			case INFORMATION:
			case SET_FACE:
			case CHATTER:
			default:
				break;
		}

		byte_array_destroy(ba);
	}

	for (id = 0; id < MAX_PLAYERS; id++) {
		if (players[id].username) {
			free(players[id].username);
		}
	}
}
