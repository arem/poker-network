/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 * 
 * This file is part of tinypokerbot.
 * 
 * tinypokerbot is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * tinypokerbot is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * tinypokerbot.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <strings.h>
#include <getopt.h>
#include <libdaemon/dlog.h>
#include <tinypoker.h>

#include "conf.h"
#include "main.h"
#include "signal.h"

/**
 * Displays some usage information, command line parameters and whatnot.
 * @param program the name of the program.
 */
void display_help(char *program)
{
	daemon_log(LOG_INFO, "Usage: %s [options]", program);
	daemon_log(LOG_INFO, "Options:");
	daemon_log(LOG_INFO, "    -h --help        Show this help message");
	daemon_log(LOG_INFO, "    -v --version     Show version information");
}

/**
 * Displays some version and copyright information upon request (-v or --version).
 */
void display_version(void)
{
	daemon_log(LOG_INFO, "%s v%s", PROGRAM, VERSION);
	daemon_log(LOG_INFO, "Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>");
	daemon_log(LOG_INFO, "This is free software; see the source for copying conditions.  There is NO");
	daemon_log(LOG_INFO, "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.");
}


/**
 * A command line parser using getopts.
 * @param argc The number of command line arguments coming in argv.
 * @param argv The command line arguments.
 * @return Returns 0 on success and non-zero when we want the program to terminate.
 */
int parse_args(int argc, char **argv)
{
	int option_index = 0, done = 0, c;

	static struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"version", no_argument, 0, 'v'},
		{0, 0, 0, 0}
	};

	while (!done) {
		c = getopt_long(argc, argv, "hv", long_options, &option_index);
		if (c < 0) {
			break;
		}
		switch (c) {
		case 'h':
			display_help(argv[0]);
			done = 1;
			break;
		case 'v':
			display_version();
			done = 1;
			break;
		default:
			display_help(argv[0]);
			done = 1;
			break;
		}
	}

	return done;
}

void protocol_logger(char *msg)
{
	if (msg && msg[0]) {
		daemon_log(LOG_INFO, "%s", msg);
	}
}

/**
 * A command line poker bot.
 * @param argc The number of command line arguments coming in argv.
 * @param argv The command line arguments.
 * @return Returns 0 on success and non-zero when we want the program to terminate.
 */
int main(int argc, char **argv)
{
	ipp_socket *sock;
	int rc;

	if (argc < 1 || !argv || !argv[0]) {
		daemon_log(LOG_ERR, "(%u:%s) Cannot determine program name from argv[0]", __LINE__, __FILE__);
		return 1;
	}

	install_signal_handlers();

	/* Set the default config file path */
	configfile = (char *) malloc((strlen("tinypokerbot.conf") + 2) * sizeof(char));
	if (!configfile) {
		daemon_log(LOG_ERR, "malloc() failed!");
		return 255;
	}
	bzero(configfile, (strlen("tinypokerbot.conf") + 2) * sizeof(char));
	snprintf(configfile, strlen("tinypokerbot.conf") + 1, "tinypokerbot.conf");

	configure();

	rc = parse_args(argc, argv);
	if (rc) {
		return rc;
	}
	if (!host || !user) {
		daemon_log(LOG_ERR, "[MAIN] Could not determine one or more configuration setting from '%s'", configfile);
		free_config();
		return 255;
	} else {
		daemon_log(LOG_INFO, "[MAIN] Connecting to %s as %s", host, user);
	}

	ipp_init();

	if ((sock = ipp_client_handshake(host, "ca.pem", user, pass, "500", protocol_logger))) {
		daemon_log(LOG_INFO, "[MAIN] Handshake OK");

/* TODO dlopen */


		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
	} else {
		daemon_log(LOG_ERR, "[MAIN] Handshake Failed!");
	}

	ipp_exit();

	free_config();
	return 0;
}
