/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <linuxgeek@gmail.com>
 * 
 * This file is part of libtinypoker.
 * 
 * libtinypoker is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * libtinypoker is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * libtinypoker.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE

#include "../main/tinypoker.h"
#include <string.h>

void logger(char *msg)
{
	if (msg) {
		printf("%s\n", msg);
	}
}

int main(int argc, char **argv, char **envp)
{
	ipp_socket *sock;
	ipp_init();

	sock = ipp_client_handshake("localhost", "ca.pem", "JSMITH", "ABC123", "500", logger);
	if (sock) {
		printf("- HANDSHAKE OK\n");
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
	} else {
		printf("- HANDSHAKE FAILED\n");
	}

	ipp_exit();
	return 0;
}
