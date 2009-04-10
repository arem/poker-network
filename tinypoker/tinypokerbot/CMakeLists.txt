# Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <tcort@tomcort.com>
#
# This file is part of TinyPoker.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

PROJECT(tinypokerbot C)

# Version Information
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(VERSION "${CPACK_PACKAGE_VERSION}")
SET(SOVERSION "${CPACK_PACKAGE_VERSION}")

SET (libdirs /usr/lib /usr/local/lib /usr/lib32 /usr/local/lib32 /usr/lib64 /usr/local/lib64)

INCLUDE(FindPkgConfig)

PKG_CHECK_MODULES(GLIB2 glib-2.0>=2.16 REQUIRED)
PKG_CHECK_MODULES(GTHREAD2 gthread-2.0>=2.16 REQUIRED)
PKG_CHECK_MODULES(LIBTINYPOKER tinypoker>=0.2.0 REQUIRED)
PKG_CHECK_MODULES(GUILE guile-1.8 REQUIRED)

#############################################
# Put everything together
#############################################

INCLUDE_DIRECTORIES(${LIBTINYPOKER_INCLUDE_DIR} ${GTHREAD2_INCLUDE_DIRS} ${GLIB2_INCLUDE_DIRS} ${GUILE_INCLUDE_DIR})
ADD_EXECUTABLE(tinypokerbot glue.c main.c)
SET_TARGET_PROPERTIES(tinypokerbot PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -std=c99 -pedantic -Wpointer-arith -Wstrict-prototypes -Wall -Werror -g -O2")
TARGET_LINK_LIBRARIES(tinypokerbot ${LIBTINYPOKER_LIBRARIES} ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES} ${GUILE_LIBRARIES})
INSTALL(PROGRAMS tinypokerbot DESTINATION bin)
INSTALL(FILES bagelbot.scm DESTINATION share/tinypokerbot)
