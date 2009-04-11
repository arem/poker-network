/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <tcort@tomcort.com>
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

#include <signal.h>
#include <tinypoker.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>

#include "signal.h"
#include "tinypokerd.h"

/**
 * Global variable used to determine if the program should halt. Will it ever halt? Only Alan Turing knows.
 * @see handle_signal()
 */
int exit_now;

/**
 * Signal handler for SIGKILL
 * @param sig signal to handle.
 */
void handle_sigkill(int sig)
{
	if (sig == SIGKILL) {
		ipp_servloop_shutdown();
		exit_now = 1;
	}
}

/**
 * Signal handler for SIGQUIT
 * @param sig signal to handle.
 */
void handle_sigquit(int sig)
{
	if (sig == SIGQUIT) {
		ipp_servloop_shutdown();
		exit_now = 1;
	}
}

/**
 * Signal handler for SIGINT
 * @param sig signal to handle.
 */
void handle_sigint(int sig)
{
	if (sig == SIGINT) {
		ipp_servloop_shutdown();
		exit_now = 1;
	}
}

/**
 * Signal handler for SIGSEGV
 * @param sig signal to handle.
 */
void handle_sigsegv(int sig)
{
	struct utsname sys_uname;
	int rc;

	if (sig != SIGSEGV) {
		return;
	}

	fprintf(stderr, "=-=-=-=-=-=-=-=-=-=-=-=-=-= Segmentation fault =-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	fprintf(stderr, " \n");
	fprintf(stderr, "A fatal error occured causing this application to crash. Please report the\n");
	fprintf(stderr, "entire error (everything between the 'Segmentation fault' lines) to the\n");
	fprintf(stderr, "following developer: Thomas Cort <tcort@tomcort.com>\n");
	fprintf(stderr, " \n");
	fprintf(stderr, "%s version %s was compiled at %s on %s\n", TINYPOKERD_NAME, TINYPOKERD_VERSION, __TIME__, __DATE__);

	rc = uname(&sys_uname);
	if (rc != -1) {
		fprintf(stderr, "%s version %s %s %s", sys_uname.sysname, sys_uname.version, sys_uname.release, sys_uname.machine);
	}

	fprintf(stderr, "\n\n");
	fprintf(stderr, "=-=-=-=-=-=-=-=-=-=-=-=-=-= Segmentation fault =-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	exit(1);
}

/**
 *  installs signal handlers (mostly SIG_IGN)
 */
void install_signal_handlers(void)
{

	int i;

	for (i = 0; i < 32; i++) {
		/*
		 * Set ignore for all signals except the 3 we handle
		 * (SIGKILL, SIGQUIT, SIGINT),
		 */

		if (i != SIGCHLD && i != SIGQUIT && i != SIGINT && i != SIGKILL && i != SIGSEGV) {
			signal(i, SIG_IGN);
		}
	}

	signal(SIGKILL, handle_sigkill);
	signal(SIGQUIT, handle_sigquit);
	signal(SIGINT, handle_sigint);
	signal(SIGSEGV, handle_sigsegv);
}
