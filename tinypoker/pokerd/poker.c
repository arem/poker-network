/*
 * Copyright (C) 2005, 2006, 2007 Thomas Cort <code@member.fsf.org>
 *
 * This file is part of pokerd.
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
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

#include "eval.h"
#include "byte.h"
#include "db.h"
#include "poker.h"
#include "sig.h"
#include "monitor.h"
#include "deck.h"
#include "rand.h"
#include "net.h"

extern int snprintf(char *str, size_t size, const char *format, ...);

/**
 *  int pot_size()
 *
 *  return the size of the pot
 */
int pot_size() {
	int i, pot = 0;
	for (i = 0; i < game_info.num_players; i++) {
		pot += game_info.players[i].amt_in;
	}
	return pot;
}

/**
 *  void players_shuffle();
 *
 *  randomizes the order of the players
 */
void players_shuffle() {
	int x, y;
	struct player temp;

	for (x = 0; x < game_info.num_players; x++) {
		/* pick a number between x and game_info.num_players (inclusive) */
		y = rand_get(game_info.num_players-x) + x;


		/*************/
		/* swap them */
		/*************/

		/* temp = x; */
		memcpy(&temp,&(game_info.players[x]),sizeof(struct player));

		/* x = y;    */
		memcpy(&(game_info.players[x]),&(game_info.players[y]),sizeof(struct player));

		/* y = temp; */
		memcpy(&(game_info.players[y]),&temp,sizeof(struct player));
	}
}

/**
 *  void end()
 *
 *  zero out all of the data
 */
void end() {
	int i;

	game_info.to_call = 0;
	game_info.winners = 0;
	game_info.num_players = 0;
	for (i = 0; i < 5; i++)
		game_info.board[i] = NULL;

	for (i = 0; i < 128; i++)
		observers[i] = 0;

	for (i = 0; i < MAX_PLAYERS; i++) {
		bzero(&(game_info.players[i]),sizeof(struct player));
		game_info.players[i].outcome = LOSE;
	}
}


/**
 *  void begin()
 *
 *  initialize the game
 */
void begin() {
	int i, j;
	struct byte_array *ba;

	do {
		end();
		db_add_players();
		players_shuffle();

		if (game_info.num_players < 2) sleep(3);
		if (done) return;
	} while (game_info.num_players < 2);

	daemon_log(LOG_INFO,"[SEND] START_NEW_GAME");
	for (i = 0; i < game_info.num_players; i++) {

		if (game_info.players[i].sd != DISCONNECTED && game_info.players[i].sd != FD) {
			ba = new_byte_array(64);

			byte_array_append_int(ba,BET_SIZE); /* bet size (10 in a 10-20 game) */
			byte_array_append_int(ba,game_info.num_players); /* number of players */
			byte_array_append_int(ba,game_info.num_players-1); /* button (always last player) */
			byte_array_append_int(ba,i); /* player position */
			byte_array_append_int(ba,db_get_games_played()); /* game id */


			for (j = 0; j < game_info.num_players; j++) {
				byte_array_append_string(ba,game_info.players[j].username); /* username */
				byte_array_append_int(ba,game_info.players[j].bankroll); /* bankroll */
				byte_array_append_int(ba,0); /*face id*/
			}

			write_message(game_info.players[i].sd,START_NEW_GAME,ba);
			byte_array_destroy(ba);
		}
	}

	for (i = 0; i < 128 && observers[i]; i++) {
		ba = new_byte_array(64);

		byte_array_append_int(ba,BET_SIZE); /* bet size (10 in a 10-20 game) */
		byte_array_append_int(ba,game_info.num_players); /* number of players */
		byte_array_append_int(ba,game_info.num_players-1); /* button (always last player) */
		byte_array_append_int(ba,-1); /* player position (-1 == not dealt in)*/
		byte_array_append_int(ba,db_get_games_played()); /* game id */


		for (j = 0; j < game_info.num_players; j++) {
			byte_array_append_string(ba,game_info.players[j].username); /* username */
			byte_array_append_int(ba,game_info.players[j].bankroll); /* bankroll */
			byte_array_append_int(ba,0); /*face id*/
		}

		write_message(observers[i],START_NEW_GAME,ba);
		byte_array_destroy(ba);
	}
}

/**
 *  void deal()
 *
 *  deal 2 "hole cards" to each player
 */
