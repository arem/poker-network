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

#include "conf.h"

#include <libdaemon/dlog.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern int yyparse();
extern int snprintf(char *str, size_t size, const char *format, ...);

void default_config() {
	strcpy(dbusername,"pokerd");
	strcpy(dbpassword,"pokerd");
	strcpy(dbhostname,"localhost");
	strcpy(dbdatabase,"pokerd");
	strcpy(version,"0.2.1");

	debug = 0;
	port = 9999;
}

/**
 *  read_config()
 *
 *  a wrapper to read & parse the configuration file 
 */
void read_config() {

	/* Check that there is a file name in configfile */
	/* if not use defaults                           */
	if (configfile[0]) {
		yyin = fopen(configfile,"r");
		if (!yyin) {
			daemon_log(LOG_ERR,"[CONF] %s",strerror(errno));
			exit(1);
		}
		daemon_log(LOG_INFO,"[CONF] Parsing config file");
		yyparse();
		fclose(yyin);
	} else {
		default_config();
	}
}
