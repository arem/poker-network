/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This file is part of cppbot.
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

#include "net.h"
#include "byte.h"

int sd;

/**
 *  struct byte_array *read_message(int sd, int *type)
 *
 *  read a message and store it in a byte_array
 *  set type
 */
struct byte_array *read_message(int *type) {
	struct byte_array *ba;
	char *data;
	int i, size = 0;

	/* grab message type */
	for (i = 0; i < 4; i++) {
		if (recv(sd,((char *)(type))+i,1,0) < 0) {
			perror("recv");
			return NULL;
		}
	}

	*type = ntohl(*type);

	/* grab message size */
	for (i = 0; i < 4; i++) {
		if (recv(sd,((char *)(&size))+i,1,0) < 0) {
			perror("recv");
			return NULL;
		}
	}

	size = ntohl(size);

	/* grab message data */
	if (size > 0) {
		if ((data = (char *) malloc(size)) == NULL) {
			perror("malloc");
			exit(1);
		}

		for (i = 0; i < size; i++) {
			if (recv(sd,data+i,1,0) < 0) {
				perror("recv");
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
 *  int write_message(int sd, int type, struct byte_array *)
 *
 *  write a message to sd
 */
void write_message(int type, struct byte_array *ba) {
	int size;
	char *data;

	data = byte_array_get_bytes(ba);

	type = htonl(type);
	size = htonl(ba->size);

	if (send(sd, (char *)&type, 4, 0) < 0) {
		perror("send");
		free(data);
		return;
	}

	if (send(sd, (char *)&size, 4, 0) < 0) {
		perror("send");
		free(data);
		return;
	}

	if (ba->size > 0) {
		if (send(sd, data, ba->size, 0) < 0) {
			perror("send");
			free(data);
			return;
		}
	}
	free(data);
}

/**
 *  void connect_to_server(char *hostname, int port)
 *
 *  connect to the server
 */
void connect_to_server(char *hostname, int port) {
	struct hostent *he;
	struct sockaddr_in sin;

	memset(&sin, 0, sizeof(sin));
	sin.sin_port = htons((short)port);;


	if (!(he = gethostbyname(hostname))) {
		perror("gethostbyname");
		exit(1);
	}

	sin.sin_family = he->h_addrtype;
	memcpy(&sin.sin_addr, he->h_addr, he->h_length);

	if (!(sd = socket(AF_INET, SOCK_STREAM, 0))) {
		perror("socket");
		exit(1);
	}

	if (connect(sd, (struct sockaddr *) &sin, sizeof(sin))) {
		perror("connect");
		exit(1);
	}
}

/**
 *  void disconnect_from_server()
 *
 *  shutdown & close the connection with the server 
 */
void disconnect_from_server() {
	shutdown(sd,SHUT_RDWR);
	close(sd);
}
