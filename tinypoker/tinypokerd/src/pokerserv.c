/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
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

#include <gnutls/gnutls.h>
#include <libdaemon/dlog.h>
#include <pthread.h>
#include <signal.h>
#include <tinypoker.h>

#include "config.h"
#include "poker.h"
#include "signal.h"
#include "tinypokerd.h"

/**
 * Handle incoming connections.
 * @param sock client socket.
 */
static void
client_connect_callback(ipp_socket * sock)
{
	/*
	 * Note: this function causes the server loop to block. Don't do
	 * anything too fancy or time intensive in here.
	 */

	ipp_message    *msg;
	char           *user;
	int		rc;
	unsigned long	a, b, c, d;

	/*
	 * We must reimplement inet_ntoa() here because it is not thread safe
	 * :|
	 */
/* IPv4 */
/*
	a = (sock->addr.sin_addr.s_addr >> 0) & 0xff;
	b = (sock->addr.sin_addr.s_addr >> 8) & 0xff;
	c = (sock->addr.sin_addr.s_addr >> 16) & 0xff;
	d = (sock->addr.sin_addr.s_addr >> 24) & 0xff;
*/

	/* For testing */
	a = 0;
	b = 0;
	c = 0;
	d = 0;

	/* TODO this might not be byte safe -- test on big-endian */
	daemon_log(LOG_INFO, "[SERV] [CONN] %lu.%lu.%lu.%lu", a, b, c, d);

	msg = ipp_new_message();
	msg->type = MSG_IPP;
	msg->payload = strdup("IPP 2.0 " TINYPOKERD_NAME "/" TINYPOKERD_VERSION);

	rc = ipp_send_msg(sock, msg, SERVER_WRITE_TIMEOUT);
	if (rc == FALSE) {
		daemon_log(LOG_ERR, "[SERV] Could not send IPP message to %lu.%lu.%lu.%lu", a, b, c, d);
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		ipp_free_message(msg);
		sock = NULL;
		return;
	} else {
		daemon_log(LOG_INFO, "[SERV] [SEND] |%s| to %lu.%lu.%lu.%lu", msg->payload, a, b, c, d);
	}

	ipp_free_message(msg);
	msg = NULL;

	msg = ipp_read_msg(sock, SERVER_READ_TIMEOUT);
	if (msg == NULL || msg->type != MSG_BUYIN || msg->payload == NULL) {
		daemon_log(LOG_ERR, "[SERV] Could not read BUYIN message from %lu.%lu.%lu.%lu", a, b, c, d);
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
		ipp_free_message(msg);
		msg = NULL;
		return;
	} else {
		daemon_log(LOG_INFO, "[SERV] [RECV] |%s| from %lu.%lu.%lu.%lu", msg->payload, a, b, c, d);
	}

	user = strdup(msg->parsed[1]);
	daemon_log(LOG_INFO, "[SERV] %lu.%lu.%lu.%lu is |%s|", a, b, c, d, user);

	ipp_free_message(msg);
	msg = NULL;

	msg = ipp_new_message();
	msg->type = MSG_WELCOME;
	msg->payload = (char *)malloc(sizeof(char) * (strlen("WELCOME ") + strlen(user) + 2));
	if (!(msg->payload)) {
		daemon_log(LOG_ERR, "[SERV] malloc failed");
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
		ipp_free_message(msg);
		msg = NULL;
	}
	memset(msg->payload, '\0', (sizeof(char) * (strlen("WELCOME ") + strlen(user) + 2)));
	snprintf(msg->payload, (sizeof(char) * (strlen("WELCOME ") + strlen(user) + 1)), "%s%s", "WELCOME ", user);

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
		daemon_log(LOG_INFO, "[SERV] [SEND] |%s| to %lu.%lu.%lu.%lu", msg->payload, a, b, c, d);
	}

	ipp_free_message(msg);
	msg = NULL;

	daemon_log(LOG_INFO, "[SERV] Handshake Complete");

	/* TODO Add client socket to internal data structure here */
	free(user);
	user = NULL;

	/* TODO: remove this. it is just here for testing */
	ipp_disconnect(sock);
	ipp_free_socket(sock);
	sock = NULL;
}

int
pokerserv()
{
	pthread_t	dealer_thread;
	pthread_attr_t	dealer_thread_attr;

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
	ipp_servloop(port, client_connect_callback, "ca.pem", "crl.pem", "cert.pem", "key.pem");

	if (!exit_now) {
		raise(SIGQUIT);
	}
	return 0;
}
