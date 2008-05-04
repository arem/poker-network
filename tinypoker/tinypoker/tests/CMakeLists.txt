# Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
#
# This file is part of TinyPoker.
#
# TinyPoker is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# TinyPoker is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with TinyPoker.  If not, see <http://www.gnu.org/licenses/>.

PROJECT(test C)

INCLUDE(CTest)
ENABLE_TESTING()

# pkg-config module for CMake
INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(LIBS REQUIRED glib-2.0>=2.16 gnet-2.0>=2.0)

INCLUDE_DIRECTORIES(../lib ${LIBS_INCLUDE_DIRS})
ADD_EXECUTABLE(test-version test-version.c)
SET_TARGET_PROPERTIES(test-version PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -ansi -pedantic -Wpointer-arith -Wstrict-prototypes -Wall -Werror")
TARGET_LINK_LIBRARIES(test-version tinypoker ${LIBS_LIBRARIES})
ADD_TEST(test-version ${EXECUTABLE_OUTPUT_PATH}/test-version)

