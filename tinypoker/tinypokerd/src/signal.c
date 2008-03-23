/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 * 
 * This file is part of tinypokerd.
 * 
 * tinypokerd is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * tinypokerd is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * tinypokerd.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libdaemon/dlog.h>
#include <signal.h>
#include <tinypoker.h>

#include "signal.h"

/**
 * Global variable used to determine if the program should halt. Will it ever halt? Only Alan Turing knows.
 * @see handle_signal()
 */
int		exit_now;

/**
 * Signal handler for SIGKILL
 * @param sig signal to handle.
 */
void
handle_sigkill(int sig)
{
	if (sig == SIGKILL) {
		raise(SIGUSR2);
		exit_now = 1;
		daemon_log(LOG_INFO, "[SIGN] SIGKILL Caught ; preparing to exit");
	}
}

/**
 * Signal handler for SIGQUIT
 * @param sig signal to handle.
 */
void
handle_sigquit(int sig)
{
	if (sig == SIGQUIT) {
		raise(SIGUSR2);
		exit_now = 1;
		daemon_log(LOG_INFO, "[SIGN] SIGQUIT Caught ; preparing to exit");
	}
}

/**
 * Signal handler for SIGINT
 * @param sig signal to handle.
 */
void
handle_sigint(int sig)
{
	if (sig == SIGINT) {
		raise(SIGUSR2);
		exit_now = 1;
		daemon_log(LOG_INFO, "[SIGN] SIGINT Caught ; preparing to exit");
	}
}

/**
 *  installs signal handlers (mostly SIG_IGN)
 */
void
install_signal_handlers()
{

	int		i;

	for (i = 0; i < 32; i++) {
		/*
		 * Set ignore for all signals except the 3 we handle
		 * (SIGKILL, SIGQUIT, SIGINT) and the 1 libtinypoker handles
		 * (SIGUSR2)
		 */

		if (i != SIGCHLD && i != SIGQUIT && i != SIGINT && i != SIGKILL && i != SIGUSR2) {
			signal(i, SIG_IGN);
		}
	}

	signal(SIGKILL, handle_sigkill);
	signal(SIGQUIT, handle_sigquit);
	signal(SIGINT, handle_sigint);
}
