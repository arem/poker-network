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

#include <tinypoker.h>

#include <stdlib.h>
#include <strings.h>

#include <libguile.h>
#include <guile/gh.h>

#include "main.h"
#include "glue.h"

ipp_socket *sock;

SCM tpb_ipp_connect(SCM s_hostname, SCM s_port, SCM s_name, SCM s_buyin)
{
	char *hostname;
	char *name;
	short port = 9898;
	char *buyin = "500";

	if (sock) {
		ipp_free_socket(sock);
		sock = NULL;
	}

	SCM_ASSERT(gh_string_p(s_hostname), s_hostname, SCM_ARG1, "ipp-connect");
	SCM_ASSERT(gh_string_p(s_name), s_name, SCM_ARG3, "ipp-connect");

	hostname = gh_scm2newstr(s_hostname, NULL);
	name = gh_scm2newstr(s_name, NULL);

	printf("Hostname => '%s'\n", hostname);
	printf("Name => '%s'\n", name);

	sock = ipp_client_handshake(hostname, port, name, buyin, NULL);

	if (hostname) {
		free(hostname);
		hostname = NULL;
	}

	if (name) {
		free(name);
		name = NULL;
	}

	return (sock != NULL) ? SCM_BOOL_T : SCM_BOOL_F;
}

SCM tpb_ipp_disconnect(void)
{
	if (sock) {
		ipp_free_socket(sock);
		sock = NULL;
	}

	return SCM_BOOL_T;
}

void tpb_register_procs(void)
{
	sock = NULL;

	gh_new_procedure("ipp-connect", tpb_ipp_connect, 4, 0, 0);
	gh_new_procedure("ipp-disconnect", tpb_ipp_disconnect, 0, 0, 0);
}
