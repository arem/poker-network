/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This file is part of libtinypoker.
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

#define _GNU_SOURCE

#include <regex.h>
#include <ctype.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include <gnutls/gnutls.h>
#include <gcrypt.h>
#include <errno.h>
#include <pthread.h>

GCRY_THREAD_OPTION_PTHREAD_IMPL;

#include "tinypoker.h"

/**
 * Initializes underlying libraries. This function *must* be called first!
 */
void ipp_init()
{
	gcry_control(GCRYCTL_SET_THREAD_CBS, &gcry_threads_pthread);
	gnutls_global_init();
}

/**
 * De-initializes underlying libraries. This function *must* be called last!
 */
void ipp_exit()
{
	gnutls_global_deinit();
}

/**
 * Convert the string to upper case.
 * Convert multiple spaces to 1 space.
 * Trim leading and trailing white space.
 * Should be called before ipp_validate_msg()
 * @param msg the message, a null terminated string, to transform.
 */
void ipp_normalize_msg(char *msg)
{
	int len, i, j;
	len = strlen(msg);
	char *pos;

	if (!msg || strlen(msg) < MIN_MSG_SIZE) {
		return;
	}

	while ((pos = strchr(msg, '\n'))) {
		*pos = ' ';	/* trim all new lines */
	}

	j = 0;			/* Trim leading white space */
	while ((msg[j] == ' ' || msg[j] == '\t') && (j < len)) {
		j++;
	}

	/* Trim whitespace as we go. Convert everything to upper case. */
	for (i = 0; j < len && i < len; i++, j++) {
		msg[i] = toupper(msg[j]);
		if ((msg[i] == ' ' || msg[i] == '\t') && (msg[j + 1] == ' ' || msg[j + 1] == '\t')) {
			i--;
		}
	}

	/* Trim that last whitespace character not caught by the 'for' loop */
	if (i > 0 && (msg[i - 1] == ' ' || msg[i - 1] == '\t')) {
		i--;
	}

	msg[i] = '\0';
}


/**
 * Validates IPP Messages
 * @param regex one of the REGEX constants.
 * @param msg a message.
 * @return 1 if msg is valid, 0 if msg is not valid.
 */
