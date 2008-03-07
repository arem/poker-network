/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This file is part of tinypokerd.
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

#ifndef __SIGNAL_H
#define __SIGNAL_H

/**
 * Signal handler for SIGKILL, SIGQUIT, SIGINT.
 * Raises a SIGUSR1 signal to tell nanohttp to stop.
 * @param sig signal to handle
 */
void handle_signal(int sig);

/**
 *  installs signal handlers (mostly SIG_IGN)
 */
void install_signal_handlers();

/**
 * Global variable used to determine if the program should halt. Will it ever halt? Only Alan Turing knows.
 * @see handle_signal()
 */
int exit_now;

#endif
