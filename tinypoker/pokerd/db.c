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

#include "md5.h"
#include "db.h"
#include "poker.h"
#include "net.h"
#include "conf.h"
#include "log.h"
#include "auth.h"

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int snprintf(char *str, size_t size, const char *format, ...);

MYSQL *db;

/**
 *  void db_connect()
 *
 *  connects to a MySQL Database
 */
void db_connect() {
	char my_true = 1;

	/* multiple threads cannot share a single MYSQL*, so we have to control sharing */
	pthread_mutex_lock(&db_lock);

	db = (MYSQL *) malloc(sizeof(MYSQL));
	mysql_init(db);

	/* enable auto re-connect */
	if (mysql_options(db, MYSQL_OPT_RECONNECT, &my_true)) {
		logit((char *)mysql_error(db));
		exit(1);
	}

	/* connect */
	if (!mysql_real_connect(db,dbhostname,dbusername,dbpassword,dbdatabase,0,NULL,0)) {
		logit((char *)mysql_error(db));
		exit(1);
	}

	logit("[DBDB] Connected");
}

/**
 *  int db_auth(char*,char*)
 *
 *  Lookup user in the database
 */

int db_auth(int sd, char* username, char* password) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[256];
	char safe_username[(128)+1];
	int rc = 0;
	char *passwd;
	bzero(query,256);
	bzero(safe_username,128+1);

	if (strlen(username) < 64) {
		mysql_real_escape_string(db, safe_username, username, strlen(username));
	} else {
		struct byte_array *ba = new_byte_array(1);
		write_message(sd,BADNICK,ba);
		byte_array_destroy(ba);
	}

	snprintf(query,255,"SELECT passwd FROM users WHERE username LIKE '%s' AND sd = 0",safe_username);

	db_connect();
	if (mysql_real_query(db,query,(unsigned int) strlen(query))) {
		logit((char *)mysql_error(db));
		exit(1);
	}
	res = mysql_store_result(db);
	db_disconnect();

	/* Check if user exists and isn't already logged in */
	if (mysql_num_rows(res) == 1) {
		row = mysql_fetch_row(res);
		passwd = md5str(password);

		/* Check Correct Password */
		if (!strcmp(row[0],passwd)) {
			rc = 1;

			/* Good password */
			snprintf(query,255,"UPDATE users SET sd = %d WHERE username LIKE '%s'",sd,safe_username);
			db_connect();
			if (mysql_real_query(db,query,(unsigned int) strlen(query))) {
				logit((char *)mysql_error(db));
				exit(1);
			}
			db_disconnect();

		} else {
			rc = 0;
		}

		free(passwd);
	} else {
		rc = 0;
	}
	mysql_free_result(res);

	return rc;
}

/**
 *  void db_add_players()
 *
 *  Grab players from database for the next game
 */
void db_add_players() {
	MYSQL_RES *res;
	MYSQL_ROW row;

	int i;

	/* Make sure we don't get more than 10 players & pick users who haven't played in a while */
	char query[128];
	bzero(query,128);

	strcpy(query,"SELECT username,bankroll,sd FROM users WHERE sd != 0 ORDER BY last_game ASC LIMIT 10");

	/* don't allow changes while we get players list. */
	pthread_mutex_lock(&auth_lock);
	db_connect();

	if (mysql_real_query(db,query,(unsigned int) strlen(query))) {
		logit((char *)mysql_error(db));
		pthread_mutex_unlock(&auth_lock);
		exit(1);
	}
	
	/* Don't unlock mutex until observers are added */

	res = mysql_store_result(db);
	db_disconnect();

	game_info.num_players = 0;

	if (mysql_num_rows(res) > 0) {

		for (i = 0; i < mysql_num_rows(res); i++) {

			row = mysql_fetch_row(res);

			strncpy(game_info.players[i].username,row[0],64);

			game_info.players[i].bankroll = atoi(row[1]);
			game_info.players[i].sd       = atoi(row[2]);

			game_info.players[i].amt_in  = 0;
			game_info.players[i].folded  = FALSE;
			game_info.players[i].outcome = LOSE;
			game_info.players[i].hole[0] = NULL;
			game_info.players[i].hole[1] = NULL;

			game_info.num_players++;

		}

	} 

	mysql_free_result(res);


	bzero(query,128);

	/* Select players not being dealt in (we let them watch) */
	strcpy(query,"SELECT sd FROM users WHERE sd != 0 ORDER BY last_game ASC LIMIT 10,127");

	db_connect();

	if (mysql_real_query(db,query,(unsigned int) strlen(query))) {
		logit((char *)mysql_error(db));
		pthread_mutex_unlock(&auth_lock);
		exit(1);
	}
	
	res = mysql_store_result(db);
	db_disconnect();
	pthread_mutex_unlock(&auth_lock);

	if (mysql_num_rows(res) > 0) {
		for (i = 0; i < mysql_num_rows(res); i++) {
			row = mysql_fetch_row(res);
			observers[i] = atoi(row[0]);
		}
	} 

	mysql_free_result(res);

}

/**
 *  void db_store_bankroll(char *username, int bankroll)
 *
 *  sets the bankroll for a player
 */
void db_store_bankroll(char *username, int bankroll) {
	char query[512];
	bzero(query,512);
	snprintf(query,511,"UPDATE users SET bankroll = %d WHERE username like '%s'",bankroll,username);

	db_connect();
	if (mysql_real_query(db,query,(unsigned int) strlen(query))) {
		logit((char *)mysql_error(db));
		exit(1);
	}
	db_disconnect();
}

/**
 *  void db_store_last_game(char *username)
 *
 *  sets the bankroll for a player
 */
