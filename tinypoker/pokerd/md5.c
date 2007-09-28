/*
 * Copyright (C) 2005, 2006, 2007 Thomas Cort <code@member.fsf.org>
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

#include "md5.h"

#include <libdaemon/dlog.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <openssl/md5.h>

int snprintf(char *str, size_t size, const char *format, ...);

/**
 *  char *md5str(char *str)
 *
 *  computes md5 digest for a given string
 */
char *md5str(char *str) {
	unsigned char *md;
	char *result;
	MD5_CTX c;
	int i;

	if ((md = (unsigned char *) malloc(16)) == NULL) {
		daemon_log(LOG_ERR,"[MD5S] %s",strerror(errno));
		exit(1);
	}

	if ((result = (char *) malloc(33)) == NULL) {
		daemon_log(LOG_ERR,"[MD5S] %s",strerror(errno));
		exit(1);
	}

	MD5_Init(&c);
	MD5_Update(&c, str, strlen(str));
	MD5_Final(md, &c);

	/* convert message digest to C string */
	for (i = 0; i < 16; i++) {
		snprintf(result+(i*2),3,"%.2x",md[i]);
	}

	free(md);
	return result;
}
