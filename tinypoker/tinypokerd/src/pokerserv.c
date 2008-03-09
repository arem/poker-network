/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This file is part of tinypokerd.
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

#include <gnutls/gnutls.h>
#include <libdaemon/dlog.h>
#include <pthread.h>
#include <signal.h>
#include <tinypoker.h>

#include "config.h"
#include "poker.h"
#include "signal.h"

static void client_connect_callback(ipp_socket * sock)
{
	ipp_message *msg;
	int rc;
	unsigned long a, b, c, d;

	/* We must reimplement inet_ntoa() here because it is not thread safe :| */

	a = (sock->addr.sin_addr.s_addr >> 0) & 0xff;
	b = (sock->addr.sin_addr.s_addr >> 8) & 0xff;
	c = (sock->addr.sin_addr.s_addr >> 16) & 0xff;
	d = (sock->addr.sin_addr.s_addr >> 24) & 0xff;

	/* TODO this might not be byte safe -- test on big-endian */

	daemon_log(LOG_INFO, "[SERV] Connect %lu.%lu.%lu.%lu", a, b, c, d);

	msg = ipp_new_message();
	msg->type = MSG_IPP;
	msg->payload = strdup("IPP 2.0 TinyPoker/0.0.0");

	rc = ipp_send_msg(sock, msg, SERVER_WRITE_TIMEOUT);
	if (rc == FALSE) {
		daemon_log(LOG_ERR, "[SERV] Could not send IPP message to %lu.%lu.%lu.%lu", a, b, c, d);
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		ipp_free_message(msg);
		sock = NULL;
		return;
	} else {
		daemon_log(LOG_INFO, "[SERV] |%s| sent to %lu.%lu.%lu.%lu", msg->payload, a, b, c, d);
	}

	ipp_free_message(msg);
	msg = NULL;

	msg = ipp_read_msg(sock, SERVER_READ_TIMEOUT);
	if (msg == NULL) {
		daemon_log(LOG_ERR, "[SERV] Could not read BUYIN message from %lu.%lu.%lu.%lu", a, b, c, d);
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
		ipp_free_message(msg);
		msg = NULL;
		return;
	} else {
		daemon_log(LOG_INFO, "[SERV] Read |%s| from %lu.%lu.%lu.%lu", msg->payload, a, b, c, d);
	}

	ipp_free_message(msg);
	msg = NULL;

	msg = ipp_new_message();
	msg->type = MSG_WELCOME;
	msg->payload = strdup("WELCOME anonymous");

	rc = ipp_send_msg(sock, msg, SERVER_WRITE_TIMEOUT);
	if (rc == FALSE) {
		daemon_log(LOG_ERR, "[SERV] Could not send WELCOME message to %lu.%lu.%lu.%lu", a, b, c, d);
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
		ipp_free_message(msg);
		msg = NULL;
		return;
	} else {
		daemon_log(LOG_INFO, "[SERV] |%s| sent to %lu.%lu.%lu.%lu", msg->payload, a, b, c, d);
	}

	ipp_free_message(msg);
	msg = NULL;

	daemon_log(LOG_INFO, "[SERV] Handshake Complete");

	/* TODO Add client socket to internal data structure here */
}

int pokerserv()
{
	pthread_t dealer_thread;
	pthread_attr_t dealer_thread_attr;

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

	/* Start listening for connections */
	ipp_servloop(port, client_connect_callback);

	if (!exit_now) {
		raise(SIGQUIT);
	}

	return 0;
}
