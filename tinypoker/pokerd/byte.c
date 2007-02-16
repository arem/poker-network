/*
 * Copyright (C) 2005, 2006 Thomas Cort
 *
 * This file is part of pokerd.
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

/*
 * This file is loosely based upon, but not copied from ByteArray.c by VASTMIND.COM
 */

#include "log.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>

#include "byte.h"

extern int snprintf(char *str, size_t size, const char *format, ...);

/**
 *  struct byte_array *new_byte_array(int capacity)
 *
 *  create a new byte array with initial capacity of "capacity"
 */
struct byte_array *new_byte_array(int capacity) {
	struct byte_array *ba;

	char buf[128];
	bzero(buf,128);

	if ((ba = (struct byte_array *) malloc(sizeof(struct byte_array))) == NULL) {
		snprintf(buf,128,"[ERRR] %s",strerror(errno));
		logit(buf);
		exit(1);
	}

	if ((ba->data = (char *) malloc(sizeof(char)*capacity)) == NULL) {
		snprintf(buf,128,"[ERRR] %s",strerror(errno));
		logit(buf);
		exit(1);
	}

	ba->capacity = capacity;

	ba->size     = 0;
	ba->position = 0;

	return ba;
}

/**
 *  void byte_array_append_bytes(struct byte_array *ba, int size, char *data)
 *
 *  append bytes to the byte array
 */
void byte_array_append_bytes(struct byte_array *ba, int size, char *data) {

	if (!ba || !data || !size)
		return;

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
 *  void byte_array_append_string(struct byte_array *ba, char *str)
 *
 *  append a null terminated C String
 */
void byte_array_append_string(struct byte_array *ba, char *str) {

	if (!ba || !str)
		return;

	byte_array_append_bytes(ba,strlen(str)+1,str);
}

/**
 *  void byte_array_append_int(struct byte_array *ba, int val) 
 *
 *  append a 4-byte int
 */
void byte_array_append_int(struct byte_array *ba, int val) {

	if (!ba)
		return;

	val = htonl(val); /* we use network byte order ints */
	byte_array_append_bytes(ba, 4, (char *)&val);
}

/**
 *  int byte_array_read_int(struct byte_array *ba)
 *
 *  read a 4-byte int
 */
int byte_array_read_int(struct byte_array *ba) {
	int r = 0;

	if (!ba)
		return -1;

	if ((ba->position+4) > ba->size)
		return -1;

	memcpy(&r, ba->data + ba->position, 4);
	ba->position += 4;

	r = ntohl(r);
	return r;
}

/**
 *  char *byte_array_read_string(struct byte_array *ba)
 *
 *  read a null terminated C String
 */
char *byte_array_read_string(struct byte_array *ba) {
	struct byte_array *temp;
	char *str, c;

	char buf[128];
	bzero(buf,128);

	if (!ba)
		return NULL;

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

	if ((str = (char *) malloc(temp->size)) == NULL) {
		snprintf(buf,128,"[ERRR] %s",strerror(errno));
		logit(buf);
		exit(1);
	}

	memcpy(str, temp->data, temp->size);
	byte_array_destroy(temp);

	return str;
}

/**
 *  byte_array_get_bytes(struct byte_array *ba)
 *
 *  return all of the data
 */
char *byte_array_get_bytes(struct byte_array *ba) {
	char buf[128];
	char *data;

	bzero(buf,128);

	if ((data = (char *)malloc(ba->size)) == NULL) {
		snprintf(buf,128,"[ERRR] %s",strerror(errno));
		logit(buf);
		exit(1);
	}

        memcpy(data, ba->data, ba->size);
        return data;
}


/**
 *  void byte_array_destroy(struct byte_array *ba)
 *
 *  free's a byte_array and all of its data
 */
void byte_array_destroy(struct byte_array *ba) {

	if (ba) {
		if (ba->data)
			free(ba->data);
		free(ba);
	}
	/* all other fields are local to the struct */
}

