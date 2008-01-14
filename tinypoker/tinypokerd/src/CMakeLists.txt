# Copyright (C) 2005, 2006, 2007 Thomas Cort <code@member.fsf.org>
#
# This file is part of tinypokerd.
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

SET (libdirs /usr/lib /usr/pkg/lib /usr/local/lib /usr/lib64 /usr/local/lib64 /usr/lib32 /usr/local/lib32)
SET (bindirs /usr/bin /usr/pkg/bin /usr/local/bin)
SET (incdirs /usr/include /usr/local/include /usr/pkg/include)

#############################################
# libpthread checks
#############################################

FIND_LIBRARY(LIBPTHREAD pthread ${libdirs})

IF (LIBPTHREAD)
  MESSAGE(STATUS "Found libpthread: ${LIBPTHREAD}")
ELSE (LIBPTHREAD)
  MESSAGE (FATAL_ERROR "libpthread not found. See: http://www.gnu.org/software/libc/")
ENDIF (LIBPTHREAD)

FIND_PATH(LIBPTHREAD_INCLUDE_DIR pthread.h ${incdirs})

IF (LIBPTHREAD_INCLUDE_DIR)
  MESSAGE(STATUS "Found pthread.h in ${LIBPTHREAD_INCLUDE_DIR}")
ELSE (LIBPTHREAD_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "pthread.h not found. See: http://www.gnu.org/software/libc/")
ENDIF (LIBPTHREAD_INCLUDE_DIR)

#############################################
# libdaemon checks
#############################################

FIND_LIBRARY(LIBDAEMON daemon ${libdirs})

IF (LIBDAEMON)
  MESSAGE(STATUS "Found libdaemon: ${LIBDAEMON}")
ELSE (LIBDAEMON)
  MESSAGE (FATAL_ERROR "libdaemon not found. See: http://0pointer.de/lennart/projects/libdaemon/")
ENDIF (LIBDAEMON)

FIND_PATH(LIBDAEMON_INCLUDE_DIR libdaemon/daemon.h ${incdirs})

IF (LIBDAEMON_INCLUDE_DIR)
  MESSAGE(STATUS "Found libdaemon/daemon.h in ${LIBDAEMON_INCLUDE_DIR}")
ELSE (LIBDAEMON_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "libdaemon/daemon.h not found. See: http://0pointer.de/lennart/projects/libdaemon/")
ENDIF (LIBDAEMON_INCLUDE_DIR)

#############################################
# libconfuse checks
#############################################

FIND_LIBRARY(LIBCONFUSE confuse ${libdirs})

IF (LIBCONFUSE)
  MESSAGE(STATUS "Found libconfuse: ${LIBCONFUSE}")
ELSE (LIBCONFUSE)
  MESSAGE (FATAL_ERROR "libconfuse not found. See: http://www.nongnu.org/confuse/")
ENDIF (LIBCONFUSE)

FIND_PATH(LIBCONFUSE_INCLUDE_DIR confuse.h ${incdirs})

IF (LIBCONFUSE_INCLUDE_DIR)
  MESSAGE(STATUS "Found confuse.h in ${LIBCONFUSE_INCLUDE_DIR}")
ELSE (LIBCONFUSE_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "confuse.h not found. See: http://www.nongnu.org/confuse/")
ENDIF (LIBCONFUSE_INCLUDE_DIR)

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
# libgnutls checks
#############################################

FIND_LIBRARY(LIBGNUTLS gnutls ${libdirs})

IF (LIBGNUTLS)
  MESSAGE(STATUS "Found libgnutls: ${LIBGNUTLS}")
ELSE (LIBGNUTLS)
  MESSAGE (FATAL_ERROR "libgnutls not found. See: http://www.gnu.org/software/gnutls/")
ENDIF (LIBGNUTLS)

FIND_PATH(LIBGNUTLS_INCLUDE_DIR gnutls/gnutls.h ${incdirs})

IF (LIBGNUTLS_INCLUDE_DIR)
  MESSAGE(STATUS "Found gnutls/gnutls.h in ${LIBGNUTLS_INCLUDE_DIR}")
ELSE (LIBGNUTLS_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "gnutls/gnutls.h not found. See: http://www.gnu.org/software/gnutls/")
ENDIF (LIBGNUTLS_INCLUDE_DIR)

#############################################
# libsqlite3 checks
#############################################

FIND_LIBRARY(LIBSQLITE sqlite3 ${libdirs})

IF (LIBSQLITE)
  MESSAGE(STATUS "Found libsqlite3: ${LIBSQLITE}")
ELSE (LIBSQLITE)
  MESSAGE (FATAL_ERROR "libsqlite3 not found. See: http://sqlite.org/")
ENDIF (LIBSQLITE)

FIND_PATH(LIBSQLITE_INCLUDE_DIR sqlite3.h ${incdirs})

IF (LIBSQLITE_INCLUDE_DIR)
  MESSAGE(STATUS "Found sqlite3.h in ${LIBSQLITE_INCLUDE_DIR}")
ELSE (LIBSQLITE_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "sqlite3.h not found. See: http://sqlite.org/")
ENDIF (LIBSQLITE_INCLUDE_DIR)

#############################################
# Put everything together
#############################################

INCLUDE_DIRECTORIES(${incdirs} ${LIBDAEMON_INCLUDE_DIR} ${LIBCONFUSE_INCLUDE_DIR} ${LIBTINYPOKER_INCLUDE_DIR} ${LIBPTHREAD_INCLUDE_DIR} ${LIBGNUTLS_INCLUDE_DIR} ${LIBSQLITE_INCLUDE_DIR})
ADD_EXECUTABLE(tinypokerd config.c db.c deck.c monitor.c signal.c tinypokerd.c poker.c pokerserv.c)
TARGET_LINK_LIBRARIES(tinypokerd ${LIBDAEMON} ${LIBCONFUSE} ${LIBTINYPOKER} ${LIBPTHREAD} ${LIBGNUTLS} ${LIBSQLITE})

#############################################
# Installation
#############################################

INSTALL(PROGRAMS tinypokerd DESTINATION sbin PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)