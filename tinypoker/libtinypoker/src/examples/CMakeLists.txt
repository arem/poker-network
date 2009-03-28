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

PKG_CHECK_MODULES(GLIB2 glib-2.0>=2.18 REQUIRED)
PKG_CHECK_MODULES(GTHREAD2 gthread-2.0>=2.18 REQUIRED)
PKG_CHECK_MODULES(GNUTLS gnutls>=2.4 REQUIRED)

#############################
## libgcrypt
#############################

FIND_LIBRARY(LIBGCRYPT gcrypt)

IF (LIBGCRYPT)
  MESSAGE(STATUS "Found libgcrypt: ${LIBGCRYPT}")
ELSE (LIBGCRYPT)
  MESSAGE (FATAL_ERROR "libgcrypt not found. See: http://www.gnupg.org/")
ENDIF (LIBGCRYPT)

FIND_PATH(LIBGCRYPT_INCDIR gcrypt.h ${incdirs})

IF (LIBGCRYPT_INCDIR)
  MESSAGE(STATUS "Found gcrypt.h in ${LIBGCRYPT_INCDIR}")
ELSE (LIBGCRYPT_INCDIR)
  MESSAGE (FATAL_ERROR "gcrypt.h not found. See: http://www.gnupg.org/")
ENDIF (LIBGCRYPT_INCDIR)

#############################################
# Put everything together
#############################################

INCLUDE_DIRECTORIES(${GNUTLS_INCLUDE_DIRS} ${GTHREAD2_INCLUDE_DIRS} ${GLIB2_INCLUDE_DIRS} ${LIBGCRYPT_INCDIR})
ADD_EXECUTABLE(client client.c)
TARGET_LINK_LIBRARIES(client tinypoker ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES} ${GNUTLS_LIBRARIES} ${LIBGCRYPT})