void deal() {
	int i, j;
	char buf[128];
	bzero(buf,128);

	game_info.stage = PREFLOP;

	for (j = 0; j < 2; j++) {
		for (i = 0; i < game_info.num_players; i++) {
			if (game_info.players[i].sd && game_info.players[i].sd != DISCONNECTED && game_info.players[i].sd != FD) {
				game_info.players[i].hole[j] = get_card();

				if (j) {
					/* send whole cards */
					struct byte_array *ba = new_byte_array(20);
					byte_array_append_int(ba,i);

					snprintf(buf, 128, "%c%c %c%c", game_info.players[i].hole[0]->rank,
									game_info.players[i].hole[0]->suit,
									game_info.players[i].hole[1]->rank,
									game_info.players[i].hole[1]->suit);

					byte_array_append_string(ba,buf);
					write_message(game_info.players[i].sd,HOLE_CARDS,ba);
					byte_array_destroy(ba);

					/* log the deal */
					daemon_log(LOG_INFO, "[SEND] HOLE_CARDS %s %c%c %c%c", game_info.players[i].username,
									game_info.players[i].hole[0]->rank,
									game_info.players[i].hole[0]->suit,
									game_info.players[i].hole[1]->rank,
									game_info.players[i].hole[1]->suit);
				}
			}
		}
	}
}

/**
 *  int num_still_in()
 *
 *  returns the number of players not folded/disconnected
 */
int num_still_in() {
	int i, still_in = 0;

	for (i = 0; i < game_info.num_players; i++) {
		if (game_info.players[i].folded == FALSE && game_info.players[i].sd != DISCONNECTED) {
			still_in++;
		}
	}

	return still_in;
}

/**
 *  int all_even()
 *
 *  Determines if evenone in the game has the same amt_in
 */
int all_even() {
	int i;
	int even = 1;
	int amt_in = -1;

	for (i = 0; i < game_info.num_players; i++) {

		if (game_info.players[i].folded == FALSE) {

			if (amt_in == -1) {

				/* set the initial amt_in amount */
				amt_in = game_info.players[i].amt_in;

			} else if (game_info.players[i].amt_in != amt_in) {

				/* someone has a different amt_in amount */
				even = 0;
				break;
			}

		}

	}

	return even;
}

/**
 *  void bet()
 *
 *  Accept bets from payers
 */
