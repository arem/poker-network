/*
 * BagelBot - Trivial client for pokerd
 * Copyright (C) 2005, 2007 Thomas Cort <code@member.fsf.org>
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

#include "conf.h"
#include "play.h"
#include "net.h"
#include "byte.h"

int authenticate() {
        int type;
        struct byte_array *ba;

        ba = new_byte_array(32);

        byte_array_append_string(ba,user);	/* User */
        byte_array_append_string(ba,pass);	/* Pass */
        byte_array_append_int(ba,1);
        byte_array_append_string(ba,"BagelBot");/* User-agent */

        write_message(JOIN_GAME,ba);
        byte_array_destroy(ba);

        ba = read_message(&type);
        byte_array_destroy(ba);

        if (type == GOODPASS)
                return 1;
        else
                return 0;
}

int main() {
        /* Set the default config file path */
	configfile = (char*) malloc((strlen("/etc/bagelbot.conf") + 2) * sizeof(char));
	if (!configfile) {
		printf("malloc() failed!\n");
		return 255;
	}
	bzero(configfile,(strlen("/etc/bagelbot.conf") + 2) * sizeof(char));
	snprintf(configfile,strlen("/etc/bagelbot.conf") + 1,"/etc/bagelbot.conf");

	configure();

	if (!host || !pass || !port || !user) {
		printf("Could not determine one or more configuration setting.\n");
		free_config();
		return 255;
	} else {
		printf("Connecting to %s:%d as %s\n",host,port,user);
	}

        connect_to_server(host,port);

        if (authenticate()) {
                printf("Authenticated!\n");
                play();
        } else {
                printf("Not Authenticated!\n");
        }

        disconnect_from_server();
	free_config();
	return 0;
}
