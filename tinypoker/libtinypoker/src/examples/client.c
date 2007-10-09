/*
 * Copyright (C) 2007 Thomas Cort <code@member.fsf.org>
 *
 * This file is part of libtinypoker.
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

#include "../main/tinypoker.h"


int main(int argc, char **argv, char **envp) {
	GTcpSocket *socket;
	gchar* msg;
	gboolean rc;

	ipp_init();
	socket = ipp_connect("localhost", 9898);
	if (!socket) {
		g_printf("! connect failed\n");
		return 1;
	}

	msg = ipp_read_msg(socket, CLIENT_READ_TIMEOUT);
	if (msg) {
		g_printf("> %s\n", msg);
		g_free(msg);
	} else {
		g_printf("! read error\n");
		return 1;
	}

	msg = g_strdup("BUYIN TOM 500");
	rc = ipp_send_msg(socket, msg, CLIENT_WRITE_TIMEOUT);
	if (rc) {
		g_printf("< %s\n", msg);
		g_free(msg);
	} else {
		g_printf("! send error\n");
		g_free(msg);
		return 1;
	}

	msg = ipp_read_msg(socket, CLIENT_READ_TIMEOUT);
	if (msg) {
		g_printf("> %s\n", msg);
		g_free(msg);
	} else {
		g_printf("! read error\n");
		return 1;
	}

	if (socket) {
		gnet_tcp_socket_delete(socket);
		socket = NULL;
	}

	return 0;
}
