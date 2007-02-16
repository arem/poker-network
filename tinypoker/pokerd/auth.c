/*
 * Copyright (C) 2005, 2006 Thomas Cort
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
#include "log.h"
#include "monitor.h"

#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int snprintf(char *str, size_t size, const char *format, ...);

void authenticate(void *void_sd) {
	int type, sd;
	char *username, *password;
	struct byte_array *ba;
	char logstr[256];

	sd = *((int *)void_sd);
	free(void_sd);

	bzero(logstr,256);

	ba = read_message(sd,&type);

	if (!ba) {
		/* no need to destroy ba; it is NULL */
		monitor_dec();
		return;
	}

	username = byte_array_read_string(ba);
	password = byte_array_read_string(ba);
	byte_array_destroy(ba);

	ba = new_byte_array(1);

	/* we have this lock so that the game thread doesn't get confused when 
  	   it tries to select players from the database. This also blocks multiple
	   login attempts from the same user  */
	pthread_mutex_lock(&auth_lock);

	if (!db_auth(sd,username,password)) {
		/* don't worry if write_message fails   */
		/* we are closing the connection anyway */
		write_message(sd,BADPASS,ba);
		snprintf(logstr,255,"[SEND] BADPASS %s",username);
		shutdown(sd, SHUT_RDWR);
		close(sd);
	} else {
		write_message(sd,GOODPASS,ba);
		snprintf(logstr,255,"[SEND] GOODPASS %s",username);
	}

	byte_array_destroy(ba);

	logit(logstr);

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
