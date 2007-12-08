/*
 * Copyright (C) 2005, 2006, 2007 Thomas Cort <code@member.fsf.org>
 *
 * This file is part of tinypokerd.
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

#include <gnutls/gnutls.h>
#include <libdaemon/dlog.h>
#include <pthread.h>
#include <signal.h>
#include <tinypoker.h>

#include "config.h"
#include "poker.h"
#include "signal.h"

static void client_connect_callback(ipp_socket * sock)
{
	daemon_log(LOG_INFO, "New Connection");


	/* Add client socket to internal data structure here */
}

int pokerserv()
{
	pthread_t dealer_thread;
	pthread_attr_t dealer_thread_attr;

	/* create a thread to play the game */
	pthread_attr_init(&dealer_thread_attr);
	pthread_attr_setdetachstate(&dealer_thread_attr, PTHREAD_CREATE_DETACHED);
	monitor_inc();
	if (pthread_create(&dealer_thread, &dealer_thread_attr, play, NULL) != 0) {
		daemon_log(LOG_ERR, "Couldn't create dealer thread");
		monitor_dec();
		raise(SIGQUIT);
		return -1;
	}

	/* Start listening for connections */
	ipp_servloop(port, client_connect_callback);

	if (!exit_now) {
		/*
		 * we got here because someone raise()'d a SIGUSR2
		 * but not a SIGKILL, SIGQUIT, or SIGINT.
		 */
		raise(SIGQUIT);
	}

	return 0;
}
