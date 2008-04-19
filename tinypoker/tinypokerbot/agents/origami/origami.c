/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 * 
 * This file is part of tinypokerbot.
 * 
 * tinypokerbot is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * tinypokerbot is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * tinypokerbot.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE

#include <libdaemon/dlog.h>
#include <string.h>
#include <strings.h>
#include <tinypoker.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "agent.h"

void play(ipp_socket * sock) {
}

void agent_init(tpb_agent *a) {
	a->name = strdup("origami");
	a->version = strdup("0.0.0");
	a->play = play;
}

void agent_exit(tpb_agent *a) {
	if (a->name) {
		free(a->name);
		a->name = NULL;
	}
	if (a->version) {
		free(a->version);
		a->version = NULL;
	}

	a->play = NULL;
}
