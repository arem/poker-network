/*
 * BagelBot - Trivial client for pokerd
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
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

#ifndef __PLAY_H
#define __PLAY_H

#define MAX_PLAYERS    10

#define FOLD            0
#define CALL            1
#define RAISE           2
#define BLIND           3


#define PREFLOP         0
#define FLOP            1
#define TURN            2
#define RIVER           3


#define JOIN_GAME      20
#define GOODPASS       21
#define BADPASS        22
#define BADNICK        23


#define START_NEW_GAME 50
#define HOLE_CARDS     51
#define NEW_STAGE      52
#define WINNERS        53
#define NEXT_TO_ACT    57


#define CHAT           32
#define INFORMATION    43
#define CHATTER        54


#define PING           60
#define PONG           61


#define ACTION         30


#define QUIT_GAME      33
#define GOODBYE        11


#define GET_GRAPH      42
#define SET_FACE       45
#define GET_FACE       46
#define CHANGE_FACE    47

void play();

struct players {
        int position;
        char *username;
        int bankroll;
        int folded;
        int raises;
        int amt_in;
        int wins;
	int face;
} players[MAX_PLAYERS];

struct card {
        char rank;
        char suit;
};

struct card hand[2];
struct card board_cards[5];

int bet_size, num_players, button, position, game_id;
int in_game, stage, games_played, min_bet, max_bet, to_call;

int done;
#endif
