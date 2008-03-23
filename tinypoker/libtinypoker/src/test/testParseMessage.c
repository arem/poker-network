/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
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

#include "../main/tinypoker.h"
#include "test.h"

#include <string.h>

int 
main()
{
	ipp_message    *msg;
	int		i;

	msg = ipp_new_message();
	msg->type = MSG_IPP;
	msg->payload = strdup("IPP 2.0 TinyPoker/0.0.0");
	ipp_parse_msg(msg);

	for (i = 0; msg->parsed[i]; i++) {
		printf("[%d] '%s'\n", i, msg->parsed[i]);
	}

	assertNotNull("msg->parsed should not be NULL", msg->parsed);
	assertNotNull("msg->parsed[0] should not be NULL", msg->parsed[0]);
	assertStringEqual("First token should have been 'IPP'", "IPP", msg->parsed[0]);
	assertNotNull("msg->parsed[1] should not be NULL", msg->parsed[1]);
	assertStringEqual("Second token should have been '2.0'", "2.0", msg->parsed[1]);
	assertNotNull("msg->parsed[2] should not be NULL", msg->parsed[2]);
	assertStringEqual("Third token should have been 'TinyPoker/0.0.0'", "TinyPoker/0.0.0", msg->parsed[2]);
	assertNull("msg->parsed[3] should be NULL", msg->parsed[3]);

	ipp_free_message(msg);
	msg = NULL;
	return PASS;
}
