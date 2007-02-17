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

#include "poker.h"
#include "db.h"
#include "log.h"
#include "net.h"
#include "byte.h"

#include <strings.h>

extern int snprintf(char *str, size_t size, const char *format, ...);

/**
 *  int passivesocket(int port)
 *
 *  creates a socket, binds it port "port" and starts listening for connections
 */
int passivesocket(int port) {
	struct sockaddr_in sin;
	int sd;

	memset(&sin, 0, sizeof(sin));

	sin.sin_family      = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port        = (unsigned short)htons(port);

	if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		char buf[128];
		bzero(buf,128);
		snprintf(buf,127,"[SOCK] %s",strerror(errno));
		logit(buf);
		exit(1);
	}
    
	if (bind(sd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		char buf[128];
		bzero(buf,128);
		snprintf(buf,127,"[SOCK] %s",strerror(errno));
		logit(buf);
		exit(1);
	}

	if (listen(sd, 64) < 0) {
		char buf[128];
		bzero(buf,128);
		snprintf(buf,127,"[SOCK] %s",strerror(errno));
		logit(buf);
		exit(1);
    	}

	return sd;
}

/**
 *  struct byte_array *read_message(int sd, int *type)
 *
 *  read a message and store it in a byte_array
 *  set type
 */
struct byte_array *read_message(int sd, int *type) {
	struct byte_array *ba;
	char *data;
	int i, size = 0;

	struct pollfd p;

	p.fd       = sd;
	p.events  = POLLIN;
	p.revents = 0;

	/* grab message type */
	for (i = 0; i < 4; i++) {
		poll(&p,1,15000); /* 15 second timeout */

		if (p.revents != POLLIN) {
			char buf[128];
			errno = ENODATA;
			db_player_kill(sd);
			bzero(buf,128);
			snprintf(buf,127,"[SOCK] %s",strerror(errno));
			logit(buf);
			return NULL;
		}


		if (recv(sd,((char *)(type))+i,1,0) < 0) {
			char buf[128];
			db_player_kill(sd);
			bzero(buf,128);
			snprintf(buf,127,"[SOCK] %s",strerror(errno));
			logit(buf);
			return NULL;
		}
	}

	*type = ntohl(*type);

	/* grab message size */
	for (i = 0; i < 4; i++) {
		poll(&p,1,10000); /* 10 second timeout */

		if (p.revents != POLLIN) {
			char buf[128];
			errno = ENODATA;
			db_player_kill(sd);
			bzero(buf,128);
			snprintf(buf,127,"[SOCK] %s",strerror(errno));
			logit(buf);
			return NULL;
		}

		if (recv(sd,((char *)(&size))+i,1,0) < 0) {
			char buf[128];
			db_player_kill(sd);
			bzero(buf,128);
			snprintf(buf,127,"[SOCK] %s",strerror(errno));
			logit(buf);
			return NULL;
		}
	}

	size = ntohl(size);

	/* limit messages to 8k */
	if (size > 8192) {
		logit("[ERRR] EXTREMELY LONG MSG DISCARDED");
		db_player_kill(sd);
		return NULL;
	}

	/* grab message data */
	if (size > 0) {
		if ((data = (char *) malloc(size)) == NULL) {
			char buf[128];
			bzero(buf,128);
			snprintf(buf,127,"[SOCK] %s",strerror(errno));
			logit(buf);
			exit(1);
		}

		for (i = 0; i < size; i++) {
			poll(&p,1,10000); /* 10 second timeout */

			if (p.revents != POLLIN) {
				char buf[128];
				errno = ENODATA;
				db_player_kill(sd);
				bzero(buf,128);
				snprintf(buf,127,"[SOCK] %s",strerror(errno));
				logit(buf);
				free(data);
				return NULL;
			}


			if (recv(sd,data+i,1,0) < 0) {
				char buf[128];
				db_player_kill(sd);
				bzero(buf,128);
				snprintf(buf,127,"[SOCK] %s",strerror(errno));
				logit(buf);
				free(data);
				return NULL;
			}
		}

		ba = new_byte_array(size);
		byte_array_append_bytes(ba,size,data);

		free(data);
		return ba;
	} else {
		return NULL;
	}
}

/**
 *  void write_message_all(int type, struct byte_array *ba)
 *
 *  send a message to all players in the game
 */
void write_message_all(int type, struct byte_array *ba) {
	int i;

	for (i = 0; i < game_info.num_players; i++) {
		if (game_info.players[i].folded != DISCONNECTED && game_info.players[i].folded != FD) {
			write_message(game_info.players[i].sd,type,ba);
		}
	}

	for (i = 0; i < 128 && observers[i]; i++)
		write_message(observers[i],type,ba);
}

/**
 *  int write_message(int sd, int type, struct byte_array *)
 *
 *  write a message to sd
 */
void write_message(int sd, int type, struct byte_array *ba) {
	int size;
	char *data;

	data = byte_array_get_bytes(ba);

	type = htonl(type);
	size = htonl(ba->size);

	if (send(sd, (char *)&type, 4, 0) < 0) {
		char buf[128];
		bzero(buf,128);
		db_player_kill(sd);
		snprintf(buf,127,"[SOCK] %s",strerror(errno));
		logit(buf);
		free(data);
		return;
	}

	if (send(sd, (char *)&size, 4, 0) < 0) {
		char buf[128];
		bzero(buf,128);
		db_player_kill(sd);
		snprintf(buf,127,"[SOCK] %s",strerror(errno));
		logit(buf);
		free(data);
		return;
	}

	if (ba->size > 0) {
		if (send(sd, data, ba->size, 0) < 0) {
			char buf[128];
			bzero(buf,128);
			db_player_kill(sd);
			snprintf(buf,127,"[SOCK] %s",strerror(errno));
			logit(buf);
			free(data);
			return;
		}
	}
	free(data);
}
