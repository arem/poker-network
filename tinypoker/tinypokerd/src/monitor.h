/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This file is part of tinypokerd.
 *
 * tinypokerd is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * tinypokerd is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with tinypokerd.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MONITOR_H
#define __MONITOR_H

/**
 *  initialize monitor variables
 */
void monitor_init();

/**
 *  increments the thread count
 */
void monitor_inc();

/**
 *  decrements the thread count
 */
void monitor_dec();

/**
 *  waits until no threads are running
 *  blocks new threads from being created
 */
void monitor_wait();

#endif
