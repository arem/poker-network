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

#ifndef __POKER_H
#define __POKER_H

#define TRUE            1
#define FALSE           0

#define DISCONNECTED   -1
#define FD             -2

#define WIN             (1)
#define TIE		(0)
#define LOSE            (-1)

#define BET_SIZE       10

/* BEGIN protocol cmds */

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


void play(void *);

#define MAX_PLAYERS 10

struct player {
        char username[65];
        int sd;
        int bankroll;
        int amt_in;
        int folded;  /* TRUE | FLASE | DISCONNECTED | FD (folded and disconnected)*/
        int outcome; /* WIN  | TIE | LOSE  */
        struct card *hole[2];
};


struct game_info {
	int num_players;
	int to_call;
	int winners;
	int stage;
	struct player players[MAX_PLAYERS];
        struct card   *board[5];
} game_info;

int observers[128]; /* players not dealt in */

#endif
