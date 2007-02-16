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

#ifndef __BYTE_H
#define __BYTE_H

struct byte_array {
	char *data;
	int size;
	int capacity;
	int position;
};

struct byte_array *new_byte_array(int capacity);
void byte_array_append_bytes(struct byte_array *ba, int size, char *data);
void byte_array_append_string(struct byte_array *ba, char *data);
void byte_array_append_int(struct byte_array *ba, int val);
char *byte_array_get_bytes(struct byte_array *b);
char *byte_array_read_string(struct byte_array *ba);
int byte_array_read_int(struct byte_array *ba);
void byte_array_destroy(struct byte_array *ba);

#endif