void bet() {
	int i, betting_allowed = 3, action_on = 0, action, bet_amt;
	struct byte_array *ba;

	/* Pay the blinds if needed */
	if (game_info.stage == PREFLOP) {
		ba = new_byte_array(10);
		byte_array_append_int(ba,action_on);
		byte_array_append_int(ba,BET_SIZE/2); /* small blind */
		write_message_all(BLIND,ba);
		byte_array_destroy(ba);

		game_info.players[action_on].amt_in   += BET_SIZE/2;
		game_info.players[action_on].bankroll -= BET_SIZE/2;

		action_on = (action_on+1) % game_info.num_players;

		ba = new_byte_array(10);
		byte_array_append_int(ba,action_on);
		byte_array_append_int(ba,BET_SIZE); /* big blind */
		write_message_all(BLIND,ba);
		byte_array_destroy(ba);

		game_info.players[action_on].amt_in   += BET_SIZE;
		game_info.players[action_on].bankroll -= BET_SIZE;

		action_on = (action_on+1) % game_info.num_players;

		game_info.to_call = BET_SIZE;
	}

	for (i = 0; i < (game_info.num_players*4); i++) {

		if (game_info.players[action_on].folded == DISCONNECTED) {
			game_info.players[action_on].folded = FD;
			ba = new_byte_array(4);
			byte_array_append_int(ba,action_on);
			write_message_all(FOLD,ba);
			byte_array_destroy(ba);
		}

		if (num_still_in() <= 1) {
			return;
		}

		if (game_info.players[action_on].folded == FALSE) {
			ba = new_byte_array(40);
			byte_array_append_int(ba,action_on);
			byte_array_append_int(ba,(game_info.to_call-(game_info.players[action_on].amt_in)));

			/* allow a max of 3 raises */
			if (betting_allowed) {

				/* We play 2-2-4-4 limit texas hold'em with fixed bet sizes*/
				if (game_info.stage == PREFLOP || game_info.stage == FLOP) {
					byte_array_append_int(ba,BET_SIZE);	/* smallest bet allowed */
					byte_array_append_int(ba,BET_SIZE);	/* largest bet allowed */
				} else {
					byte_array_append_int(ba,BET_SIZE*2);	/* smallest bet allowed */
					byte_array_append_int(ba,BET_SIZE*2);	/* largest bet allowed */
				}

			} else {
				byte_array_append_int(ba,-1);	/* no more raises */
				byte_array_append_int(ba,-1);	/* no more raises */
			}

			write_message_all(NEXT_TO_ACT,ba);

			daemon_log(LOG_INFO,"[SEND] NEXT_TO_ACT %s %d",game_info.players[action_on].username,action_on);

			byte_array_destroy(ba);

			ba = read_message(game_info.players[action_on].sd,&action);
			action = byte_array_read_int(ba);

			if (ba == NULL) {

				/* couldn't read from player */
				game_info.players[action_on].folded = FD;

				/* send fold */
				ba = new_byte_array(10);
				byte_array_append_int(ba,action_on);
				write_message_all(FOLD,ba);
				byte_array_destroy(ba);

				daemon_log(LOG_INFO,"[SEND] FOLD %s",game_info.players[action_on].username);

			} else {

				switch (action) {
					case FOLD:
						byte_array_destroy(ba);
						game_info.players[action_on].folded = TRUE;

						/* send fold */
						ba = new_byte_array(10);
						byte_array_append_int(ba,action_on);
						write_message_all(FOLD,ba);
						byte_array_destroy(ba);

						daemon_log(LOG_INFO,"[SEND] FOLD %s",game_info.players[action_on].username);
						break;

					case CALL:
						bet_amt = (game_info.to_call - game_info.players[action_on].amt_in);
						game_info.players[action_on].bankroll -= bet_amt;
						game_info.players[action_on].amt_in   += bet_amt;
						byte_array_destroy(ba);

						/* send call */
						ba = new_byte_array(10);
						byte_array_append_int(ba,action_on);
						byte_array_append_int(ba,bet_amt);
						write_message_all(CALL,ba);
						byte_array_destroy(ba);

						daemon_log(LOG_INFO,"[SEND] CALL %s",game_info.players[action_on].username);
						break;

					case RAISE:
						/* allow a max of 3 raises */
						if (betting_allowed) {

							/* meet the current bets */
							game_info.players[action_on].bankroll -= (game_info.to_call-(game_info.players[action_on].amt_in));
							game_info.players[action_on].amt_in   += (game_info.to_call-(game_info.players[action_on].amt_in));

							/* raise bet_amt */
							bet_amt = (game_info.stage == PREFLOP || game_info.stage == FLOP) ? BET_SIZE : (BET_SIZE*2);
							game_info.players[action_on].bankroll -= bet_amt;
							game_info.players[action_on].amt_in   += bet_amt;
							byte_array_destroy(ba);

							game_info.to_call += bet_amt;

							/* send raise */
							ba = new_byte_array(10);
							byte_array_append_int(ba,action_on);
							byte_array_append_int(ba,bet_amt);
							write_message_all(RAISE,ba);
							byte_array_destroy(ba);

							daemon_log(LOG_INFO,"[SEND] RAISE %s",game_info.players[action_on].username);

							betting_allowed--;

						} else { /* no betting allowed; just call/check */

							/* meet the current bets */
							bet_amt = (game_info.to_call - game_info.players[action_on].amt_in);
							game_info.players[action_on].bankroll -= bet_amt;
							game_info.players[action_on].amt_in   += bet_amt;
							byte_array_destroy(ba);

							/* send call */
							ba = new_byte_array(10);
							byte_array_append_int(ba,action_on);
							byte_array_append_int(ba,bet_amt);
							write_message_all(CALL,ba);
							byte_array_destroy(ba);

							daemon_log(LOG_INFO,"[SEND] CALL %s",game_info.players[action_on].username);

						}
						break;


					default:
						/* Invalid Message */

						daemon_log(LOG_ERR,"[ERRR] Invalid Message %s",game_info.players[action_on].username);

						byte_array_destroy(ba);
						game_info.players[action_on].folded = FD;

						/* send fold */
						ba = new_byte_array(10);
						byte_array_append_int(ba,action_on);
						write_message_all(FOLD,ba);
						byte_array_destroy(ba);

						daemon_log(LOG_INFO,"[SEND] FOLD %s",game_info.players[action_on].username);

						break;
				} /* end switch */
			} /* end else */
		} /* end if (game_info.players[action_on].folded == FALSE) */

		if ((i >= (game_info.num_players-1)) && all_even()) {
			return;
		}

		action_on = (action_on+1) % game_info.num_players;
	}

}

/**
 *  void flop()
 *
 *  put first 3 cards on the board
 */
