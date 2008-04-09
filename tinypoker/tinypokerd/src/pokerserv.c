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

#include <netdb.h>
#include <sys/socket.h>
#include <gnutls/gnutls.h>
#include <libdaemon/dlog.h>
#include <pthread.h>
#include <signal.h>
#include <tinypoker.h>

#include "config.h"
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
	/*
	 * Note: this function causes the server loop to block. Don't do
	 * anything too fancy or time intensive in here.
	 */

	ipp_message *msg;
	char *user, *pass;
	int rc;
	char ip[64];

	memset(ip, '\0', sizeof(char) * 64);
	rc = getnameinfo((struct sockaddr *) &(sock->sockaddr), sock->sockaddrlen, ip, sizeof(char) * 64, NULL, 0, NI_NUMERICHOST);
	if (rc != 0) {
		/* something crazy happened where the sockaddr didn't contain a valid address */
		/* this should never happen, but just in case, we'll handle it */
		snprintf(ip, 60, "(unknown-ip)");
	}

	daemon_log(LOG_INFO, "[SERV] [CONN] %s", ip);

	msg = ipp_new_message();
	msg->type = MSG_IPP;
	msg->payload = strdup("IPP 2.0 " TINYPOKERD_NAME "/" TINYPOKERD_VERSION);

	rc = ipp_send_msg(sock, msg, SERVER_WRITE_TIMEOUT);
	if (rc == FALSE) {
		daemon_log(LOG_ERR, "[SERV] Could not send IPP message to %s", ip);
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		ipp_free_message(msg);
		sock = NULL;
		return;
	} else {
		daemon_log(LOG_INFO, "[SERV] [SEND] |%s| to %s", msg->payload, ip);
	}

	ipp_free_message(msg);
	msg = NULL;

	msg = ipp_read_msg(sock, SERVER_READ_TIMEOUT);
	if (msg == NULL || msg->type != MSG_BUYIN || msg->payload == NULL) {
		daemon_log(LOG_ERR, "[SERV] Could not read BUYIN message from %s", ip);
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
		ipp_free_message(msg);
		msg = NULL;
		return;
	} else {
		daemon_log(LOG_INFO, "[SERV] [RECV] {BUYIN} from %s", ip);
	}

	user = strdup(msg->parsed[1]);
	ipp_free_message(msg);
	msg = NULL;

	pass = strchr(user, ':');
	if (pass == NULL) {
		pass = strdup("");
		if (pass == NULL) {
			daemon_log(LOG_ERR, "[SERV] malloc failed");
			ipp_disconnect(sock);
			ipp_free_socket(sock);
			sock = NULL;
			ipp_free_message(msg);
			msg = NULL;
			free(user);
			user = NULL;
			return;
		}
	} else {
		*pass = '\0';
		pass = strdup(pass + sizeof(char));
		if (pass == NULL) {
			daemon_log(LOG_ERR, "[SERV] malloc failed");
			ipp_disconnect(sock);
			ipp_free_socket(sock);
			sock = NULL;
			ipp_free_message(msg);
			msg = NULL;
			free(user);
			user = NULL;
			return;
		}
	}

	daemon_log(LOG_INFO, "[SERV] %s is |%s|", ip, user);

	/* daemon_log(LOG_INFO, "[SERV] '%s' '%s'", user, pass); */

	rc = ipp_auth(user, pass);
	memset(pass, 'X', strlen(pass));
	free(pass);
	pass = NULL;

	if (rc == TRUE) {
		daemon_log(LOG_INFO, "[SERV] Auth OK");
	} else {
		daemon_log(LOG_INFO, "[SERV] Auth Failed");
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
		ipp_free_message(msg);
		msg = NULL;
		free(user);
		user = NULL;
		return;
	}

	msg = ipp_new_message();
	msg->type = MSG_WELCOME;
	msg->payload = (char *) malloc(sizeof(char) * (strlen("WELCOME ") + strlen(user) + 2));
	if (!(msg->payload)) {
		daemon_log(LOG_ERR, "[SERV] malloc failed");
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
		ipp_free_message(msg);
		msg = NULL;
		free(user);
		user = NULL;
		return;
	}
	memset(msg->payload, '\0', (sizeof(char) * (strlen("WELCOME ") + strlen(user) + 2)));
	snprintf(msg->payload, (sizeof(char) * (strlen("WELCOME ") + strlen(user) + 1)), "%s%s", "WELCOME ", user);

	rc = ipp_send_msg(sock, msg, SERVER_WRITE_TIMEOUT);
	if (rc == FALSE) {
		daemon_log(LOG_ERR, "[SERV] Could not send WELCOME message to %s", ip);
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
		ipp_free_message(msg);
		msg = NULL;
		free(user);
		user = NULL;
		return;
	} else {
		daemon_log(LOG_INFO, "[SERV] [SEND] |%s| to %s", msg->payload, ip);
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

int pokerserv()
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
	daemon_log(LOG_INFO, "[SERV] Dealer Thread Started");

	/* Start listening for connections */
	ipp_servloop(client_connect_callback, x509_ca, x509_crl, x509_cert, x509_key);

	daemon_log(LOG_INFO, "[SERV] Server Loop Exited");

	if (!exit_now) {
		raise(SIGQUIT);
	}
	return 0;
}
