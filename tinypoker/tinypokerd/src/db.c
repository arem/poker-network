/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This file is part of tinypokerd.
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
#include <stdlib.h>
#include <sqlite3.h>

#include "config.h"
#include "db.h"

/**
 * Database Structure.
 */
static sqlite3 *db;

/**
 * Connects to a SQLite Database
 * @return 0 if connected, -1 if failed to connect
 */
int db_connect()
{
	int rc;

	db = NULL;

	/* connect */
	rc = sqlite3_open(database, &db);
	if (rc) {
		daemon_log(LOG_ERR, "[DBDB] Can't open database (%s): %s", database, sqlite3_errmsg(db));
		sqlite3_close(db);
		db = NULL;
		return -1;
	}

	daemon_log(LOG_INFO, "[DBDB] Connected");
	return 0;
}

/**
 * Close the connection to a SQLite Database
 */
void db_disconnect()
{
	if (db) {
		sqlite3_close(db);
		db = NULL;
	}
	daemon_log(LOG_INFO, "[DBDB] Disconnected");
}
