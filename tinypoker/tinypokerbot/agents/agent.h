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

#ifndef __AGENT_H
#define __AGENT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char *name;
	char *version;
	void *handle;
	void (*play)(ipp_socket *sock);
} tpb_agent;

#ifdef __cplusplus
}
#endif

#endif
