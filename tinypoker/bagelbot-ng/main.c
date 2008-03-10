/*
 * BagelBot - Trivial client for pokerd
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

#include <getopt.h>
#include <libdaemon/dlog.h>

#include "conf.h"
#include "play.h"
#include "main.h"
#include "net.h"
#include "byte.h"

/**
 * Send a JOIN_GAME message to the server.
 * @return 1 for GOODPASS and 0 for failure.
 */
int authenticate() {
        int type;
        struct byte_array *ba;

        ba = new_byte_array(32);

        byte_array_append_string(ba,user);	/* User */
        byte_array_append_string(ba,pass);	/* Pass */
        byte_array_append_int(ba,1);
        byte_array_append_string(ba,"BagelBot");/* User-agent */

        write_message(JOIN_GAME,ba);
        byte_array_destroy(ba);

        ba = read_message(&type);
        byte_array_destroy(ba);

        if (type == GOODPASS) {
                return 1;
        } else {
                return 0;
        }
}


/**
 * Displays some usage information, command line parameters and whatnot.
 * @param program the name of the program.
 */
void display_help(char *program) {
	daemon_log(LOG_INFO, "Usage: %s [options]", program);
	daemon_log(LOG_INFO, "Options:");
	daemon_log(LOG_INFO, "    -h --help        Show this help message");
	daemon_log(LOG_INFO, "    -v --version     Show version information");
}

/**
 * Displays some version and copyright information upon request (-v or --version).
 */
void display_version() {
	daemon_log(LOG_INFO, "%s v%1.1f", PROGRAM, VERSION);
	daemon_log(LOG_INFO, "Copyright (C) 2005, 2007 Thomas Cort");
	daemon_log(LOG_INFO, "This is free software; see the source for copying conditions.  There is NO");
	daemon_log(LOG_INFO, "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.");
}


/**
 * A command line parser using getopts.
 * @param argc The number of command line arguments coming in argv.
 * @param argv The command line arguments.
 * @return Returns 0 on success and non-zero when we want the program to terminate.
 */
int parse_args(int argc, char **argv) {
	int option_index = 0, done = 0, c;

	static struct option long_options[] = {
		{      "help",       no_argument, 0, 'h'},
		{   "version",       no_argument, 0, 'v'},
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

/**
 * A command line poker bot.
 * @param argc The number of command line arguments coming in argv.
 * @param argv The command line arguments.
 * @return Returns 0 on success and non-zero when we want the program to terminate.
 */
int main(int argc, char **argv) {
	int rc;

	if (argc < 1 || !argv || !argv[0]) {
		daemon_log(LOG_ERR, "(%u:%s) Cannot determine program name from argv[0]", __FILE__, __LINE__);
		return 1;
	}

	/* Set the default config file path */
	configfile = (char*) malloc((strlen("/etc/bagelbot.conf") + 2) * sizeof(char));
	if (!configfile) {
		daemon_log(LOG_ERR, "malloc() failed!");
		return 255;
	}
	bzero(configfile,(strlen("/etc/bagelbot.conf") + 2) * sizeof(char));
	snprintf(configfile,strlen("/etc/bagelbot.conf") + 1,"/etc/bagelbot.conf");

	configure();

	rc = parse_args(argc, argv);
	if (rc) {
		return rc;
	}

	if (!host || !pass || !port || !user) {
		daemon_log(LOG_ERR, "Could not determine one or more configuration setting.");
		free_config();
		return 255;
	} else {
		daemon_log(LOG_INFO, "Connecting to %s:%d as %s",host,port,user);
	}

        connect_to_server(host,port);

        if (authenticate()) {
                daemon_log(LOG_INFO, "Authenticated!");
                play();
        } else {
                daemon_log(LOG_ERR, "Not Authenticated!");
        }

        disconnect_from_server();
	free_config();
	return 0;
}