void flop() {
	char buf[128];
	struct byte_array *ba;
	bzero(buf,128);

	game_info.stage = FLOP;

	/* In theory we don't have to burn cards because the computer should shuffle
           better than any human, however we will remain true to poker tradition and
	   burn the top card before cards are placed on the board in each betting phase (flop, turn, river) */

	/* burn card */      get_card();
	game_info.board[0] = get_card();
	game_info.board[1] = get_card();
	game_info.board[2] = get_card();

	snprintf(buf,128,"%c%c %c%c %c%c",
		game_info.board[0]->rank,game_info.board[0]->suit,
		game_info.board[1]->rank,game_info.board[1]->suit,
		game_info.board[2]->rank,game_info.board[2]->suit
	);

	ba = new_byte_array(20);

	byte_array_append_int(ba,FLOP);
	byte_array_append_string(ba,buf);

	write_message_all(NEW_STAGE,ba);
	byte_array_destroy(ba);

	daemon_log(LOG_INFO,"[SEND] NEW_STAGE FLOP %c%c %c%c %c%c",
		game_info.board[0]->rank,game_info.board[0]->suit,
		game_info.board[1]->rank,game_info.board[1]->suit,
		game_info.board[2]->rank,game_info.board[2]->suit
	);

}

/**
 *  void turn()
 *
 *  put 4th card on the board
 */
void turn() {
	char buf[128];
	struct byte_array *ba;

	bzero(buf,128);

	game_info.stage = TURN;

	/* burn card */      get_card();
	game_info.board[3] = get_card();

	snprintf(buf,128,"%c%c %c%c %c%c %c%c",
		game_info.board[0]->rank,game_info.board[0]->suit,
		game_info.board[1]->rank,game_info.board[1]->suit,
		game_info.board[2]->rank,game_info.board[2]->suit,
		game_info.board[3]->rank,game_info.board[3]->suit
	);

	ba = new_byte_array(20);

	byte_array_append_int(ba,TURN);
	byte_array_append_string(ba,buf);

	write_message_all(NEW_STAGE,ba);
	byte_array_destroy(ba);

	daemon_log(LOG_INFO,"[SEND] NEW_STAGE TURN %c%c %c%c %c%c %c%c",
		game_info.board[0]->rank,game_info.board[0]->suit,
		game_info.board[1]->rank,game_info.board[1]->suit,
		game_info.board[2]->rank,game_info.board[2]->suit,
		game_info.board[3]->rank,game_info.board[3]->suit
	);
}

/**
 *  void river()
 *
 *  put 5th card on the board
 */
void river() {
	char buf[128];
	struct byte_array *ba;

	bzero(buf,128);

	game_info.stage = RIVER;

	/* burn card */      get_card();
	game_info.board[4] = get_card();

	snprintf(buf,128,"%c%c %c%c %c%c %c%c %c%c",
		game_info.board[0]->rank,game_info.board[0]->suit,
		game_info.board[1]->rank,game_info.board[1]->suit,
		game_info.board[2]->rank,game_info.board[2]->suit,
		game_info.board[3]->rank,game_info.board[3]->suit,
		game_info.board[4]->rank,game_info.board[4]->suit
	);

	ba = new_byte_array(20);

	byte_array_append_int(ba,RIVER);
	byte_array_append_string(ba,buf);

	write_message_all(NEW_STAGE,ba);
	byte_array_destroy(ba);

	daemon_log(LOG_INFO,"[SEND] NEW_STAGE RIVER %c%c %c%c %c%c %c%c %c%c",
		game_info.board[0]->rank,game_info.board[0]->suit,
		game_info.board[1]->rank,game_info.board[1]->suit,
		game_info.board[2]->rank,game_info.board[2]->suit,
		game_info.board[3]->rank,game_info.board[3]->suit,
		game_info.board[4]->rank,game_info.board[4]->suit
	);

}

/**
 *  void showdown()
 *
 *  sends card info to players at the showdown
 */
void showdown() {
	int i;
	struct byte_array *ba;
	char cards[6], buf[128];

	for (i = 0; i < game_info.num_players; i++) {

		if (game_info.players[i].folded == FALSE && game_info.players[i].sd != DISCONNECTED) {
			ba = new_byte_array(20);

			bzero(cards,6);
			bzero(buf,128);

			byte_array_append_int(ba, i); /* who */
			cards[0] = game_info.players[i].hole[0]->rank;
			cards[1] = game_info.players[i].hole[0]->suit;
			cards[2] = ' ';
			cards[3] = game_info.players[i].hole[1]->rank;
			cards[4] = game_info.players[i].hole[1]->suit;
			cards[5] = '\0';
			byte_array_append_string(ba,cards); /* cards */
			write_message_all(HOLE_CARDS,ba);
			byte_array_destroy(ba);

			daemon_log(LOG_INFO,"[SEND] HOLE_CARDS %d %s",i,cards);
		}
	}

}


