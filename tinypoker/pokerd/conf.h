/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
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

#ifndef __CONFIG_H
#define __CONFIG_H

#include <stdio.h>

char *configfile;
char *dbusername;
char *dbpassword;
char *dbhostname;
char *dbdatabase;

int port;
int debug;

extern FILE *yyin;

int lineno;

void read_config();
void default_config();

#define free_config()								\
	do {									\
		if (dbusername) { free(dbusername); dbusername = NULL; }	\
		if (dbpassword) { free(dbpassword); dbpassword = NULL; }	\
		if (dbhostname) { free(dbhostname); dbhostname = NULL; }	\
		if (dbdatabase) { free(dbdatabase); dbdatabase = NULL; }	\
	} while (0)

#endif
