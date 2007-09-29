/*
 * BagelBot - Trivial client for pokerd
 * Copyright (C) 2005, 2006, 2007 Thomas Cort <code@member.fsf.org>
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

#include <libdaemon/dlog.h>

#include "net.h"
#include "byte.h"
#include "conf.h"

/**
 * Read a message and store it in a byte_array.
 * @param type the type of message read from the socket.
 * @return the message itself.
 */
struct byte_array *read_message(int *type) {
	struct byte_array *ba;
	char *data;
	int i, size = 0;

	/* grab message type */
	for (i = 0; i < 4; i++) {
		if (recv(sd,((char *)(type))+i,1,0) < 0) {
			daemon_log(LOG_ERR, "recv %s", strerror(errno));
			return NULL;
		}
	}

	*type = ntohl(*type);

	/* grab message size */
	for (i = 0; i < 4; i++) {
		if (recv(sd,((char *)(&size))+i,1,0) < 0) {
			daemon_log(LOG_ERR, "recv %s", strerror(errno));
			return NULL;
		}
	}

	size = ntohl(size);

	/* grab message data */
	if (size > 0) {
		if ((data = (char *) malloc(size)) == NULL) {
			daemon_log(LOG_ERR, "malloc %s", strerror(errno));
			free_config();
			exit(1);
		}

		for (i = 0; i < size; i++) {
			if (recv(sd,data+i,1,0) < 0) {
				daemon_log(LOG_ERR, "recv %s", strerror(errno));
				free(data);
				return NULL;
			}
		}

		ba = new_byte_array(size);
		byte_array_append_bytes(ba,size,data);

		free(data);
		return ba;
	} else {
		return NULL;
	}
}

/**
 * Write a message to the socket.
 * @param type the type of message we're sending.
 * @param ba the message itself.
 */
void write_message(int type, struct byte_array *ba) {
	int size;
	char *data;

	data = byte_array_get_bytes(ba);

	type = htonl(type);
	size = htonl(ba->size);

	if (send(sd, (char *)&type, 4, 0) < 0) {
		daemon_log(LOG_ERR, "send %s", strerror(errno));
		free(data);
		return;
	}

	if (send(sd, (char *)&size, 4, 0) < 0) {
		daemon_log(LOG_ERR, "send %s", strerror(errno));
		free(data);
		return;
	}

	if (ba->size > 0) {
		if (send(sd, data, ba->size, 0) < 0) {
			daemon_log(LOG_ERR, "send %s", strerror(errno));
			free(data);
			return;
		}
	}
	free(data);
}

/**
 * Connect to the server.
 * @param hostname server to connect to.
 * @param the TCP port to connect to.
 */
void connect_to_server(char *hostname, int port) {
	struct hostent *he;
	struct sockaddr_in sin;

	memset(&sin, 0, sizeof(sin));
	sin.sin_port = htons((short)port);;


	if (!(he = gethostbyname(hostname))) {
		daemon_log(LOG_ERR, "gethostbyname %s", strerror(errno));
		free_config();
		exit(1);
	}

	sin.sin_family = he->h_addrtype;
	memcpy(&sin.sin_addr, he->h_addr, he->h_length);

	if (!(sd = socket(AF_INET, SOCK_STREAM, 0))) {
		daemon_log(LOG_ERR, "socket %s", strerror(errno));
		free_config();
		exit(1);
	}

	if (connect(sd, (struct sockaddr *) &sin, sizeof(sin))) {
		daemon_log(LOG_ERR, "connect %s", strerror(errno));
		free_config();
		exit(1);
	}
}

/**
 *  Shutdown and close the connection with the server .
 */
void disconnect_from_server() {
	shutdown(sd,SHUT_RDWR);
	close(sd);
}
