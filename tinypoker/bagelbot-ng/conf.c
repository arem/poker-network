/*
 * bagelbot-ng - Trivial client for tinypokerd
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
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
#include <confuse.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Basic sanity check on the configfile. Rules: file must exist,
 * file must be a regular file, file must have permission no more
 * than 640 (-rw-r-----).
 * @return 1 if the rules are broken, 0 if file is ready to be read.
 */
int check_config_permissions()
{
	struct stat s;

	if (!configfile || !configfile[0]) {
		daemon_log(LOG_ERR, "Configfile path not set");
		return 1;
	}

	if (stat(configfile, &s) < 0) {
		daemon_log(LOG_ERR, "Could not stat config file");
		return 1;
	}

	if (!S_ISREG(s.st_mode)) {
		daemon_log(LOG_ERR, "Config file '%s' is not a regular file", configfile);
		return 1;
	}

	if (s.st_mode & (S_IXUSR | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH)) {
		daemon_log(LOG_ERR, "Config file '%s' has a bad mode", configfile);
		return 1;
	}

	return 0;
}

/**
 * Reads 'configfile' and allocates memory for their values
 */
void configure()
{
	cfg_opt_t opts[] = {
		CFG_SIMPLE_STR("user", &user),
		CFG_SIMPLE_STR("host", &host),
		CFG_SIMPLE_INT("port", &port),
		CFG_END()
	};

	cfg_t *cfg;

	user = host = NULL;
	port = 0;

	if (check_config_permissions()) {
		return;
	}

	cfg = cfg_init(opts, 0);	/* call libconfuse */
	cfg_parse(cfg, configfile);
	cfg_free(cfg);
}
