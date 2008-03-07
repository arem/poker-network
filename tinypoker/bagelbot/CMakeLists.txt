#  BagelBot - Trivial client for pokerd
#  Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
# 
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
# 
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

PROJECT(bagelbot)

SET (libdirs /usr/lib /usr/local/lib /usr/lib32 /usr/local/lib32 /usr/lib64 /usr/local/lib64)

#############################################
# libdaemon checks
#############################################

FIND_LIBRARY(LIBDAEMON daemon ${libdirs})

IF (LIBDAEMON)
  MESSAGE(STATUS "Found libdaemon: ${LIBDAEMON}")
ELSE (LIBDAEMON)
  MESSAGE (FATAL_ERROR "libdaemon not found.")
ENDIF (LIBDAEMON)

FIND_PATH(LIBDAEMON_INCLUDE_DIR libdaemon/daemon.h /usr/include /usr/local/include)

IF (LIBDAEMON_INCLUDE_DIR)
  MESSAGE(STATUS "Found libdaemon/daemon.h in ${LIBDAEMON_INCLUDE_DIR}")
ELSE (LIBDAEMON_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "libdaemon/daemon.h not found.")
ENDIF (LIBDAEMON_INCLUDE_DIR)

#############################################
# libconfuse checks
#############################################

FIND_PATH(CONFUSE_INCLUDE_DIR confuse.h /usr/include /usr/local/include)

IF (CONFUSE_INCLUDE_DIR)
  MESSAGE(STATUS "Found confuse headers in ${CONFUSE_INCLUDE_DIR}")
ELSE (CONFUSE_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "confuse.h not found.")
ENDIF (CONFUSE_INCLUDE_DIR)

FIND_LIBRARY(LIBCONFUSE confuse ${libdirs})

IF (LIBCONFUSE)
  MESSAGE(STATUS "Found libconfuse: ${LIBCONFUSE}")
ELSE (LIBCONFUSE)
  MESSAGE (FATAL_ERROR "libconfuse not found.")
ENDIF (LIBCONFUSE)

#############################################
# Put everything together
#############################################

ADD_EXECUTABLE(bagelbot byte.c conf.c main.c net.c play.c)
TARGET_LINK_LIBRARIES(bagelbot ${LIBCONFUSE} ${LIBDAEMON})
INSTALL(PROGRAMS bagelbot DESTINATION usr/bin)
INSTALL(FILES bagelbot.conf DESTINATION etc PERMISSIONS OWNER_WRITE OWNER_READ)
