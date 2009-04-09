# Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <linuxgeek@gmail.com>
#
# This file is part of tinypokerd.
#
# tinypokerd is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# tinypokerd is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with tinypokerd.  If not, see <http://www.gnu.org/licenses/>.

SET (libdirs /lib /usr/lib /usr/pkg/lib /usr/local/lib /usr/lib64 /usr/local/lib64 /usr/lib32 /usr/local/lib32)
SET (bindirs /usr/bin /usr/pkg/bin /usr/local/bin)
SET (incdirs /usr/include /usr/local/include /usr/pkg/include /usr/i586-mingw32msvc/include /usr/i686-pc-mingw32/sys-root/mingw/include)

INCLUDE(FindPkgConfig)

PKG_CHECK_MODULES(GLIB2 glib-2.0>=2.16 REQUIRED)
PKG_CHECK_MODULES(GTHREAD2 gthread-2.0>=2.16 REQUIRED)
PKG_CHECK_MODULES(LIBTINYPOKER tinypoker>=0.2.0 REQUIRED)

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
# libxml2 checks
#############################################

INCLUDE(FindLibXml2)

IF (LIBXML2_FOUND)
  MESSAGE(STATUS "Found libxml2")
ELSE (LIBXML2_FOUND)
  MESSAGE (FATAL_ERROR "libxml2 not found. See: http://xmlsoft.org/")
ENDIF (LIBXML2_FOUND)

#############################################
# Put everything together
#############################################

INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR} ${LIBDAEMON_INCLUDE_DIR} ${LIBCONFUSE_INCLUDE_DIR} ${LIBTINYPOKER_INCLUDE_DIR} ${GTHREAD2_INCLUDE_DIRS} ${GLIB2_INCLUDE_DIRS})
ADD_EXECUTABLE(tinypokerd config.c monitor.c signal.c tinypokerd.c poker.c pokerserv.c log.c)
SET_TARGET_PROPERTIES(tinypokerd PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -std=c99 -pedantic -Wpointer-arith -Wstrict-prototypes -Wall -Werror -g -O2")
TARGET_LINK_LIBRARIES(tinypokerd ${LIBDAEMON} ${LIBCONFUSE} ${LIBTINYPOKER_LIBRARIES} ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES} ${LIBXML2_LIBRARIES})

#############################################
# Installation
#############################################

INSTALL(PROGRAMS tinypokerd DESTINATION sbin PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
