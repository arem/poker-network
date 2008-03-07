/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
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

#include "rand.h"

#include <libdaemon/dlog.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <stdio.h>
#include <gsl/gsl_rng.h>

/**
 * random number generator
 */
gsl_rng *r;

/**
 *  void rand_init()
 *
 *  initializes random number generator
 *  uses RANLUX algorithm which offers the best mathematically-proven quality
 */
void rand_init() {
	const gsl_rng_type *T;
	gsl_rng_env_setup();

	T = gsl_rng_ranlux;

	if ((r = gsl_rng_alloc(T)) == NULL) {
		daemon_log(LOG_INFO,"[RAND] %s",strerror(errno));
		exit(1);
	}
}

/**
 *  int rand_get(int n)
 *
 *  gets a random integer between 0 and n
 */
int rand_get(int n) {
	return gsl_rng_uniform_int(r,n);
}

/**
 *  void rand_destroy()
 *
 *  Frees all memory used by the random number generator
 */
void rand_destroy() {
	gsl_rng_free(r);
}
