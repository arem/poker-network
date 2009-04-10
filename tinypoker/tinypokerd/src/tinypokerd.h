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

#ifndef __TINYPOKERD_H
#define __TINYPOKERD_H

#define TINYPOKERD_NAME "tinypokerd"
#define TINYPOKERD_VERSION "0.0.0"

#include <tinypoker.h>

#if (LIBTINYPOKER_MAJOR_VERSION == 0 && LIBTINYPOKER_MINOR_VERSION < 1)
#error "libtinypoker version too old. Minimum version is 0.1.0"
#endif

#endif