int ipp_validate_msg(char *regex, char *msg)
{
	regex_t preg;
	int ret;

	if (!regex || !msg) {
		return FALSE;
	}

	ret = regcomp(&preg, regex, REG_EXTENDED);
	if (ret) {		/* compile the pattern */
		return FALSE;
	}

	/* See if the message matches */
	ret = regexec(&preg, msg, 0, 0, 0);
	regfree(&preg);		/* Clean up */

	if (!ret) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
 * Validates an arbitrary IPP Messages.
 * @param msg a message.
 * @return 1 if msg is valid, 0 if msg is not valid.
 */
int ipp_validate_unknown_msg(char *msg)
{
	unsigned int i;
	int is_valid = FALSE;

	char *regex[] = {
		REGEX_MSG_IPP,
		REGEX_MSG_BUYIN,
		REGEX_MSG_WELCOME,
		REGEX_MSG_NEWGAME,
		REGEX_MSG_PLAYER,
		NULL
	};

	if (!regex || !msg) {
		return FALSE;
	}

	for (i = 0; regex[i]; i++) {
		if (ipp_validate_msg(regex[i], msg)) {
			is_valid = TRUE;
			break;
		}
	}

	return is_valid;
}

/**
 * Allocates an empty ipp_socket. Don't forget to free().
 * @return a malloc()'d ipp_socket structure.
 */
ipp_socket *ipp_new_socket()
{
	ipp_socket *sock;
	sock = (ipp_socket *) malloc(sizeof(ipp_socket));
	if (!sock) {
		return NULL;
	}

	memset(sock, '\0', sizeof(ipp_socket));
	return sock;
}

/**
 * Connect to a server.
 * @param hostname the hostname of the server to connect to (example: host.domain.tld).
 * @param port the port number (example: 9999).
 * @return a socket or NULL if an error happened.
 */
ipp_socket *ipp_connect(char *hostname, int port)
{
	ipp_socket *sock;
	int ret;
	const int kx_prio[] = { GNUTLS_KX_ANON_DH, 0 };
	struct sockaddr_in sin;
	struct hostent *he;

	/* TinyPoker -- create an empty socket structure */
	sock = ipp_new_socket();

	/* GNU TLS -- initialize the structure */
	gnutls_anon_allocate_client_credentials(&(sock->anoncred));
	gnutls_init(&(sock->session), GNUTLS_CLIENT);
	gnutls_set_default_priority(sock->session);
	gnutls_kx_set_priority(sock->session, kx_prio);
	gnutls_credentials_set(sock->session, GNUTLS_CRD_ANON, sock->anoncred);

	/* TCP -- resolve the server's hostname, create a socket descriptor and connect */
	memset(&sin, 0, sizeof(sin));
	sin.sin_port = htons((short) port);
	he = gethostbyname(hostname);
	if (!ret) {
		ipp_disconnect(sock);
		free(sock);
		return NULL;
	}

	sin.sin_family = he->h_addrtype;
	memcpy(&sin.sin_addr, he->h_addr, he->h_length);
	sock->sd = socket(AF_INET, SOCK_STREAM, 0);
	if (!(sock->sd)) {
		ipp_disconnect(sock);
		free(sock);
		return NULL;
	}

	ret = connect(sock->sd, (struct sockaddr *) &sin, sizeof(sin));
	if (ret) {
		ipp_disconnect(sock);
		free(sock);
		return NULL;
	}

	/* GNU TLS -- handshaking */
	gnutls_transport_set_ptr(sock->session, (gnutls_transport_ptr_t) sock->sd);
	ret = gnutls_handshake(sock->session);
	if (ret < 0) {
		gnutls_perror(ret);
		ipp_disconnect(sock);
		free(sock);
		return NULL;
	}

	return sock;
}

/**
 * Disconnect from the server.
 * @param sock a socket to disconnect.
 */
void ipp_disconnect(ipp_socket * sock)
{
	if (sock->session) {
		gnutls_bye(sock->session, GNUTLS_SHUT_RDWR);
	}

	if (sock->sd) {
		/* close the connection */
		shutdown(sock->sd, SHUT_RDWR);
		close(sock->sd);
	}

	if (sock->session) {
		/* free session data */
		gnutls_deinit(sock->session);
	}

	if (sock->anoncred) {
		/* free credentials */
		gnutls_anon_free_client_credentials(sock->anoncred);
	}

	/* set to NULL to prevent double free() and other misuse */
	memset(sock, '\0', sizeof(ipp_socket));
}

/**
 * INTERNAL FUNCTION. DO NOT USE OUTSIDE LIBTINYPOKER!!!
 * @param void_params a __ipp_readln_thread_params structure.
 */
void __ipp_readln_thread(void *void_params)
{
	int ret;
	__ipp_readln_thread_params *params;
	params = (__ipp_readln_thread_params *) void_params;

	*(params->buffer) = (char *) malloc(sizeof(char) * (MAX_MSG_SIZE + 1));
	if (*(params->buffer)) {
		memset(*(params->buffer), '\0', (sizeof(char) * (MAX_MSG_SIZE + 1)));

		do {
			ret = gnutls_record_recv(params->sock->session, *(params->buffer), MAX_MSG_SIZE);
		} while (ret == GNUTLS_E_INTERRUPTED || ret == GNUTLS_E_AGAIN);

		*(params->n) = ret;

		if (!(*(params->n))) {
			*(params->n) = -1;
		}
	}

	pthread_exit(0);
}

/**
 * Read a message from the socket.
 * @param sock the socket to read from.
 * @param timeout number of seconds to wait for input.
 * @return a valid normalized message or NULL if message is invalid. All messages need to be deallocate by the user with free().
 */
char *ipp_read_msg(ipp_socket * sock, int timeout)
{
	__ipp_readln_thread_params params;
	char *buffer;
	int n, is_valid, ret;
	pthread_t reader;
	pthread_attr_t reader_attr;
	time_t clock;

	is_valid = FALSE;
	buffer = NULL;
	n = 0;

	params.sock = sock;
	params.buffer = &buffer;
	params.n = &n;

	pthread_attr_init(&reader_attr);
	pthread_attr_setdetachstate(&reader_attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&reader, &reader_attr, (void *(*)(void *)) __ipp_readln_thread, (void *) &params);
	if (ret != 0) {
		pthread_attr_destroy(&reader_attr);
		return NULL;
	}

	clock = time(NULL);
	do {
		if ((time(NULL) - clock) > timeout) {
			break;
		}
		pthread_yield();
	} while (!n);

	pthread_cancel(reader);
	pthread_attr_destroy(&reader_attr);

	buffer = *(params.buffer);

	if (n < 0) {
		free(buffer);
		return NULL;
	} else if (!n) {
		return NULL;
	}

	ipp_normalize_msg(buffer);

	is_valid = ipp_validate_unknown_msg(buffer);
	if (is_valid) {
		return buffer;
	} else {
		free(buffer);
		return NULL;
	}
}

/**
 * INTERNAL FUNCTION. DO NOT USE OUTSIDE LIBTINYPOKER!!!
 * @param void_params a __ipp_writeln_thread_params structure.
 */
void __ipp_writeln_thread(void *void_params)
{
	int ret;
	__ipp_writeln_thread_params *params;
	params = (__ipp_writeln_thread_params *) void_params;

	do {
		ret = gnutls_record_send(params->sock->session, params->buffer, strlen(params->buffer));
	} while (ret == GNUTLS_E_INTERRUPTED || ret == GNUTLS_E_AGAIN);

	*(params->n) = ret;
	pthread_exit(0);
}

/**
 * Send a message to the socket. It will be normalized and validated by this function before sending.
 * @param sock the socket to read from.
 * @param msg the message to send.
 * @param timeout number of seconds to wait for output.
 * @return TRUE if msg was sent OK, else FALSE for error.
 */
int ipp_send_msg(ipp_socket * sock, char *msg, int timeout)
{
	__ipp_writeln_thread_params params;
	int is_valid, ret, n;
	pthread_t writer;
	pthread_attr_t writer_attr;
	time_t clock;

	is_valid = FALSE;
	n = 0;

	ipp_normalize_msg(msg);
	is_valid = ipp_validate_unknown_msg(msg);
	if (is_valid) {
		char *final_msg;

		final_msg = strndup(msg, strlen(msg) + 1);
		final_msg[strlen(msg)] = '\n';

		params.sock = sock;
		params.buffer = msg;
		params.n = &n;

		pthread_attr_init(&writer_attr);
		pthread_attr_setdetachstate(&writer_attr, PTHREAD_CREATE_DETACHED);

		ret = pthread_create(&writer, &writer_attr, (void *(*)(void *)) __ipp_writeln_thread, (void *) &params);
		if (ret != 0) {
			pthread_attr_destroy(&writer_attr);
			return FALSE;
		}

		clock = time(NULL);
		do {
			if ((time(NULL) - clock) > timeout) {
				break;
			}
			pthread_yield();
		} while (!n);

		pthread_cancel(writer);
		pthread_attr_destroy(&writer_attr);

		if (n == strlen(final_msg)) {
			free(final_msg);
			final_msg = NULL;
			return TRUE;
		} else {
			free(final_msg);
			final_msg = NULL;
			return FALSE;
		}
	} else {
		return FALSE;
	}
}

/**
 * servloop exit Indicator
 * 1 for exit now, 0 for continue.
 */
static int done;

/**
 * Set done to 1 when SIGUSR2 is raised.
 * @param sig signal
 */
void __ipp_handle_sigusr2(int sig)
{
	if (sig == SIGUSR2) {
		done = 1;
	}
}

/**
 * Main server loop. This function sets up the networking and accepts
 * incoming connections. For every incoming client, a 'callback' is
 * called. The server blocks and waits for 'callback' to return, so
 * make 'callback' short and sweet.
 * @param port TCP/IP port to listen on.
 * @param callback function to call when a new client connects.
 */
void ipp_servloop(int port, void (*callback) (ipp_socket *))
{
	int master, slave, rc;
	ipp_socket *ipp_slave;

	struct pollfd p;
	struct sockaddr_in sin;
	struct sockaddr_in client_addr;
	unsigned int client_addr_len;
	const int kx_prio[] = { GNUTLS_KX_ANON_DH, 0 };
	gnutls_session_t session;
	gnutls_dh_params_t dh_params;
	gnutls_anon_server_credentials_t anoncred;

	gnutls_anon_allocate_server_credentials(&anoncred);
	gnutls_dh_params_init(&dh_params);
	gnutls_dh_params_generate2(dh_params, 1024);
	gnutls_anon_set_server_dh_params(anoncred, dh_params);

	client_addr_len = sizeof(client_addr);
	memset(&sin, 0, sizeof(sin));

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = (unsigned short) htons(port);

	master = socket(PF_INET, SOCK_STREAM, 0);
	if (master < 0) {
		gnutls_anon_free_server_credentials(anoncred);
		gnutls_dh_params_deinit(dh_params);
		return;
	}

	rc = bind(master, (struct sockaddr *) &sin, sizeof(sin));
	if (rc < 0) {
		gnutls_anon_free_server_credentials(anoncred);
		gnutls_dh_params_deinit(dh_params);
		return;
	}

	rc = listen(master, 64);
	if (rc < 0) {
		gnutls_anon_free_server_credentials(anoncred);
		gnutls_dh_params_deinit(dh_params);
		return;
	}

	p.fd = master;
	p.events = POLLIN;
	p.revents = 0;

	done = 0;
	signal(SIGUSR2, __ipp_handle_sigusr2);

	while (!done) {
		/* Poll master so that we don't block on accept */
		/* this is done so that when we signal we re-evaluate if !done == true */

		poll(&p, 1, 30000);	/* 30 second timeout */
		if (p.revents != POLLIN) {	/* no activity */
			if (done) {
				break;
			} else {
				continue;
			}
		}

		if (done)
			break;

		slave = accept(master, (struct sockaddr *) &client_addr, &client_addr_len);
		if (slave < 0) {
			if (errno == EINTR) {
				continue;
			} else {
				shutdown(master, SHUT_RDWR);
				close(master);
				gnutls_anon_free_server_credentials(anoncred);
				gnutls_dh_params_deinit(dh_params);
				return;
			}
		}

		gnutls_init(&session, GNUTLS_SERVER);
		gnutls_set_default_priority(session);
		gnutls_kx_set_priority(session, kx_prio);
		gnutls_credentials_set(session, GNUTLS_CRD_ANON, anoncred);
		gnutls_dh_set_prime_bits(session, 1024);
		gnutls_transport_set_ptr(session, (gnutls_transport_ptr_t) slave);

		rc = gnutls_handshake(session);
		if (rc < 0) {
			shutdown(slave, SHUT_RDWR);
			close(slave);
			gnutls_deinit(session);
			continue;
		}

		ipp_slave = ipp_new_socket();
		if (!ipp_slave) {
			shutdown(slave, SHUT_RDWR);
			close(slave);
			gnutls_deinit(session);
			continue;
		}

		ipp_slave->sd = slave;
		ipp_slave->session = session;
		ipp_slave->anoncred = NULL;
		memcpy(&(ipp_slave->addr), &(client_addr), sizeof(struct sockaddr_in));

		callback(ipp_slave);
	}

	shutdown(master, SHUT_RDWR);
	close(master);
	gnutls_anon_free_server_credentials(anoncred);
	gnutls_dh_params_deinit(dh_params);
}
