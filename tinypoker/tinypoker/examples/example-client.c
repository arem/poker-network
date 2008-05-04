/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This file is part of TinyPoker.
 *
 * TinyPoker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TinyPoker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TinyPoker.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string.h>

#include <tinypoker/tinypoker.h>

/* Check the version of the header file */
#if (TP_CHECK_VERSION(0, 0, 0) == FALSE)
#error "tinypoker-network.h is too old"
#endif

int main(int argc, char *argv[]) {
	const gchar *err;
	const gchar *hostname;
	gint port;
	TPConn *conn;
/*	const GRegex *regex;
*/
	/* command line parameter checking */
	if (argc != 3 || argv == NULL || argv[0] == NULL || argv[1] == NULL || argv[2] == NULL) {
		if (argc >= 1 && argv[0]) {
			g_print("Usage:\n\t%s [hostname] [port]\n", argv[0]);
		} else {
			/* this really can happen if the program executed with execve and the programmer gave a bad argv paramter */
			g_print("Usage:\n\texample-client [hostname] [port]\n");
		}

		return 1;
	} else {
		/* attempt to parse hostname and port */
		hostname = argv[1];
		if (hostname == NULL || strlen(hostname) == 0) {
			/* this can really happen if the use does something like `./example-client "" 9898` or execve is use incorrectly */
			g_print("Hostname too short. It must be at least 1 character long.\n");
			return 1;
		} else if (strlen(hostname) > 255) {
			g_print("Hostname too long. Try something 255 characters or less.");
			return 1;
		}

		port = atoi(argv[2]);
		if (port < 1 || port > 65535) {
			g_print("Port number out of range\n");
			return 1;
		}
	}

	/* check the glib library version to make sure it is compatible */
	err = glib_check_version(GLIB_MAJOR_VERSION, GLIB_MINOR_VERSION, GLIB_MICRO_VERSION);
	if (err) {
		g_error(err);
	}

	/* check the tinypoker-network library version to make sure it is compatible */
	err = tp_check_version(TP_MAJOR_VERSION, TP_MINOR_VERSION, TP_MICRO_VERSION);
	if (err) {
		g_error(err);
	}

	/* This should be one of the first calls in your program. */
	/* The only other tp_* call before it should be tp_check_version() */
	tp_init();

/*
	regex = g_regex_new(const gchar *pattern, GRegexCompileFlags compile_options, GRegexMatchFlags match_options, GError **error);

*/

	conn = tp_conn_new(hostname, port);
	tp_conn_connect(conn);
	g_print("Is Connected? %d\n", tp_conn_is_connected(conn));
	tp_conn_disconnect(conn);
	tp_conn_unref(conn);

	return 0;
}
