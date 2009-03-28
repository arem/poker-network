# Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <linuxgeek@gmail.com>
#
# This file is part of tinypokerbot.
#
# tinypokerbot is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# tinypokerbot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with tinypokerbot.  If not, see <http://www.gnu.org/licenses/>.

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

PKG_CHECK_MODULES(GLIB2 glib-2.0>=2.18 REQUIRED)
PKG_CHECK_MODULES(GTHREAD2 gthread-2.0>=2.18 REQUIRED)
PKG_CHECK_MODULES(GNUTLS gnutls>=2.4 REQUIRED)
PKG_CHECK_MODULES(GUILE guile-1.8 REQUIRED)

#############################################
# libtinypoker checks
#############################################

FIND_LIBRARY(LIBTINYPOKER tinypoker ${libdirs})

IF (LIBTINYPOKER)
  MESSAGE(STATUS "Found libtinypoker: ${LIBTINYPOKER}")
ELSE (LIBTINYPOKER)
  MESSAGE (FATAL_ERROR "libtinypoker not found. See: http://tinypoker.org/")
ENDIF (LIBTINYPOKER)

FIND_PATH(LIBTINYPOKER_INCLUDE_DIR tinypoker.h ${incdirs})

IF (LIBTINYPOKER_INCLUDE_DIR)
  MESSAGE(STATUS "Found tinypoker.h in ${LIBTINYPOKER_INCLUDE_DIR}")
ELSE (LIBTINYPOKER_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "tinypoker.h not found. See: http://tinypoker.org/")
ENDIF (LIBTINYPOKER_INCLUDE_DIR)

#############################################
# Put everything together
#############################################

INCLUDE_DIRECTORIES(${LIBTINYPOKER_INCLUDE_DIR} ${GNUTLS_INCLUDE_DIRS} ${GTHREAD2_INCLUDE_DIRS} ${GLIB2_INCLUDE_DIRS} ${LIBGCRYPT_INCDIR} ${GUILE_INCLUDE_DIR})
ADD_EXECUTABLE(tinypokerbot main.c)
SET_TARGET_PROPERTIES(tinypokerbot PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -std=c99 -pedantic -Wpointer-arith -Wstrict-prototypes -Wall -Werror -g -O2")
TARGET_LINK_LIBRARIES(tinypokerbot ${LIBTINYPOKER} ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES} ${GNUTLS_LIBRARIES} ${GUILE_LIBRARIES} ${LIBGCRYPT})
INSTALL(PROGRAMS tinypokerbot DESTINATION usr/bin)
