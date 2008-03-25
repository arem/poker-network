/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 * 
 * This file is part of libtinypoker.
 * 
 * libtinypoker is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * libtinypoker is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * libtinypoker.  If not, see <http://www.gnu.org/licenses/>.
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
void 
ipp_init()
{
	gcry_control(GCRYCTL_SET_THREAD_CBS, &gcry_threads_pthread);
	gcry_control(GCRYCTL_ENABLE_QUICK_RANDOM, 0);
	gnutls_global_init();
}

/**
 * De-initializes underlying libraries. This function *must* be called last!
 */
void 
ipp_exit()
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
void 
ipp_normalize_msg(char *msg)
{
	int		len       , start, end, i, j;
	len = strlen(msg);
	char           *pos;

	if (!msg || strlen(msg) < MIN_MSG_SIZE) {
		return;
	}
	while ((pos = strchr(msg, '\n'))) {
		*pos = ' ';	/* replace new lines with white space */
	}

	/* find start */
	for (start = 0; start < len && (msg[start] == ' ' || msg[start] == '\t'); start++) {
		 /* do nothing */ ;
	}

	/* find end */
	for (end = len - 1; end > 0 && (msg[end] == ' ' || msg[end] == '\t'); end--) {
		/* do nothing */
	}

	/* Trim whitespace as we go. Convert everything to upper case. */
	for (i = 0, j = start; i < len && j <= end; i++, j++) {
		msg[i] = toupper(msg[j]);

		/* Collapse white space */
		if ((msg[j] == ' ' || msg[j] == '\t') && (msg[j + 1] == ' ' || msg[j + 1] == '\t')) {
			i--;
		}
	}

	msg[i] = '\0';
}


/**
 * Validates IPP Messages
 * @param regex one of the REGEX constants.
 * @param msg a message.
 * @return 1 if msg is valid, 0 if msg is not valid.
 */