void db_store_last_game(char *username) {
	int gameid;
	char query[512];
	bzero(query,512);
	gameid = db_get_games_played() + 1;
	snprintf(query,511,"UPDATE users SET last_game = %d WHERE username like '%s'",gameid,username);

	db_connect();
	if (mysql_real_query(db,query,(unsigned int) strlen(query))) {
		logit((char *)mysql_error(db));
		exit(1);
	}
	db_disconnect();
}

/**
 *  void db_player_kill(int sd)
 *
 *  set a player's sd to 0 in the database and close the socket
 */
void db_player_kill(int sd) {
	int i;
	char query[256];
	bzero(query,256);

	snprintf(query,255,"UPDATE users SET sd = 0 WHERE sd = %d",sd);
	db_connect();
	if (mysql_real_query(db,query,(unsigned int) strlen(query))) {
		logit((char *)mysql_error(db));
		exit(1);
	}
	db_disconnect();

	snprintf(query,255,"[KILL] DISCONNECT sock #%d",sd);

	for (i = 0; i < game_info.num_players; i++) {
		if (sd == game_info.players[i].sd) {
			game_info.players[i].sd = DISCONNECTED;
			snprintf(query,255,"[KILL] DISCONNECT %s",game_info.players[i].username);
		}
	}

	for (i = 0; i < 128 && observers[i]; i++) {
		if (sd == observers[i]) {
			/* remove a disconnected observer */
			observers[i] = 0;

			/* shuffle down the rest of the observers*/
			for (i = i; i < 127 && observers[i+1]; i++)
				observers[i] = observers[i+1];

			break; /* get out of the for loop */
		}
	}

	logit(query);
	shutdown(sd,SHUT_RDWR);
	close(sd);
}


/**
 *  void db_inc_games_played()
 *
 *  increments the number of games played
 */
void db_inc_games_played() {
	char query[512];
	bzero(query,512);

	snprintf(query,511,"UPDATE config SET gameid = gameid + 1");

	db_connect();
	if (mysql_real_query(db,query,(unsigned int) strlen(query))) {
		logit((char *)mysql_error(db));
		exit(1);
	}
	db_disconnect();

}

/**
 *  void db_inc_wins(char*)
 *
 *  increments the number of games played
 */
void db_inc_wins(char *username) {
	char query[512];
	bzero(query,512);

	snprintf(query,511,"UPDATE users SET wins = wins + 1 WHERE username like '%s'",username);

	db_connect();
	if (mysql_real_query(db,query,(unsigned int) strlen(query))) {
		logit((char *)mysql_error(db));
		exit(1);
	}
	db_disconnect();

}

/**
 *  void db_inc_ties(char*)
 *
 *  increments the number of ties
 */
void db_inc_ties(char *username) {
	char query[512];
	bzero(query,512);

	snprintf(query,511,"UPDATE users SET ties = ties + 1 WHERE username like '%s'",username);

	db_connect();
	if (mysql_real_query(db,query,(unsigned int) strlen(query))) {
		logit((char *)mysql_error(db));
		exit(1);
	}
	db_disconnect();

}

/**
 *  void db_inc_loses(char*)
 *
 *  increments the number of loses
 */
void db_inc_loses(char *username) {
	char query[512];
	bzero(query,512);

	snprintf(query,511,"UPDATE users SET loses = loses + 1 WHERE username like '%s'",username);

	db_connect();
	if (mysql_real_query(db,query,(unsigned int) strlen(query))) {
		logit((char *)mysql_error(db));
		exit(1);
	}
	db_disconnect();

}

/**
 *  void db_inc_folds(char*)
 *
 *  increments the number of folds
 */
void db_inc_folds(char *username) {
	char query[512];
	bzero(query,512);

	snprintf(query,511,"UPDATE users SET folds = folds + 1 WHERE username like '%s'",username);

	db_connect();
	if (mysql_real_query(db,query,(unsigned int) strlen(query))) {
		logit((char *)mysql_error(db));
		exit(1);
	}
	db_disconnect();

}

/**
 *  void db_inc_user_games_played(char*)
 *
 *  increments the number of games played
 */
void db_inc_user_games_played(char *username) {
	char query[512];
	bzero(query,512);

	snprintf(query,511,"UPDATE users SET games_played = games_played + 1 WHERE username like '%s'",username);

	db_connect();
	if (mysql_real_query(db,query,(unsigned int) strlen(query))) {
		logit((char *)mysql_error(db));
		exit(1);
	}
	db_disconnect();

}

/**
 *  int db_get_games_played()
 *
 *  returns the gameid number
 */
int db_get_games_played() {
	MYSQL_ROW row;
	MYSQL_RES *res;
	int x = 0;

	char query[256];
	bzero(query,256);

	snprintf(query,255,"SELECT gameid FROM config");

	db_connect();		
	if (mysql_real_query(db,query,(unsigned int) strlen(query))) {
		logit((char *)mysql_error(db));
		exit(1);
	}

	res = mysql_store_result(db);
	db_disconnect();

	if (mysql_num_rows(res)) {
		row = mysql_fetch_row(res);
		x = atoi(row[0]);
	}

	mysql_free_result(res);
	return x;
}

/**
 *  void db_players_kill()
 *
 *  set players sd to 0 in the database 
 */
void db_players_kill() {
	char query[256];
	bzero(query,256);

	snprintf(query,255,"UPDATE users SET sd = 0");
	db_connect();
	if (mysql_real_query(db,query,(unsigned int) strlen(query))) {
		logit((char *)mysql_error(db));
		exit(1);
	}
	db_disconnect();
}

/**
 *  void db_disconnect()
 *
 *  close the connection to a MySQL Database
 */
void db_disconnect() {
	mysql_close(db);
	free(db);
	logit("[DBDB] Disconnected");
	pthread_mutex_unlock(&db_lock);
}
