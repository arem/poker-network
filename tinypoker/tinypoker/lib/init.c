/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This file is part of TinyPoker.
 *
 * TinyPoker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TinyPoker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TinyPoker.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <tinypoker/tinypoker.h>

/**
 * Initializes the tinypoker-network library.
 * Call this function at the beginning of your program.
 * The only tp_* function that should preceed this is tp_check_version().
 * If your program calls gtk_init(), call this function before.
 * There is no need to call gnet_init() since this function calls it.
 * @see tp_check_version()
 */
void tp_init(void) {
        gnet_init();
}
