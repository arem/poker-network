/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <linuxgeek@gmail.com>
 * 
 * This file is part of tinypokerd.
 * 
 * tinypokerd is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * tinypokerd is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * tinypokerd.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _XOPEN_SOURCE 500

#include <pthread.h>
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
pthread_mutex_t mon_lock;

/**
 *  initialize monitor variables
 */
void monitor_init(void)
{
	pthread_mutex_init(&mon_lock, 0);
	cnt = 0;
}

/**
 *  increments the thread count
 */
void monitor_inc(void)
{
	pthread_mutex_lock(&mon_lock);
	cnt++;
	pthread_mutex_unlock(&mon_lock);
}

/**
 *  decrements the thread count
 */
void monitor_dec(void)
{
	pthread_mutex_lock(&mon_lock);
	cnt--;
	pthread_mutex_unlock(&mon_lock);
}

/**
 *  waits until no threads are running
 *  blocks new threads from being created
 */
void monitor_wait(void)
{
	raise(SIGQUIT);

	while (1) {
		pthread_mutex_lock(&mon_lock);

		if (!cnt) {
			pthread_mutex_unlock(&mon_lock);
			pthread_mutex_destroy(&mon_lock);
			return;
		} else {
			pthread_mutex_unlock(&mon_lock);
			sched_yield();
			sleep(3);
		}
	}
}
