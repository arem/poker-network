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

SET (incdirs /usr/include /usr/local/include /usr/pkg/include /usr/i586-mingw32msvc/include /usr/i686-pc-mingw32/sys-root/mingw/include)

INCLUDE(FindPkgConfig)

PKG_CHECK_MODULES(GLIB2 glib-2.0>=2.16 REQUIRED)
PKG_CHECK_MODULES(GTHREAD2 gthread-2.0>=2.16 REQUIRED)

IF (WIN32)

#############################
## libwsock32
#############################

FIND_LIBRARY(LIBWSOCK32 wsock32)

IF (LIBWSOCK32)
  MESSAGE(STATUS "Found libwsock32: ${LIBWSOCK32}")
ELSE (LIBWSOCK32)
  MESSAGE (FATAL_ERROR "libwsock32 not found.")
ENDIF (LIBWSOCK32)
FIND_PATH(LIBWSOCK32_INCDIR winsock2.h ${incdirs})

IF (LIBWSOCK32_INCDIR)
  MESSAGE(STATUS "Found winsock2.h in ${LIBWSOCK32_INCDIR}")
ELSE (LIBWSOCK32_INCDIR)
  MESSAGE (FATAL_ERROR "winsock2.h not found.")
ENDIF (LIBWSOCK32_INCDIR)

#############################
## libws2_32
#############################

FIND_LIBRARY(LIBWS2_32 ws2_32)

IF (LIBWS2_32)
  MESSAGE(STATUS "Found libws2_32: ${LIBWS2_32}")
ELSE (LIBWS2_32)
  MESSAGE (FATAL_ERROR "libws2_32 not found.")
ENDIF (LIBWS2_32)

FIND_PATH(LIBWS2_32_INCDIR ws2tcpip.h ${incdirs})

FIND_PATH(LIBWS2_32_INCDIR ws2tcpip.h ${incdirs})

IF (LIBWS2_32_INCDIR)
  MESSAGE(STATUS "Found ws2tcpip.h in ${LIBWS2_32_INCDIR}")
ELSE (LIBWS2_32_INCDIR)
  MESSAGE (FATAL_ERROR "ws2tcpip.h not found.")
ENDIF (LIBWS2_32_INCDIR)

ENDIF (WIN32)


#############################################
# Put everything together
#############################################

IF (WIN32)
  INCLUDE_DIRECTORIES(${GTHREAD2_INCLUDE_DIRS} ${GLIB2_INCLUDE_DIRS} ${LIBWSOCK32_INCDIR} ${LIBWS2_32_INCDIR})
ELSE (WIN32)
  INCLUDE_DIRECTORIES(${GTHREAD2_INCLUDE_DIRS} ${GLIB2_INCLUDE_DIRS})
ENDIF (WIN32)

ADD_LIBRARY(tinypoker SHARED tinypoker.c)
LINK_LIBRARIES(tinypoker)

IF (WIN32)
  TARGET_LINK_LIBRARIES(tinypoker ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES} ${LIBWSOCK32} ${LIBWS2_32})
ELSE (WIN32)
  TARGET_LINK_LIBRARIES(tinypoker ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES})
ENDIF (WIN32)

INSTALL(TARGETS tinypoker DESTINATION lib)
INSTALL(FILES tinypoker.h DESTINATION include)
