/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
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

#ifndef __DB_H
#define __DB_H

#include <pthread.h>

void db_connect();
void db_disconnect();
int  db_auth(int,char*,char*);
void db_add_players();
void db_player_kill(int sd);
void db_players_kill();
int db_get_games_played();
void db_store_bankroll(char *, int);
void db_inc_games_played();
void db_store_last_game(char *username);
void db_inc_user_games_played(char*);
void db_inc_wins(char*);
void db_inc_ties(char*);
void db_inc_loses(char*);
void db_inc_folds(char*);
pthread_mutex_t db_lock;

#endif
