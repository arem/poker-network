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

INCLUDE(FindPkgConfig)

PKG_CHECK_MODULES(GLIB2 glib-2.0>=2.18 REQUIRED)
PKG_CHECK_MODULES(GTHREAD2 gthread-2.0>=2.18 REQUIRED)
PKG_CHECK_MODULES(GNUTLS gnutls>=2.4 REQUIRED)

INCLUDE_DIRECTORIES(${GNUTLS_INCLUDE_DIRS} ${GTHREAD2_INCLUDE_DIR} ${GLIB2_INCLUDE_DIRS})

ADD_EXECUTABLE(client client.c)

IF (WIN32)
  TARGET_LINK_LIBRARIES(client tinypoker ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES} ${GNUTLS_LIBRARIES} wsock32 gcrypt)
ELSE (WIN32)
  TARGET_LINK_LIBRARIES(client tinypoker ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES} ${GNUTLS_LIBRARIES})
ENDIF (WIN32)
