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

PKG_CHECK_MODULES(GLIB2 glib-2.0>=2.18 REQUIRED)
PKG_CHECK_MODULES(GTHREAD2 gthread-2.0>=2.18 REQUIRED)
PKG_CHECK_MODULES(GNUTLS gnutls>=2.4 REQUIRED)

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
# libpam checks
#############################################

FIND_LIBRARY(LIBPAM pam ${libdirs})

IF (LIBPAM)
  MESSAGE(STATUS "Found libpam: ${LIBPAM}")
ELSE (LIBPAM)
  MESSAGE (FATAL_ERROR "libpam not found. See: http://www.kernel.org/pub/linux/libs/pam/")
ENDIF (LIBPAM)

FIND_PATH(LIBPAM_INCLUDE_DIR security/pam_appl.h ${incdirs})

IF (LIBPAM_INCLUDE_DIR)
  MESSAGE(STATUS "Found security/pam_appl.h in ${LIBPAM_INCLUDE_DIR}")
ELSE (LIBPAM_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "security/pam_appl.h not found. See: http://www.kernel.org/pub/linux/libs/pam/")
ENDIF (LIBPAM_INCLUDE_DIR)

#############################################
# libxml2 checks
#############################################

INCLUDE(FindLibXml2)

IF (LIBXML2_FOUND)
  MESSAGE(STATUS "Found libxml2")
ELSE (LIBXML2_FOUND)
  MESSAGE (FATAL_ERROR "libxml2 not found. See: http://xmlsoft.org/")
ENDIF (LIBXML2_FOUND)

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

INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR} ${LIBDAEMON_INCLUDE_DIR} ${LIBCONFUSE_INCLUDE_DIR} ${LIBTINYPOKER_INCLUDE_DIR} ${LIBPTHREAD_INCLUDE_DIR} ${GNUTLS_INCLUDE_DIRS} ${GTHREAD2_INCLUDE_DIRS} ${GLIB2_INCLUDE_DIRS} ${LIBGCRYPT_INCDIR} ${LIBPAM_INCLUDE_DIR})
ADD_EXECUTABLE(tinypokerd config.c monitor.c signal.c tinypokerd.c pam.c poker.c pokerserv.c log.c)
SET_TARGET_PROPERTIES(tinypokerd PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -std=c99 -pedantic -Wpointer-arith -Wstrict-prototypes -Wall -Werror -g -O2")
TARGET_LINK_LIBRARIES(tinypokerd ${LIBDAEMON} ${LIBCONFUSE} ${LIBTINYPOKER} ${LIBPTHREAD} ${GNUTLS_LIBRARIES} ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES} ${LIBGCRYPT} ${LIBPAM} ${LIBXML2_LIBRARIES})

#############################################
# Installation
#############################################

INSTALL(PROGRAMS tinypokerd DESTINATION sbin PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
