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

#include "auth.h"
#include "byte.h"
#include "net.h"
#include "db.h"
#include "poker.h"
#include "monitor.h"

#include <libdaemon/dlog.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int snprintf(char *str, size_t size, const char *format, ...);

/**
 * Attempts to authenticate a user connected to the socket void_sd.
 * If a read from void_sd doesn't yield a message of type JOIN_GAME or
 * the authentication fails, the user is disconnected. If everything is
 * ok, then the user stays connected and joins the games.
 * @param void_sd the socket descriptor of the current player.
 */
void authenticate(void *void_sd) {
	int type, sd;
	char *username, *password;
	struct byte_array *ba;

	sd = *((int *)void_sd);
	free(void_sd);

	ba = read_message(sd,&type);

	if (!ba || type != JOIN_GAME) {
		if (ba) {
			byte_array_destroy(ba);
			ba = NULL;
		}

		/* don't worry if write_message fails   */
		/* we are closing the connection anyway */
		ba = new_byte_array(1);
		write_message(sd,BADPASS,ba);
		daemon_log(LOG_WARNING, "[SEND] BADPASS %s",username);
		shutdown(sd, SHUT_RDWR); /* Disconnect the user */
		close(sd);

		monitor_dec();
		pthread_exit(NULL);
		return; /* we should never get here */
	}

	username = byte_array_read_string(ba);
	password = byte_array_read_string(ba);
	byte_array_destroy(ba);

	ba = new_byte_array(1);

	/* we have this lock so that the game thread doesn't get confused when 
  	   it tries to select players from the database. This also blocks multiple
	   concurrent login attempts from the same user */
	pthread_mutex_lock(&auth_lock);

	if (!db_auth(sd,username,password)) {
		/* don't worry if write_message fails   */
		/* we are closing the connection anyway */
		write_message(sd,BADPASS,ba);
		daemon_log(LOG_WARNING,"[SEND] BADPASS %s",username);
		shutdown(sd, SHUT_RDWR);
		close(sd);
	} else {
		write_message(sd,GOODPASS,ba);
		daemon_log(LOG_INFO,"[SEND] GOODPASS %s",username);
	}

	byte_array_destroy(ba);

	free(username);
	free(password);

	/* Don't unlock until after we send info msg so that poker playing thread
	   doesn't write to the socket while we are writing the info msg */
	pthread_mutex_unlock(&auth_lock);

	/* thread ends here              */
	/* another thread runs the games */
	/* we just auth and add players  */
	monitor_dec();
	pthread_exit(NULL);
}
