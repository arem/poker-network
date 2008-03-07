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

#include "db.h"
#include "sig.h"
#include "net.h"
#include "byte.h"
#include "poker.h"
#include "pokerserv.h"
#include "monitor.h"

#include <libdaemon/dlog.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/socket.h>

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

/**
 * main function for the poker server
 *
 * continuously-running thread which serves poker clients
 */
int pokermain(int port) {
	int master, slave;
	int *slavep;

	pthread_t tt;
	pthread_attr_t ta;

	pthread_t tt2;
	pthread_attr_t ta2;

	struct pollfd p;
	struct sockaddr_in client_addr;
	unsigned int client_addr_len;

	pthread_attr_init(&ta);
	pthread_attr_setdetachstate(&ta,PTHREAD_CREATE_DETACHED);

	pthread_attr_init(&ta2);
	pthread_attr_setdetachstate(&ta2,PTHREAD_CREATE_DETACHED);

	pthread_mutex_init(&auth_lock,0);
	pthread_mutex_init(&db_lock,0);

	client_addr_len = sizeof(client_addr);

	if ((master = passivesocket(port)) < 0) {
		daemon_log(LOG_ERR,"[SOCK] %s",strerror(errno));
		exit(1);
	}

	p.fd      = master;
	p.events  = POLLIN;
	p.revents = 0;


	/* create a thread to play the game */
	monitor_inc();
	if (pthread_create(&tt2,&ta2,(void* (*) (void*))play, (void*)NULL) != 0) {
		daemon_log(LOG_ERR,"[THRD] %s",strerror(errno));
		monitor_dec();
		exit(1);
	}

	/* listen and accept new connections */
	while (!done) {

		/* Poll master so that we don't block on accept */
		/* this is done so that when we signal we re-evaluate if !done == true */

		poll(&p,1,30000);  /* 30 second timeout */
		if (p.revents != POLLIN) { /* no activity */
			if (done) break;
			else   continue;
		}

		if ((slave = accept(master, (struct sockaddr*)&client_addr, &client_addr_len)) < 0) {
			if (errno == EINTR) {
				continue;
			}

			daemon_log(LOG_ERR,"[SOCK] %s",strerror(errno));
			exit(1);
		}

		/* we need to do this instead of just casting the int to void* in pthread_create
		 * because on Alpha sizeof(int) = 4 and sizeof(void*) = 8 */
		 slavep = (int*) malloc(sizeof(int));
		*slavep = slave;

		monitor_inc();
		if (pthread_create(&tt,&ta,(void* (*) (void*))authenticate, (void*)slavep) != 0) {
			daemon_log(LOG_INFO,"[THRD] %s",strerror(errno));
			monitor_dec();
			exit(1);
		}

	}

	pthread_attr_destroy(&ta);
	pthread_attr_destroy(&ta2);

	shutdown(master,SHUT_RDWR);
	close(master);

	return 0;
}
