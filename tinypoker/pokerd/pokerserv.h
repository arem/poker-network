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

#ifndef __POKERSERV_H
#define __POKERSERV_H

int pokermain(int port);

#include <pthread.h>

/**
 * used to block against multiple concurrent login attempts.
 */
pthread_mutex_t auth_lock;

/**
 * Attempts to authenticate a user connected to the socket void_sd.
 * If a read from void_sd doesn't yield a message of type JOIN_GAME or
 * the authentication fails, the user is disconnected. If everything is
 * ok, then the user stays connected and joins the games.
 * @param void_sd the socket descriptor of the current player.
 */
void authenticate(void *void_sd);

#endif
