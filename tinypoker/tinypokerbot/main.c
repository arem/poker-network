/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010 Thomas Cort <linuxgeek@gmail.com>
 *
 * This file is part of TinyPoker.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <strings.h>
#include <getopt.h>
#include <tinypoker.h>

#include <libguile.h>
#include <guile/gh.h>

#include "main.h"
#include "glue.h"

/**
 * A command line poker bot.
 * @param argc The number of command line arguments coming in argv.
 * @param argv The command line arguments.
 * @return Returns 0 on success and non-zero when we want the program to terminate.
 */
int main(int argc, char **argv)
{
	if (argc < 1 || !argv || !argv[0]) {
		fprintf(stderr, "Cannot determine program name from argv[0]\n");
		return 1;
	}

	if (argc != 2 || !argv[1]) {
		fprintf(stderr, "Usage: %s /path/to/bot.scm\n", argv[0]);
		return 1;
	}

	ipp_init();
	scm_init_guile();
	tpb_register_procs();
	scm_c_primitive_load("scheme/bagelbot.scm");
	ipp_exit();

	return 0;
}