int 
ipp_validate_msg(char *regex, char *msg)
{
	regex_t		preg;
	int		ret;

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
 * @return constant for message type (example MSG_IPP), -1 if msg is not valid.
 */
int 
ipp_validate_unknown_msg(char *msg)
{
	unsigned int	i;
	int		is_valid = -1;

	if (!ipp_regex_msg || !msg) {
		return FALSE;
	}
	for (i = 0; ipp_regex_msg[i]; i++) {
		if (ipp_validate_msg(ipp_regex_msg[i], msg)) {
			is_valid = i;
			break;
		}
	}

	return is_valid;
}

/**
 * Allocates an empty ipp_socket. Don't forget to ipp_free_socket().
 * @return a malloc()'d ipp_socket structure.
 */
ipp_socket     *
ipp_new_socket()
{
	ipp_socket     *sock;
	sock = (ipp_socket *) malloc(sizeof(ipp_socket));
	if (!sock) {
		return NULL;
	}
	memset(sock, '\0', sizeof(ipp_socket));
	return sock;
}

/**
 * Deallocate an ipp_socket.
 */
void 
ipp_free_socket(ipp_socket * sock)
{
	if (sock) {
		ipp_disconnect(sock);
		free(sock);
	}
}

/**
 * Allocates an empty ipp_message. Don't forget to ipp_free_message().
 * @return a malloc()'d ipp_message structure.
 */
ipp_message    *
ipp_new_message()
{
	ipp_message    *msg;
	msg = (ipp_message *) malloc(sizeof(ipp_message));
	if (!msg) {
		return NULL;
	}
	memset(msg, '\0', sizeof(ipp_message));
	return msg;
}

/**
 * Parses msg->payload into msg->parsed.
 * @param msg an IPP message
 */
void 
ipp_parse_msg(ipp_message * msg)
{
	int		tokcnt    , i, j;
	char		delim = ' ';
	char           *s, *t;

	if ((msg == NULL || msg->payload == NULL || msg->parsed != NULL) && msg->type >= 0) {
		return;
	}
	tokcnt = 1;
	for (i = 0; msg->payload[i]; i++) {
		if (msg->payload[i] == delim) {
			tokcnt++;
		}
	}

	msg->parsed = (char **)malloc(sizeof(char *) * (tokcnt + 1));
	if (!(msg->parsed)) {
		/* malloc failed */
		return;
	}
	memset(msg->parsed, '\0', sizeof(char *) * (tokcnt + 1));
	s = msg->payload;
	for (i = 0; i < tokcnt; i++) {
		t = strchr(s, delim);
		if (t) {
			msg->parsed[i] = (char *)malloc(((t - s) + (sizeof(char) * 1)));
			if (!(msg->parsed[i])) {
				/* malloc() failed */
				for (j = 0; msg->parsed[j]; j++) {
					free(msg->parsed[j]);
					msg->parsed[j];
				}
				free(msg->parsed);
				msg->parsed = NULL;
				return;
			}
			memset(msg->parsed[i], '\0', ((t - s) + (sizeof(char) * 1)));
			memcpy(msg->parsed[i], s, t - s);
		} else {
			msg->parsed[i] = strdup(s);
		}

		t++;
		s = t;
	}
}

/**
 * Deallocate an ipp_message.
 */
void 
ipp_free_message(ipp_message * msg)
{
	int		i;

	if (msg) {
		if (msg->payload) {
			free(msg->payload);
			msg->payload = NULL;
		}
		if (msg->parsed) {
			for (i = 0; msg->parsed[i] != NULL; i++) {
				free(msg->parsed[i]);
				msg->parsed[i] = NULL;
			}
			free(msg->parsed);
			msg->parsed = NULL;
		}
		free(msg);
	}
}

/**
 * Allocates an empty ipp_card. Don't forget to ipp_free_card().
 * @return a malloc()'d ipp_card structure.
 */
ipp_card       *
ipp_new_card()
{
	ipp_card       *card;

	card = (ipp_card *) malloc(sizeof(ipp_card));
	if (card == NULL) {
		return NULL;
	}
	memset(card, '\0', sizeof(ipp_card));
	return card;
}

/**
 * Deallocate an ipp_card.
 */
void 
ipp_free_card(ipp_card * card)
{
	if (card) {
		free(card);
	}
}

/**
 * Allocates an empty ipp_player. Don't forget to ipp_free_player().
 * @return a malloc()'d ipp_card structure.
 */
ipp_player     *
ipp_new_player()
{
	ipp_player     *player;

	player = (ipp_player *) malloc(sizeof(ipp_player));
	if (player == NULL) {
		return NULL;
	}
	memset(player, '\0', sizeof(ipp_player));
	return player;
}

/**
 * Deallocates an ipp_player.
 */
void 
ipp_free_player(ipp_player * player)
{
	if (player) {
		if (player->name) {
			free(player->name);
			player->name = NULL;
		}
		if (player->sock) {
			ipp_disconnect(player->sock);
			ipp_free_socket(player->sock);
			player->sock = NULL;
		}
		if (player->hole[0]) {
			ipp_free_card(player->hole[0]);
			player->hole[0] = NULL;
		}
		if (player->hole[1]) {
			ipp_free_card(player->hole[1]);
			player->hole[1] = NULL;
		}
		player->bankroll = 0;
		player->amt_in = 0;
		player->still_in = 0;

		free(player);
	}
}

/**
 * Allocates an empty ipp_table. Don't for get to ipp_free_table().
 * @return a malloc()'d ipp_table structure.
 */
ipp_table      *
ipp_new_table()
{
	ipp_table      *table;

	table = (ipp_table *) malloc(sizeof(ipp_table));
	if (table == NULL) {
		return NULL;
	}
	memset(table, '\0', sizeof(ipp_table));
	return table;
}

/**
 * Deallocates an ipp_table.
 */
void 
ipp_free_table(ipp_table * table)
{
	int		i;

	if (table) {
		if (table->players) {
			for (i = 0; i < HOLDEM_PLAYERS_PER_TABLE; i++) {
				if (table->players[i]) {
					ipp_free_player(table->players[i]);
					table->players[i] = NULL;
				}
			}
		}
		if (table->board) {
			for (i = 0; i < HOLDEM_BOARD_CARDS; i++) {
				if (table->board[i]) {
					ipp_free_card(table->board[i]);
					table->board[i] = NULL;
				}
			}
		}
		table->num_players = 0;
		table->amt_to_call = 0;
		table->stage = 0;

		free(table);
		table = NULL;
	}
}

/**
 * INTERNAL FUNCTION. DO NOT USE OUTSIDE LIBTINYPOKER!!!
 * Checks a certificate to make sure it is valid.
 * @param session GNU TLS Session.
 * @param hostname the hostname of the server connected to.
 */
int 
__ipp_verify_cert(gnutls_session session, const char *hostname)
{
	unsigned int	status;
	const gnutls_datum *cert_list;
	unsigned int	cert_list_size;
	int		ret       , i, valid;
	gnutls_x509_crt	cert;

	ret = gnutls_certificate_verify_peers2(session, &status);
	if (ret < 0) {
		/* gnutls_certificate_verify_peers2 failed */
		return FALSE;
	}
	if (status & GNUTLS_CERT_INVALID) {
		/* The certificate is not trusted */
		return FALSE;
	}
	if (status & GNUTLS_CERT_SIGNER_NOT_FOUND) {
		/* The certificate hasn't got a known issuer */
		return FALSE;
	}
	if (status & GNUTLS_CERT_REVOKED) {
		/* The certificate has been revoked */
		return FALSE;
	}
	if (gnutls_certificate_type_get(session) != GNUTLS_CRT_X509) {
		/* The certificate is not an x509 certificate */
		return FALSE;
	}
	cert_list = gnutls_certificate_get_peers(session, &cert_list_size);
	if (cert_list == NULL || cert_list_size == 0) {
		/* No certificate was found */
		return FALSE;
	}
	for (i = 0; i < cert_list_size; i++) {
		valid = 1;

		if (gnutls_x509_crt_init(&cert) < 0) {
			/* init error */
			return FALSE;
		}
		if (gnutls_x509_crt_import(cert, &cert_list[i], GNUTLS_X509_FMT_DER) < 0) {
			/* error parsing certificate */
			valid = 0;
		}
		if (gnutls_x509_crt_get_expiration_time(cert) < time(0)) {
			/* Certificate has expired */
			valid = 0;
		}
		if (gnutls_x509_crt_get_activation_time(cert) > time(0)) {
			/* Certificate is not yet activated */
			valid = 0;
		}
		if (!gnutls_x509_crt_check_hostname(cert, hostname)) {
			/* Certificate doesn't match hostname */
			valid = 0;
		}
		gnutls_x509_crt_deinit(cert);

		if (!valid) {
			return FALSE;
		}
	}

	return TRUE;
}

/**
 * Connect to a server.
 * @param hostname the hostname of the server to connect to (example: host.domain.tld).
 * @param port the port number (example: 9999).
 * @param ca_file Path to Certificate Authority file.
 * @return a socket or NULL if an error happened.
 */
ipp_socket     *
ipp_connect(char *hostname, int port, char *ca_file)
{
	ipp_socket     *sock;
	int		ret;
	const int	kx_prio[] = {GNUTLS_KX_RSA, 0};
	const char     *err;

/* IPv4 */
/*
	struct sockaddr_in sin;
	struct hostent *he;
 */

	/* IPv4 and IPv6 */
	struct addrinfo *ai;
	struct addrinfo hints;
	struct addrinfo *runp;

	gnutls_transport_ptr_t ptr;
	long		gnutls_sock;

	/* TinyPoker -- create an empty socket structure */
	sock = ipp_new_socket();

	/* GNU TLS -- initialize the structure */
	gnutls_certificate_allocate_credentials(&(sock->x509_cred));
	gnutls_certificate_set_x509_trust_file(sock->x509_cred, ca_file, GNUTLS_X509_FMT_PEM);
	gnutls_init(&(sock->session), GNUTLS_CLIENT);
	gnutls_set_default_priority(sock->session);
	gnutls_kx_set_priority(sock->session, kx_prio);
	gnutls_credentials_set(sock->session, GNUTLS_CRD_CERTIFICATE, sock->x509_cred);

	/*
	 * TCP -- resolve the server's hostname, create a socket descriptor
	 * and connect
	 */

/* IPv4 */
/*
	memset(&sin, 0, sizeof(sin));
	sin.sin_port = htons((short)port);
	he = gethostbyname(hostname);
	if (!ret) {
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		return NULL;
	}
	sin.sin_family = he->h_addrtype;
	memcpy(&sin.sin_addr, he->h_addr, he->h_length);
	sock->sd = socket(AF_INET, SOCK_STREAM, 0);
	if (!(sock->sd)) {
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		return NULL;
	}
	ret = connect(sock->sd, (struct sockaddr *)&sin, sizeof(sin));
	if (ret) {
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		return NULL;
	}
*/
	/* IPv4 and IPv6 */
	memset (&hints, '\0', sizeof (hints));
	hints.ai_flags = AI_ADDRCONFIG;
	hints.ai_socktype = SOCK_STREAM;

	ret = getaddrinfo(hostname, IPP_SERVICE_NAME, &hints, &ai);
	if (ret != 0) {
		ipp_free_socket(sock);
		return NULL;
	}

	for (runp = ai; runp != NULL; runp = runp->ai_next) {
		sock->sd = socket(runp->ai_family, runp->ai_socktype, runp->ai_protocol);
		if (sock->sd == -1) {
			continue;
		}

		ret = connect(sock->sd, runp->ai_addr, runp->ai_addrlen);
		if (ret != 0) {
			close(sock->sd);
			sock->sd = -1;
			continue;
		}

		/* if we get here it means we are connected */
		break;
	}

	freeaddrinfo (ai);

	/* couldn't connect on any socket :( give up. */
	if (sock->sd == -1) {
		ipp_free_socket(sock);
		return NULL;
	}

	/* GNU TLS -- handshaking */
	gnutls_sock = sock->sd;
	ptr = (gnutls_transport_ptr_t) gnutls_sock;
	gnutls_transport_set_ptr(sock->session, ptr);
	ret = gnutls_handshake(sock->session);
	if (ret < 0) {
		gnutls_perror(ret);
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		return NULL;
	}
	ret = __ipp_verify_cert(sock->session, hostname);
	if (ret == 0) {
		/* can't verify cert */
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		return NULL;
	}
	return sock;
}

/**
 * Disconnect from the server.
 * @param sock a socket to disconnect.
 */
void 
ipp_disconnect(ipp_socket * sock)
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
	if (sock->x509_cred) {
		gnutls_certificate_free_credentials(sock->x509_cred);
	}
	/* set to NULL to prevent double free() and other misuse */
	memset(sock, '\0', sizeof(ipp_socket));
}

