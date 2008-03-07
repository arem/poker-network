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

#include "conf.h"

#include <confuse.h>
#include <libdaemon/dlog.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void default_config() {
	free_config();

	dbusername = (char*) malloc(sizeof(char)*strlen("pokerd")+1);
	dbpassword = (char*) malloc(sizeof(char)*strlen("pokerd")+1);
	dbhostname = (char*) malloc(sizeof(char)*strlen("localhost")+1);
	dbdatabase = (char*) malloc(sizeof(char)*strlen("pokerd")+1);

	if (!dbusername || !dbpassword || !dbhostname || !dbdatabase) {
		daemon_log(LOG_ERR,"malloc() FAILED!");
		exit(1);
	}


	strcpy(dbusername,"pokerd");
	strcpy(dbpassword,"pokerd");
	strcpy(dbhostname,"localhost");
	strcpy(dbdatabase,"pokerd");

	debug = 0;
	port = 9999;
}

/**
 *  read_config()
 *
 *  a wrapper to read & parse the configuration file 
 */
void read_config() {

	cfg_opt_t opts[] = {
		CFG_SIMPLE_STR("dbusername", &dbusername),
		CFG_SIMPLE_STR("dbpassword", &dbpassword),
		CFG_SIMPLE_STR("dbhostname", &dbhostname),
		CFG_SIMPLE_STR("dbdatabase", &dbdatabase),
		CFG_END()
       };

	cfg_t *cfg;

	free_config();

	cfg = cfg_init(opts, 0);        /* call libconfuse */
	cfg_parse(cfg, configfile);
	cfg_free(cfg);

}
