/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <tcort@tomcort.com>
 *
 * This file is part of TinyPoker.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _XOPEN_SOURCE 500

#include <glib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "tinypokerd.h"
#include "log.h"

/**
 * A lock used to serialize access to writer.
 */
GMutex *writer_lock;

char *protocol_logger_timestamp()
{
	struct tm *tm;
	time_t t;
	size_t len;
	char *s;

	len = sizeof(char) * 32;

	t = time(NULL);
	tm = gmtime(&t);

	s = (char *) malloc(len);
	if (!s) {
		return NULL;
	}
	memset(s, '\0', len);

	strftime(s, 32, ISO8601_FORMAT, tm);
	return s;
}

void protocol_logger_init()
{
	writer_lock = g_mutex_new();
}

/**
 * Logs internet poker protocol messages.
 * @param msg the message to log.
 */
void protocol_logger(char *msg)
{
	static unsigned long long id = 0LL;

	if (msg && msg[0]) {
		g_mutex_lock(writer_lock);

		/* TODO: write to log */

		id++;
		g_mutex_unlock(writer_lock);
	}
}

void protocol_logger_exit()
{
	g_mutex_free(writer_lock);
	writer_lock = NULL;

	return;
}
