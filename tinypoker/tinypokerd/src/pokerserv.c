/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <linuxgeek@gmail.com>
 * 
 * This file is part of tinypokerd.
 * 
 * tinypokerd is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * tinypokerd is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * tinypokerd.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE

#include <netdb.h>
#include <sys/socket.h>
#include <gnutls/gnutls.h>
#include <libdaemon/dlog.h>
#include <pthread.h>
#include <signal.h>
#include <tinypoker.h>

#include "config.h"
#include "monitor.h"
#include "log.h"
#include "pam.h"
#include "poker.h"
#include "signal.h"
#include "tinypokerd.h"

/**
 * Handle incoming connections.
 * @param sock client socket.
 */
static void client_connect_callback(ipp_socket * sock)
{
	ipp_player *p;
	int rc;

	p = ipp_server_handshake(sock, TINYPOKERD_NAME "/" TINYPOKERD_VERSION, ipp_auth, protocol_logger);
	if (!p) {
		daemon_log(LOG_ERR, "[SERV] Handshake Failed");
		return;
	}

	g_mutex_lock(tbl->lock);
	rc = ipp_add_player(tbl, p);
	g_mutex_unlock(tbl->lock);
	if (rc == -1) {
		/* TODO send table full error message */
		daemon_log(LOG_INFO, "[SERV] Table Full");
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
		return;
	}

	daemon_log(LOG_INFO, "[SERV] Added '%s' to table.", p->name);
}

int pokerserv(void)
{
	pthread_t dealer_thread;
	pthread_attr_t dealer_thread_attr;

	tbl = ipp_new_table();
	if (tbl == NULL) {
		exit_now = 1;
		raise(SIGQUIT);
		return 0;
	}

	/* create a thread to play the game */
	pthread_attr_init(&dealer_thread_attr);
	pthread_attr_setdetachstate(&dealer_thread_attr, PTHREAD_CREATE_DETACHED);
	monitor_inc();
	if (pthread_create(&dealer_thread, &dealer_thread_attr, play, NULL) != 0) {
		daemon_log(LOG_ERR, "[SERV] Couldn't create dealer thread");
		monitor_dec();
		raise(SIGQUIT);
		return -1;
	}

	daemon_log(LOG_DEBUG, "[SERV] Dealer Thread Started");

	/* Start listening for connections */
	ipp_servloop(TINYPOKER_PORT, client_connect_callback, x509_ca, x509_crl, x509_cert, x509_key);

	daemon_log(LOG_DEBUG, "[SERV] Server Loop Exited");

	if (!exit_now) {
		raise(SIGQUIT);
	}
	return 0;
}
