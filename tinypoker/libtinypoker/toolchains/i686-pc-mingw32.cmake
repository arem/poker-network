#!/bin/sh
# Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <linuxgeek@gmail.com>
#
# This file is part of libtinypoker.
#
# libtinypoker is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libtinypoker is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with libtinypoker.  If not, see <http://www.gnu.org/licenses/>.

set (CMAKE_SYSTEM_NAME Windows)
set (CMAKE_C_COMPILER i686-pc-mingw32-gcc)
set (CMAKE_CXX_COMPILER i686-pc-mingw32-g++)
set (CMAKE_FIND_ROOT_PATH /usr/i686-pc-mingw32/sys-root/mingw/)
set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set (CMAKE_INSTALL_PREFIX /usr/i686-pc-mingw32/sys-root/mingw/)
set (RUN_TESTS_WITH_WINE 1)
