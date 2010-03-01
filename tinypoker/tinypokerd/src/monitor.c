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

#define _XOPEN_SOURCE 500

#include <glib.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

#include "monitor.h"

/**
 * Counter for the number of running threads.
 */
int cnt;

/**
 * A lock used to serialize access to cnt.
 */
GMutex *mon_lock;

/**
 *  initialize monitor variables
 */
void monitor_init(void)
{
	mon_lock = g_mutex_new();
	cnt = 0;
}

/**
 *  increments the thread count
 */
void monitor_inc(void)
{
	g_mutex_lock(mon_lock);
	cnt++;
	g_mutex_unlock(mon_lock);
	g_debug("[MNTR] thread count incremented");
}

/**
 *  decrements the thread count
 */
void monitor_dec(void)
{
	g_mutex_lock(mon_lock);
	cnt--;
	g_mutex_unlock(mon_lock);
	g_debug("[MNTR] thread count decremented");
}

/**
 *  waits until no threads are running
 *  blocks new threads from being created
 */
void monitor_wait(void)
{
	g_debug("[MNTR] monitor_wait() called");

	while (1) {
		g_mutex_lock(mon_lock);

		g_debug("[MNTR] locked mon_lock");

		if (!cnt) {
			g_debug("[MNTR] cnt == 0 -- all threads stopped");
			g_mutex_unlock(mon_lock);
			g_mutex_free(mon_lock);
			mon_lock = NULL;
			return;
		} else {
			g_debug("[MNTR] cnt != 0");
			g_mutex_unlock(mon_lock);
			sched_yield();
#ifdef _WIN32
			Sleep(3000);
#else
			sleep(3);
#endif
		}
	}
}
