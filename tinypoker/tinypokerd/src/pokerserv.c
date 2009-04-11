/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <tcort@tomcort.com>
 *
 * This file is part of TinyPoker.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE

#include <glib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <signal.h>
#include <tinypoker.h>

#include "monitor.h"
#include "log.h"
#include "poker.h"
#include "signal.h"
#include "tinypokerd.h"

int ipp_auth(char *name)
{
	g_debug("[PSRV] Authenticating '%s'", name);
	return TRUE;
}

/**
 * Handle incoming connections.
 * @param sock client socket.
 */
static void client_connect_callback(ipp_socket * sock)
{
	ipp_player *p;
	int rc;

	g_debug("[PSRV] client connected");

	p = ipp_server_handshake(sock, TINYPOKERD_NAME "/" TINYPOKERD_VERSION, ipp_auth, protocol_logger);
	if (!p) {
		g_debug("[PSRV] handshake fail");
		return;
	}

	g_debug("[PSRV] handshake OK");

	g_mutex_lock(tbl->lock);
	rc = ipp_add_player(tbl, p);
	g_mutex_unlock(tbl->lock);
	if (rc == -1) {
		g_debug("[PSRV] add player failed -- table full");

		/* TODO send table full error message */
		ipp_free_socket(sock);
		sock = NULL;
		return;
	}

	g_debug("[PSRV] player added to table");
}

int pokerserv(void)
{
	GError *error;
	GThread *dealer_thread;

	tbl = ipp_new_table();
	if (tbl == NULL) {
		exit_now = 1;
		raise(SIGINT);
		return 0;
	}

	/* create a thread to play the game */
	monitor_inc();

	error = NULL;
	dealer_thread = NULL;
	dealer_thread = g_thread_create(play, NULL, TRUE, &error);
	if (dealer_thread == NULL) {
		if (error) {
			fprintf(stderr, "Error creating thread: %s\n", error->message);
			g_error_free(error);
		}
		monitor_dec();
		raise(SIGINT);
		return -1;
	}

	/* Start listening for connections */
	ipp_servloop(TINYPOKER_PORT, client_connect_callback);

	g_thread_join(dealer_thread);

	if (!exit_now) {
		raise(SIGINT);
	}
	return 0;
}
