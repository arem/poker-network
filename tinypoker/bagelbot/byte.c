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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>

#include <libdaemon/dlog.h>

#include "byte.h"
#include "conf.h"

/**
 * Allocates a new byte array.
 * @param capacity the maximum allowable size of the byte array.
 * @return a new byte_array struct.
 */
struct byte_array *new_byte_array(int capacity) {
	struct byte_array *ba;

	ba = (struct byte_array *) malloc(sizeof(struct byte_array));
	if (!ba) {
		daemon_log(LOG_ERR, "malloc %s", strerror(errno));
		free_config();
		exit(1);
	}

	ba->data = (char *) malloc(sizeof(char) * capacity);
	if (!ba) {
		daemon_log(LOG_ERR, "malloc %s", strerror(errno));
		free_config();
		exit(1);
	}

	ba->capacity = capacity;

	ba->size     = 0;
	ba->position = 0;

	return ba;
}

/**
 * Append data to the byte array.
 * @param ba the byte array to append to.
 * @param size the number of bytes to copy.
 * @param data the bytes to copy.
 */
void byte_array_append_bytes(struct byte_array *ba, int size, char *data) {

	if (!ba || !data || !size) {
		return;
	}

	/* check if we have enough space */
	if ((ba->size + size) >= ba->capacity) {

		/* make more space */
	        struct byte_array *nba = new_byte_array(ba->capacity + size);
	        byte_array_append_bytes(nba, ba->size, ba->data);
	        free(ba->data);
	        ba->data = nba->data;
	        ba->capacity = nba->capacity;
	        free(nba);
	}

	memcpy(ba->data + ba->size, data, size);
        ba->size += size;
}

/**
 * Append a null terminated C String.
 * @param ba the byte array to append to.
 * @param str a null terminated string.
 */
void byte_array_append_string(struct byte_array *ba, char *str) {

	if (!ba || !str) {
		return;
	}

	byte_array_append_bytes(ba,strlen(str)+1,str);
}

/**
 * Append a 4-byte int.
 * @param ba the byte array to append to.
 * @param val a 4-byte int in host byte order.
 */
void byte_array_append_int(struct byte_array *ba, int val) {

	if (!ba) {
		return;
	}

	val = htonl(val); /* we use network byte order ints */
	byte_array_append_bytes(ba, 4, (char *)&val);
}

/**
 * Read a 4-byte int.
 * @param ba the byte array to read from.
 * @return the 4 byte host order integer read. In case of error -1 is returned; not an ideal solution :(
 */
int byte_array_read_int(struct byte_array *ba) {
	int r = 0;

	if (!ba) {
		return -1;
	}

	/* don't read past the end of the data */
	if ((ba->position+4) > ba->size) {
		return -1;
	}

	memcpy(&r, ba->data + ba->position, 4);
	ba->position += 4;

	r = ntohl(r);
	return r;
}

/**
 * Read a null terminated C String
 * @param ba the byte array to read from.
 * @return a null terminated C String or NULL if an error occurred.
 */
char *byte_array_read_string(struct byte_array *ba) {
	struct byte_array *temp;
	char *str, c;

	if (!ba) {
		return NULL;
	}

	temp = new_byte_array(20);

	/* get 1st char */
	if (ba->position < ba->size) {
		c = ba->data[ba->position];
		ba->position++;
	} else {
		c = 0;
	}

	byte_array_append_bytes(temp, 1, &c);

	while (ba->position < ba->size && c) {

		/* get another char */
		if (ba->position < ba->size) {
			c = ba->data[ba->position];
			ba->position++;
			byte_array_append_bytes(temp, 1, &c);
		} else {
			c = 0;
		}
	}

	c = '\0'; /* null terminated */
	byte_array_append_bytes(temp, 1, &c);

	str = (char *) malloc(temp->size);
	if (!str) {
		daemon_log(LOG_ERR, "malloc %s", strerror(errno));
		free_config();
		exit(1);
	}

	memcpy(str, temp->data, temp->size);
	byte_array_destroy(temp);

	return str;
}

/**
 * Return all of the data.
 * @param ba byte array to read from.
 * @return a copy of ba->data.
 */
char *byte_array_get_bytes(struct byte_array *ba) {
	char *data;

	data = (char *)malloc(ba->size);
	if (!data) {
		daemon_log(LOG_ERR, "malloc %s", strerror(errno));
		free_config();
		exit(1);
	}

        memcpy(data, ba->data, ba->size);
        return data;
}


/**
 * Free's a byte_array and all of its data.
 * @param ba the byte array to free.
 */
void byte_array_destroy(struct byte_array *ba) {

	if (ba) {
		if (ba->data) {
			free(ba->data);
		}

		free(ba);
		ba = NULL;
	}
	/* all other fields are local to the struct */
}