/**
 * INTERNAL FUNCTION. DO NOT USE OUTSIDE LIBTINYPOKER!!!
 * @param void_params a __ipp_readln_thread_params structure.
 */
void 
__ipp_readln_thread(void *void_params)
{
	int		ret;
	__ipp_readln_thread_params *params;
	params = (__ipp_readln_thread_params *) void_params;

	*(params->buffer) = (char *)malloc(sizeof(char) * (MAX_MSG_SIZE + 1));
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
ipp_message    *
ipp_read_msg(ipp_socket * sock, int timeout)
{
	__ipp_readln_thread_params params;
	char           *buffer;
	int		n         , is_valid, ret;
	pthread_t	reader;
	pthread_attr_t	reader_attr;
	time_t		clock;

	is_valid = FALSE;
	buffer = NULL;
	n = 0;

	params.sock = sock;
	params.buffer = &buffer;
	params.n = &n;

	pthread_attr_init(&reader_attr);
	pthread_attr_setdetachstate(&reader_attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&reader, &reader_attr, (void *(*) (void *))__ipp_readln_thread, (void *)&params);
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
	if (is_valid > -1) {
		ipp_message    *msg;
		msg = ipp_new_message();
		if (msg) {
			msg->type = is_valid;
			msg->payload = buffer;
			ipp_parse_msg(msg);
			return msg;
		} else {
			free(buffer);
			return NULL;
		}
	} else {
		free(buffer);
		return NULL;
	}
}

/**
 * INTERNAL FUNCTION. DO NOT USE OUTSIDE LIBTINYPOKER!!!
 * @param void_params a __ipp_writeln_thread_params structure.
 */
void 
__ipp_writeln_thread(void *void_params)
{
	int		ret;
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
int 
ipp_send_msg(ipp_socket * sock, ipp_message * msg, int timeout)
{
	__ipp_writeln_thread_params params;
	int		is_valid  , ret, n;
	pthread_t	writer;
	pthread_attr_t	writer_attr;
	time_t		clock;

	is_valid = FALSE;
	n = 0;

	ipp_normalize_msg(msg->payload);
	is_valid = ipp_validate_unknown_msg(msg->payload);
	if (is_valid > -1 && is_valid == msg->type) {
		int		len;
		char           *final_msg;

		len = strlen(msg->payload);
		final_msg = malloc(sizeof(char) * (len + 2));
		if (final_msg == NULL) {
			return FALSE;
		}
		memset(final_msg, '\0', len + 2);
		strncpy(final_msg, msg->payload, len + 2);
		final_msg[len] = '\n';

		params.sock = sock;
		params.buffer = final_msg;
		params.n = &n;

		pthread_attr_init(&writer_attr);
		pthread_attr_setdetachstate(&writer_attr, PTHREAD_CREATE_DETACHED);

		ret = pthread_create(&writer, &writer_attr, (void *(*) (void *))__ipp_writeln_thread, (void *)&params);
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
static int	done;

/**
 * Set done to 1 when SIGUSR2 is raised.
 * @param sig signal
 */
void 
__ipp_handle_sigusr2(int sig)
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
 * @param ca_file Certificate Authority
 * @param crl_file CRL
 * @param cert_file SSL/TLS Certificate File
 * @param key_file Private Key
 */
void		ipp_servloop(int port, void (*callback) (ipp_socket *), char *ca_file, char *crl_file, char *cert_file, char *key_file){
	int		slave, rc, optval;
	ipp_socket     *ipp_slave;

/* IPv4 */
/*
  	int master;
	struct pollfd	p;
	struct sockaddr_in sin;
	struct sockaddr_in client_addr;
	unsigned int	client_addr_len;
*/

	/* IPv4 and IPv6 */
	struct addrinfo *ai;
	struct addrinfo hints;
	struct addrinfo *runp;
	struct sockaddr_storage sockaddr;
	socklen_t sockaddrlen;
	struct pollfd sds[IPP_SERVER_MAX_SDS];
	int nsds, i;

	const int	kx_prio[] = {GNUTLS_KX_RSA, 0};
	gnutls_session_t session;
	gnutls_dh_params_t dh_params;
	gnutls_certificate_credentials_t x509_cred;
	gnutls_transport_ptr_t ptr;
	long		gnutls_sock;

	gnutls_certificate_allocate_credentials(&x509_cred);
	gnutls_certificate_set_x509_trust_file(x509_cred, ca_file, GNUTLS_X509_FMT_PEM);
	gnutls_certificate_set_x509_crl_file(x509_cred, crl_file, GNUTLS_X509_FMT_PEM);
	gnutls_certificate_set_x509_key_file(x509_cred, cert_file, key_file, GNUTLS_X509_FMT_PEM);

	gnutls_dh_params_init(&dh_params);
	gnutls_dh_params_generate2(dh_params, 1024);
	gnutls_certificate_set_dh_params(x509_cred, dh_params);

/* IPv4 */
/*
	client_addr_len = sizeof(client_addr);
	memset(&sin, 0, sizeof(sin));

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = (unsigned short)htons(port);
*/

	/* IPv4 and IPv6 */
	memset(&hints, '\0', sizeof (hints));
	hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
	hints.ai_socktype = SOCK_STREAM;
	rc = getaddrinfo(NULL, IPP_SERVICE_NAME, &hints, &ai);
	if (rc != 0) {
		gnutls_certificate_free_credentials(x509_cred);
		gnutls_dh_params_deinit(dh_params);
		return;
	}

/* IPv4 */
/*
	master = socket(PF_INET, SOCK_STREAM, 0);
	if (master < 0) {
		gnutls_certificate_free_credentials(x509_cred);
		gnutls_dh_params_deinit(dh_params);
		return;
	}
	setsockopt(master, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

	rc = bind(master, (struct sockaddr *)&sin, sizeof(sin));
	if (rc < 0) {
		gnutls_certificate_free_credentials(x509_cred);
		gnutls_dh_params_deinit(dh_params);
		return;
	}
	rc = listen(master, 64);
	if (rc < 0) {
		gnutls_certificate_free_credentials(x509_cred);
		gnutls_dh_params_deinit(dh_params);
		return;
	}
	p.fd = master;
	p.events = POLLIN;
	p.revents = 0;
*/

	memset(sds, '\0', sizeof(struct pollfd) * IPP_SERVER_MAX_SDS);

	nsds = 0;
	for (runp = ai; runp != NULL && nsds < IPP_SERVER_MAX_SDS; runp = runp->ai_next) {
		sds[nsds].fd = socket(runp->ai_family, runp->ai_socktype, runp->ai_protocol);
		if (sds[nsds].fd == -1) {
			continue;
		}

		optval = 1;
		setsockopt(sds[nsds].fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

		rc = bind(sds[nsds].fd, runp->ai_addr, runp->ai_addrlen);
		if (rc != 0) {
			close(sds[nsds].fd);
			sds[nsds].fd = -1;
			continue;
		}

		rc = listen(sds[nsds].fd, SOMAXCONN);
		if (rc != 0) {
			close(sds[nsds].fd);
			sds[nsds].fd = -1;
			continue;
		}

		sds[nsds].events = POLLIN;
		sds[nsds].revents = 0;
		nsds++;
	}

	freeaddrinfo (ai);

	if (nsds == 0) {
		gnutls_certificate_free_credentials(x509_cred);
		gnutls_dh_params_deinit(dh_params);
		return;
	}

	done = 0;
	signal(SIGUSR2, __ipp_handle_sigusr2);

	while (!done) {
		/* Poll master so that we don't block on accept */
		/*
		 * this is done so that when we signal we re-evaluate if
		 * !done == true
		 */

/* IPv4 */
/*
		poll(&p, 1, 30000);
		if (p.revents != POLLIN) {
			if (done) {
				break;
			} else {
				continue;
			}
		}
*/
		/* IPv4 and IPv6 */
		rc = poll(sds, nsds, 30000); 

		if (done) {
			break;
		}

		/* check if 1 or more sockets is ready for us */
		if (rc > 0) {
			for (i = 0; i < nsds; i++) {
				if (sds[i].revents & POLLIN) {
/* IPv4 */
/*
					slave = accept(master, (struct sockaddr *)&client_addr, &client_addr_len);
*/
					/* IPv4 and IPv6 */
					sockaddrlen = sizeof(struct sockaddr); /* probably not needed */
					slave = accept(sds[i].fd, (struct sockaddr *) &sockaddr, &sockaddrlen);
					if (slave < 0) {
						if (errno == EINTR) {
							continue;
						} else {
							for (i = 0; i < nsds; i++) {
								shutdown(sds[i].fd, SHUT_RDWR);
								close(sds[i].fd);
							}

							gnutls_certificate_free_credentials(x509_cred);
							gnutls_dh_params_deinit(dh_params);
							return;
						}
					}

					gnutls_init(&session, GNUTLS_SERVER);
					gnutls_set_default_priority(session);
					gnutls_kx_set_priority(session, kx_prio);
					gnutls_credentials_set(session, GNUTLS_CRD_CERTIFICATE, x509_cred);
					gnutls_certificate_server_set_request(session, GNUTLS_CERT_REQUEST);

					gnutls_sock = slave;
					ptr = (gnutls_transport_ptr_t) gnutls_sock;
					gnutls_transport_set_ptr(session, ptr);
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
					ipp_slave->x509_cred = NULL;
					memcpy(&(ipp_slave->sockaddr), &(sockaddr), sockaddrlen);
					ipp_slave->sockaddrlen = sockaddrlen;

					callback(ipp_slave);
				}

				sds[i].revents = 0;
				sds[i].events = POLLIN;
			}
		}
	}

	for (i = 0; i < nsds; i++) {
		shutdown(sds[i].fd, SHUT_RDWR);
		close(sds[i].fd);
	}

	gnutls_certificate_free_credentials(x509_cred);
	gnutls_dh_params_deinit(dh_params);
}
