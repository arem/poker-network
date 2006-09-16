/*
 * Copyright (C) 2005, 2006 Thomas Cort
 *
 * This file is part of pokerd.
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

#define _XOPEN_SOURCE 500

#include <pthread.h>
#include <unistd.h>

#include "log.h"
#include "monitor.h"

extern void pthread_yield();

int cnt;
pthread_mutex_t mon_lock;

/**
 *  monitor_init()
 *
 *  initialize monitor variables
 */
void monitor_init() {
        pthread_mutex_init(&mon_lock,0);
	cnt = 0;
}

/**
 *  void monitor_inc()
 *
 *  increments the thread count
 */
void monitor_inc() {
	pthread_mutex_lock(&mon_lock);
	cnt++;
	pthread_mutex_unlock(&mon_lock);
}

/**
 *  void monitor_dec()
 *
 *  decrements the thread count
 */
void monitor_dec() {
	pthread_mutex_lock(&mon_lock);
	cnt--;
	pthread_mutex_unlock(&mon_lock);
}

/**
 *  void monitor_wait()
 *
 *  waits until no threads are running
 *  blocks new threads from being created
 */
void monitor_wait() {
	while(1) {
		pthread_mutex_lock(&mon_lock);

		logit("[WAIT] Threads need to die");

		if (!cnt) {
			/* Do NOT release lock; we don't want any more threads starting */
			return;
		} else {
			pthread_mutex_unlock(&mon_lock);
			pthread_yield();
			sleep(1);
		}
	}
}
