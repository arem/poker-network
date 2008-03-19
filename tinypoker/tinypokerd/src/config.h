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

#ifndef __CONFIG_H
#define __CONFIG_H

/**
 * The TCP port to listen on. Default: 9899
 */
int port;

/**
 * Location of the database file. Default: /var/lib/tinypokerd/tinypokerd.db
 */
char *database;

/**
 * The default database file location
 */
#define DEFAULT_DATABASE "/var/lib/tinypokerd/tinypokerd.db"

/**
 * The default configuration file location.
 */
#define DEFAULT_CONFIGFILE "/etc/tinypokerd.conf"

/**
 * Parses an tinypokerd.conf configuration file.
 */
void config_parse();

/**
 * Release any resources that hold configuration information.
 * This function effectively resets all configurable values.
 * It should be called at the end of the program.
 */
void config_free();

#endif
