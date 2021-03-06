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
void install_signal_handlers(void);

/**
 * Global variable used to determine if the program should halt. Will it ever halt? Only Alan Turing knows.
 * @see handle_signal()
 */
int exit_now;

#endif