/**
 *  void winners()
 *
 *  sends info about winning hands
 */
void winners() {
	int i, pot;
	struct byte_array *ba;

	pot = pot_size();

	/* if there is only 1 player he automatically wins (ie the other players all folded         */
	/* we need to do this here because evalmain() isn't called when there is only 1 player left */
	if (num_still_in() == 1) {
		for (i = 0; i < game_info.num_players; i++) {
			if (game_info.players[i].folded == FALSE) {
				game_info.players[i].outcome = WIN;
				game_info.winners++;
				break;
			}
		}
	}

	ba = new_byte_array(40);
	byte_array_append_int(ba, game_info.winners);

	daemon_log(LOG_INFO,"[SEND] WINNERS %d",game_info.winners);

	if (game_info.winners == 1) {
		/* if 1 winner, then we don't have to divide the pot */

		for (i = 0; i < game_info.num_players; i++) {
			if (game_info.players[i].outcome == WIN) {
				byte_array_append_int(ba, i);   /* who */
				byte_array_append_int(ba, pot); /* amt */

				game_info.players[i].bankroll += pot;

				daemon_log(LOG_INFO,"[SEND] WINNERS %s %d",game_info.players[i].username,pot);
				break;
			}
		}
	} else {
		if ((pot % game_info.winners) == 0) {
			/* the pot divides evenly */

			for (i = 0; i < game_info.num_players; i++) {
				if (game_info.players[i].outcome == TIE) {
					byte_array_append_int(ba, i);                     /* who */
					byte_array_append_int(ba, pot/game_info.winners); /* amt */

					game_info.players[i].bankroll += pot/game_info.winners;

					daemon_log(LOG_INFO,"[SEND] WINNERS %s %d",game_info.players[i].username,pot/game_info.winners);
				}
			}
		} else {
			/* The money cannot be evenly divided; give the left over money to the 1st winner */

			int extra = pot % game_info.winners;
			pot -= extra;

			for (i = 0; i < game_info.num_players; i++) {
				if (game_info.players[i].outcome == TIE) {
					byte_array_append_int(ba, i);                           /* who */
					byte_array_append_int(ba, pot/game_info.winners+extra); /* amt */

					game_info.players[i].bankroll += pot/game_info.winners+extra;

					extra = 0; /* only one player (the 1st player) gets extra */

                                        daemon_log(LOG_INFO,"[SEND] WINNERS %s %d",game_info.players[i].username,pot/game_info.winners+extra);
				}
			}
		}
	}

	write_message_all(WINNERS,ba);
	byte_array_destroy(ba);
}

/**
 *  void store_results()
 *
 *  puts the game results in the database
 */
void store_results() {
	int i;

	for (i = 0; i < game_info.num_players; i++) {
		db_store_bankroll(game_info.players[i].username, game_info.players[i].bankroll);
		db_store_last_game(game_info.players[i].username);
		db_inc_user_games_played(game_info.players[i].username);

		if (game_info.players[i].folded == FALSE) {
			switch (game_info.players[i].outcome) {
				case WIN:
					db_inc_wins(game_info.players[i].username);
					break;
				case TIE:
					db_inc_ties(game_info.players[i].username);
					break;
				case LOSE:
					db_inc_loses(game_info.players[i].username);
					break;
			}
		} else {
			db_inc_folds(game_info.players[i].username);
		}
	}

	db_inc_games_played();
}

/**
 *  void play(void *v)
 *
 *  game play thread
 */
void play(void *v) {

	while(!done) {
		begin();
		if (done) break;

		deal();
		if (done) break;

		if (num_still_in() > 1) {
			bet();
		}

		if (done) break;

		if (num_still_in() > 1) {
			flop();
			if (done) break;
			bet();
		}

		if (done) break;

		if (num_still_in() > 1) {
			turn();
			if (done) break;
			bet();
		}

		if (done) break;

		if (num_still_in() > 1) {
			river();
			if (done) break;
			bet();
		}
		if (done) break;

		if (num_still_in() > 1) {
			evalmain();
		}
		if (done) break;

		if (num_still_in() > 0) {
			showdown();
			if (done) break;

			winners();
			if (done) break;

			store_results();
			if (done) break;
		} /* else everyone is either folded or disconnected */

		end();
		if (done) break;

		deck_shuffle();
	}

	monitor_dec();
}
