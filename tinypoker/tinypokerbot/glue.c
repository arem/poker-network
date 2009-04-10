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

#include <tinypoker.h>

#include <stdlib.h>
#include <strings.h>

#include <libguile.h>
#include <guile/gh.h>

#include "main.h"
#include "glue.h"

SCM tpb_ipp_handshake(SCM s_hostname, SCM s_port, SCM s_name, SCM s_buyin)
{
	char *hostname;
	char *name;

	SCM_ASSERT(gh_string_p(s_hostname), s_hostname, SCM_ARG1, "ipp-handshake");

	hostname = gh_scm2newstr(s_hostname, NULL);
	name = gh_scm2newstr(s_name, NULL);

	printf("Hostname => '%s'\n",hostname);
	printf("Name => '%s'\n",name);

//	ipp_client_handshake(hostname, port, name, buyin, NULL);

	if (hostname) {
		free(hostname);
		hostname = NULL;
	}

	if (name) {
		free(name);
		name = NULL;
	}
        return SCM_BOOL_T;
}

void tpb_register_procs(void)
{
    gh_new_procedure("ipp-handshake", tpb_ipp_handshake, 4, 0, 0);
}
