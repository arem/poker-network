/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 * 
 * This file is part of tinypokerd.
 * 
 * tinypokerd is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * tinypokerd is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * tinypokerd.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <tinypoker.h>

/**
 * Location of the database file. Default: /var/lib/tinypokerd/tinypokerd.db
 */
char           *database;

/**
 * The type of game we're playing (holdem, draw, stud)
 */
enum game_type	game_type;

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
void		config_parse();

/**
 * Release any resources that hold configuration information.
 * This function effectively resets all configurable values.
 * It should be called at the end of the program.
 */
void		config_free();

#endif
