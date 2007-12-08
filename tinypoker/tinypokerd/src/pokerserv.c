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
#include <signal.h>
#include <tinypoker.h>

#include "config.h"
#include "signal.h"

static void client_connect_callback(ipp_socket * sock)
{
	daemon_log(LOG_INFO, "New Connection");


	/* Add client socket to internal data structure here */
}

int pokerserv()
{
	ipp_init();
	ipp_servloop(port, client_connect_callback);
	ipp_exit();

	if (!exit_now) {
		/*
		 * we got here because someone raise()'d a SIGUSR2
		 * but not a SIGKILL, SIGQUIT, or SIGINT.
		 */
		raise(SIGQUIT);
	}

	return 0;
}
