/*
 * BagelBot - Trivial client for pokerd
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
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

#ifndef __BYTE_H
#define __BYTE_H

struct byte_array {
	char *data;
	int size;
	int capacity;
	int position;
};

/**
 * Allocates a new byte array.
 * @param capacity the maximum allowable size of the byte array.
 * @return a new byte_array struct.
 */
struct byte_array *new_byte_array(int capacity);

/**
 * Append data to the byte array.
 * @param ba the byte array to append to.
 * @param size the number of bytes to copy.
 * @param data the bytes to copy.
 */
void byte_array_append_bytes(struct byte_array *ba, int size, char *data);

/**
 * Append a null terminated C String.
 * @param ba the byte array to append to.
 * @param str a null terminated string.
 */
void byte_array_append_string(struct byte_array *ba, char *data);

/**
 * Append a 4-byte int.
 * @param ba the byte array to append to.
 * @param val a 4-byte int in host byte order.
 */
void byte_array_append_int(struct byte_array *ba, int val);

/**
 * Read a 4-byte int.
 * @param ba the byte array to read from.
 * @return the 4 byte host order integer read. In case of error -1 is returned; not an ideal solution :(
 */
char *byte_array_get_bytes(struct byte_array *b);

/**
 * Read a null terminated C String
 * @param ba the byte array to read from.
 * @return a null terminated C String or NULL if an error occurred.
 */
char *byte_array_read_string(struct byte_array *ba);

/**
 * Return all of the data.
 * @param ba byte array to read from.
 * @return a copy of ba->data.
 */
int byte_array_read_int(struct byte_array *ba);

/**
 * Free's a byte_array and all of its data.
 * @param ba the byte array to free.
 */
void byte_array_destroy(struct byte_array *ba);

#endif
