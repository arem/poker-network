# Copyright (C) 2007 Thomas Cort <code@member.fsf.org>
#
# This file is part of libtinypoker.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

SET (libdirs /usr/lib /usr/pkg/lib /usr/local/lib /usr/lib64 /usr/pkg/lib64 /usr/local/lib64 /usr/lib32 /usr/pkg/lib32 /usr/local/lib32)
SET (bindirs /usr/bin /usr/pkg/bin /usr/local/bin)
SET (incdirs /usr/include /usr/local/include /usr/pkg/include /usr/include/mysql /usr/local/include/mysql /usr/pkg/include/mysql /usr/include/poker-eval /usr/local/include/poker-eval /usr/pkg/include/poker-eval /usr/include/glib-2.0 /usr/local/include/glib-2.0 /usr/pkg/include/glib-2.0 /usr/include/gnet-2.0 /usr/local/include/gnet-2.0 /usr/pkg/include/gnet-2.0 /usr/lib/glib-2.0/include /usr/pkg/lib/glib-2.0/include /usr/local/lib/glib-2.0/include /usr/lib64/glib-2.0/include /usr/pkg/lib64/glib-2.0/include /usr/local/lib64/glib-2.0/include /usr/lib32/glib-2.0/include /usr/pkg/lib32/glib-2.0/include /usr/local/lib32/glib-2.0/include /usr/lib/gnet-2.0/include /usr/pkg/lib/gnet-2.0/include /usr/local/lib/gnet-2.0/include /usr/lib64/gnet-2.0/include /usr/pkg/lib64/gnet-2.0/include /usr/local/lib64/gnet-2.0/include /usr/lib32/gnet-2.0/include /usr/pkg/lib32/gnet-2.0/include /usr/local/lib32/gnet-2.0/include)

#############################################
# libgnet checks
#############################################

FIND_LIBRARY(LIBGNET gnet-2.0 ${libdirs})

IF (LIBGNET)
  MESSAGE(STATUS "Found libgnet-2.0: ${LIBGNET}")
ELSE (LIBGNET)
  MESSAGE (FATAL_ERROR "libgnet-2.0 not found.")
ENDIF (LIBGNET)

FIND_PATH(LIBGNET_INCLUDE_DIR gnet.h ${incdirs})

IF (LIBGNET_INCLUDE_DIR)
  MESSAGE(STATUS "Found gnet.h in ${LIBGNET_INCLUDE_DIR}")
ELSE (LIBGNET_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "gnet.h not found.")
ENDIF (LIBGNET_INCLUDE_DIR)

FIND_PATH(LIBGNET_INCLUDE_DIR2 gnetconfig.h ${incdirs})

IF (LIBGNET_INCLUDE_DIR2)
  MESSAGE(STATUS "Found gnetconfig.h in ${LIBGNET_INCLUDE_DIR2}")
ELSE (LIBGNET_INCLUDE_DIR2)
  MESSAGE (FATAL_ERROR "gnetconfig.h not found.")
ENDIF (LIBGNET_INCLUDE_DIR2)

#############################################
# libglib checks
#############################################

FIND_LIBRARY(LIBGLIB glib-2.0 ${libdirs})

IF (LIBGLIB)
  MESSAGE(STATUS "Found libglib-2.0: ${LIBGLIB}")
ELSE (LIBGLIB)
  MESSAGE (FATAL_ERROR "libglib-2.0 not found.")
ENDIF (LIBGLIB)

FIND_PATH(LIBGLIB_INCLUDE_DIR glib.h ${incdirs})

IF (LIBGLIB_INCLUDE_DIR)
  MESSAGE(STATUS "Found glib.h in ${LIBGLIB_INCLUDE_DIR}")
ELSE (LIBGLIB_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "glib.h not found.")
ENDIF (LIBGLIB_INCLUDE_DIR)

FIND_PATH(LIBGLIB_INCLUDE_DIR2 glibconfig.h ${incdirs})

IF (LIBGLIB_INCLUDE_DIR2)
  MESSAGE(STATUS "Found glibconfig.h in ${LIBGLIB_INCLUDE_DIR2}")
ELSE (LIBGLIB_INCLUDE_DIR2)
  MESSAGE (FATAL_ERROR "glibconfig.h not found.")
ENDIF (LIBGLIB_INCLUDE_DIR2)

#############################################
# libpthread checks
#############################################

FIND_LIBRARY(LIBPTHREAD pthread ${libdirs})

IF (LIBPTHREAD)
  MESSAGE(STATUS "Found libpthread: ${LIBPTHREAD}")
ELSE (LIBPTHREAD)
  MESSAGE (FATAL_ERROR "libpthread not found.")
ENDIF (LIBPTHREAD)

FIND_PATH(LIBPTHREAD_INCLUDE_DIR pthread.h ${incdirs})

IF (LIBPTHREAD_INCLUDE_DIR)
  MESSAGE(STATUS "Found pthread.h in ${LIBPTHREAD_INCLUDE_DIR}")
ELSE (LIBPTHREAD_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "pthread.h not found.")
ENDIF (LIBPTHREAD_INCLUDE_DIR)

#############################################
# Put everything together
#############################################

INCLUDE_DIRECTORIES(${incdirs} ${LIBGNET_INCLUDE_DIR} ${LIBGLIB_INCLUDE_DIR} ${LIBGNET_INCLUDE_DIR2} ${LIBGLIB_INCLUDE_DIR2} ${LIBPTHREAD_INCLUDE_DIR})
ADD_LIBRARY(tinypoker tinypoker.c)
LINK_LIBRARIES(tinypoker ${LIBGNET} ${LIBGLIB} ${LIBPTHREAD})
INSTALL(TARGETS tinypoker DESTINATION lib)
