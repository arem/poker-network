/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <linuxgeek@gmail.com>
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
#include <tinypoker.h>

#include <libguile.h>
#include <guile/gh.h>

#include "main.h"

/**
 * Displays some usage information, command line parameters and whatnot.
 * @param program the name of the program.
 */
void display_help(char *program)
{
	printf("Usage: %s [options]\n", program);
	printf("Options:\n");
	printf("    -h --help        Show this help message\n");
	printf("    -v --version     Show version information\n");
}

/**
 * Displays some version and copyright information upon request (-v or --version).
 */
void display_version(void)
{
	printf("%s v%s\n", PROGRAM, VERSION);
	printf("Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <linuxgeek@gmail.com>\n");
	printf("This is free software; see the source for copying conditions.  There is NO\n");
	printf("warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
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
		printf("%s\n", msg);
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
	int rc;


	if (argc < 1 || !argv || !argv[0]) {
		printf("Cannot determine program name from argv[0]\n");
		return 1;
	}

	rc = parse_args(argc, argv);
	if (rc) {
		return rc;
	}

	ipp_init();
	scm_init_guile();

	scm_c_primitive_load("scheme/agent.scm");

	{
		SCM hostname;

		hostname = scm_c_lookup("hostname");

		if (scm_variable_bound_p(hostname)  == SCM_BOOL_T) {
			SCM hostname_val;
			size_t len = 0;
			char *hostname_cstr;

			hostname_val = scm_variable_ref(hostname);
			hostname_cstr = gh_scm2newstr(hostname_val, &len);

			printf("%s\n", hostname_cstr);

		} else {
			fprintf(stderr, "variable not bound\n");
			ipp_exit();
			return 0;
		}
	}

	ipp_exit();
	return 0;
}
