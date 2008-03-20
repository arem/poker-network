/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This file is part of bagelbot-ng.
 *
 * bagelbot-ng is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * bagelbot-ng is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bagelbot-ng.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CONF_H
#define __CONF_H

#ifndef NULL
#define NULL 0
#endif

char *configfile;
char *user;
char *host;
int port;

/**
 * Reads 'configfile' and allocates memory for their values
 */
void configure();

/**
 * Macro to free the user and host (if set).
 */
#define free_config()				\
	if (user) { free(user); user = NULL; } \
	if (host) { free(host); host = NULL; }

#endif
