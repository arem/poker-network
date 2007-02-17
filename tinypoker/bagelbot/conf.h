/*
 * BagelBot - Trivial client for pokerd
 * Copyright (C) 2005, 2007 Thomas Cort <code@member.fsf.org>
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

#ifndef __CONF_H
#define __CONF_H

#ifndef NULL
#define NULL 0
#endif

char *configfile;
char *user;
char *pass;
char *host;
int   port;

void configure();

#define free_config()				\
	if (user) { free(user); user = NULL; } \
	if (pass) { free(pass); pass = NULL; } \
	if (host) { free(host); host = NULL; }

#endif