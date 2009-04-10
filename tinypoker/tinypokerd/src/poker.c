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

#include <glib.h>

#include <stdlib.h>
#include <tinypoker.h>
#include <unistd.h>

#include "log.h"
#include "monitor.h"
#include "poker.h"
#include "signal.h"

ipp_table *tbl = NULL;

gpointer play(gpointer arg)
{

	do {
		/* shuffle before every hand and when no players are around */
		ipp_shuffle_deck(tbl->deck);

		/* must have 2 players */
		if (tbl->nplayers < 2) {
			g_mutex_unlock(tbl->lock);
			sleep(1);	/* sleep while we wait : constantly shuffling hogs the CPU */
			continue;
		}

		g_mutex_lock(tbl->lock);
		ipp_deal(tbl, SERVER_WRITE_TIMEOUT, protocol_logger);
		g_mutex_unlock(tbl->lock);

	} while (!exit_now);

	monitor_dec();
	return NULL;
}
