/*
 * Copyright (C) 2005, 2006 Thomas Cort
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

#include "sig.h"
#include "log.h"

/**
 *  void got_signal(int)
 */
void got_signal(int sig) {
	done = 1;

	switch (sig) {
		case SIGKILL:
			logit("[SGNL] SIGKILL");
			break;
		case SIGQUIT:
			logit("[SGNL] SIGQUIT");
			break;
		case SIGINT:
			logit("[SGNL] SIGINT");
			break;
	}
}

/**
 *  void install_signal_handlers()
 *
 *  installs signal handlers (mostly SIG_IGN)
 */
void install_signal_handlers() {
	int i;
	done = 0;

	for (i = 0; i < 32; i++)
		if (i != SIGCHLD && i != SIGQUIT && i != SIGINT && i != SIGKILL)
			signal(i,SIG_IGN);

	signal(SIGKILL,got_signal);
	signal(SIGINT,got_signal);
	signal(SIGQUIT,got_signal);
}
