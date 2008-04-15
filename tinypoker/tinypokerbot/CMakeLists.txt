# Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
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

PROJECT(tinypokerbot C)

# Version Information
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(VERSION "${CPACK_PACKAGE_VERSION}")
ADD_DEFINITIONS("-DVERSION=\\\"${VERSION}\\\"")

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
# libgsl checks
#############################################

FIND_LIBRARY(LIBGSL gsl ${libdirs})

IF (LIBGSL)
  MESSAGE(STATUS "Found libgsl: ${LIBGSL}")
ELSE (LIBGSL)
  MESSAGE (FATAL_ERROR "libgsl not found.")
ENDIF (LIBGSL)

FIND_LIBRARY(LIBGSLCBLAS gslcblas ${libdirs})

IF (LIBGSLCBLAS)
  MESSAGE(STATUS "Found libgslcblas: ${LIBGSLCBLAS}")
ELSE (LIBGSLCBLAS)
  MESSAGE (FATAL_ERROR "libgslcblas not found.")
ENDIF (LIBGSLCBLAS)

FIND_PATH(LIBGSL_INCLUDE_DIR gsl/gsl_rng.h ${incdirs})

IF (LIBGSL_INCLUDE_DIR)
  MESSAGE(STATUS "Found gsl/gsl_rng.h in ${LIBGSL_INCLUDE_DIR}")
ELSE (LIBGSL_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "gsl/gsl_rng.h not found.")
ENDIF (LIBGSL_INCLUDE_DIR)

#############################################
# libdl checks
#############################################

FIND_LIBRARY(LIBDL dl ${libdirs})

IF (LIBDL)
  MESSAGE(STATUS "Found libdl: ${LIBDL}")
ELSE (LIBDL)
  MESSAGE (FATAL_ERROR "libdl not found. See: http://www.gnu.org/software/libc/")
ENDIF (LIBDL)

FIND_PATH(LIBDL_INCLUDE_DIR dlfcn.h ${incdirs})

IF (LIBDL_INCLUDE_DIR)
  MESSAGE(STATUS "Found dlfcn.h in ${LIBDL_INCLUDE_DIR}")
ELSE (LIBDL_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "dlfcn.h not found. See: http://www.gnu.org/software/libc/")
ENDIF (LIBDL_INCLUDE_DIR)

#############################################
# libbfd checks
#############################################

FIND_LIBRARY(LIBBFD bfd ${libdirs})

IF (LIBBFD)
  MESSAGE(STATUS "Found libbfd: ${LIBBFD}")
ELSE (LIBBFD)
  MESSAGE (FATAL_ERROR "libbfd not found. See: http://www.gnu.org/software/binutils/")
ENDIF (LIBBFD)

FIND_PATH(LIBBFD_INCLUDE_DIR bfd.h ${incdirs})

IF (LIBBFD_INCLUDE_DIR)
  MESSAGE(STATUS "Found bfd.h in ${LIBBFD_INCLUDE_DIR}")
ELSE (LIBBFD_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "bfd.h not found. See: http://www.gnu.org/software/binutils/")
ENDIF (LIBBFD_INCLUDE_DIR)

#############################################
# Put everything together
#############################################

INCLUDE_DIRECTORIES(${LIBDAEMON_INCLUDE_DIR} ${LIBCONFUSE_INCLUDE_DIR} ${LIBTINYPOKER_INCLUDE_DIR} ${LIBGNUTLS_INCLUDE_DIR} ${LIBPTHREAD_INCLUDE_DIR} ${LIBGSL_INCLUDE_DIR} ${LIBDL_INCLUDE_DIR} ${LIBBFD_INCLUDE_DIR})
ADD_EXECUTABLE(tinypokerbot conf.c main.c play.c signal.c)
SET_TARGET_PROPERTIES(tinypokerbot PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -std=c99 -pedantic -Wpointer-arith -Wstrict-prototypes -Wall -Werror -g -O2")
TARGET_LINK_LIBRARIES(tinypokerbot ${LIBCONFUSE} ${LIBDAEMON} ${LIBTINYPOKER} ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSL} ${LIBGSLCBLAS} ${LIBDL} ${LIBBFD})
INSTALL(PROGRAMS tinypokerbot DESTINATION usr/bin)
INSTALL(FILES tinypokerbot.conf DESTINATION etc PERMISSIONS OWNER_WRITE OWNER_READ)
