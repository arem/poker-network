# Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
#
# This file is part of pokerd.
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

PROJECT(pokerd C)

INCLUDE(InstallRequiredSystemLibraries)

# Version Information
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "3")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(VERSION "${CPACK_PACKAGE_VERSION}")
ADD_DEFINITIONS("-DVERSION=\\\"${VERSION}\\\"")

SET (libdirs /usr/lib /usr/pkg/lib /usr/local/lib /usr/lib64 /usr/local/lib64 /usr/lib32 /usr/local/lib32)
SET (bindirs /usr/bin /usr/pkg/bin /usr/local/bin)
SET (incdirs /usr/include /usr/local/include /usr/pkg/include /usr/include/mysql /usr/local/include/mysql /usr/pkg/include/mysql /usr/include/poker-eval /usr/local/include/poker-eval /usr/pkg/include/poker-eval)

#############################################
# libdaemon checks
#############################################

FIND_LIBRARY(LIBDAEMON daemon ${libdirs})

IF (LIBDAEMON)
  MESSAGE(STATUS "Found libdaemon: ${LIBDAEMON}")
ELSE (LIBDAEMON)
  MESSAGE (FATAL_ERROR "libdaemon not found.")
ENDIF (LIBDAEMON)

FIND_PATH(LIBDAEMON_INCLUDE_DIR libdaemon/daemon.h ${incdirs})

IF (LIBDAEMON_INCLUDE_DIR)
  MESSAGE(STATUS "Found libdaemon/daemon.h in ${LIBDAEMON_INCLUDE_DIR}")
ELSE (LIBDAEMON_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "libdaemon/daemon.h not found.")
ENDIF (LIBDAEMON_INCLUDE_DIR)

#############################################
# libconfuse checks
#############################################

FIND_LIBRARY(LIBCONFUSE confuse ${libdirs})

IF (LIBCONFUSE)
  MESSAGE(STATUS "Found libconfuse: ${LIBCONFUSE}")
ELSE (LIBCONFUSE)
  MESSAGE (FATAL_ERROR "libconfuse not found.")
ENDIF (LIBCONFUSE)

FIND_PATH(LIBCONFUSE_INCLUDE_DIR confuse.h ${incdirs})

IF (LIBCONFUSE_INCLUDE_DIR)
  MESSAGE(STATUS "Found confuse.h in ${LIBCONFUSE_INCLUDE_DIR}")
ELSE (LIBCONFUSE_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "confuse.h not found.")
ENDIF (LIBCONFUSE_INCLUDE_DIR)

#############################################
# libpoker-eval checks
#############################################

FIND_LIBRARY(LIBPOKEREVAL poker-eval ${libdirs})

IF (LIBPOKEREVAL)
  MESSAGE(STATUS "Found libpoker-eval: ${LIBPOKEREVAL}")
ELSE (LIBPOKEREVAL)
  MESSAGE (FATAL_ERROR "libpoker-eval not found.")
ENDIF (LIBPOKEREVAL)

FIND_PATH(LIBPOKEREVAL_INCLUDE_DIR poker-eval/inlines/eval.h ${incdirs})

IF (LIBPOKEREVAL_INCLUDE_DIR)
  MESSAGE(STATUS "Found poker-eval/inlines/eval.h in ${LIBPOKEREVAL_INCLUDE_DIR}")
ELSE (LIBPOKEREVAL_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "poker-eval/inlines/eval.h not found.")
ENDIF (LIBPOKEREVAL_INCLUDE_DIR)

FIND_PATH(LIBPOKEREVAL_INCLUDE_DIR2 pokereval_export.h ${incdirs})

IF (LIBPOKEREVAL_INCLUDE_DIR2)
  MESSAGE(STATUS "Found pokereval_export.h in ${LIBPOKEREVAL_INCLUDE_DIR2}")
ELSE (LIBPOKEREVAL_INCLUDE_DIR2)
  MESSAGE (FATAL_ERROR "pokereval_export.h not found.")
ENDIF (LIBPOKEREVAL_INCLUDE_DIR2)

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
  MESSAGE(STATUS "Found libgslcblas: ${LIBGSL}")
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
# libmysqlclient checks
#############################################

FIND_LIBRARY(LIBMYSQLCLIENT mysqlclient ${libdirs})

IF (LIBMYSQLCLIENT)
  MESSAGE(STATUS "Found libmysqlclient: ${LIBMYSQLCLIENT}")
ELSE (LIBMYSQLCLIENT)
  MESSAGE (FATAL_ERROR "libmysqlclient not found.")
ENDIF (LIBMYSQLCLIENT)

FIND_PATH(LIBMYSQLCLIENT_INCLUDE_DIR mysql/mysql.h ${incdirs})

IF (LIBMYSQLCLIENT_INCLUDE_DIR)
  MESSAGE(STATUS "Found mysql/mysql.h in ${LIBMYSQLCLIENT_INCLUDE_DIR}")
ELSE (LIBMYSQLCLIENT_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "mysql/mysql.h not found.")
ENDIF (LIBMYSQLCLIENT_INCLUDE_DIR)

#############################################
# libgsl checks
#############################################

FIND_LIBRARY(LIBSSL ssl ${libdirs})

IF (LIBSSL)
  MESSAGE(STATUS "Found libssl: ${LIBSSL}")
ELSE (LIBSSL)
  MESSAGE (FATAL_ERROR "libssl not found.")
ENDIF (LIBSSL)

FIND_PATH(LIBSSL_INCLUDE_DIR openssl/md5.h ${incdirs})

IF (LIBSSL_INCLUDE_DIR)
  MESSAGE(STATUS "Found openssl/md5.h in ${LIBSSL_INCLUDE_DIR}")
ELSE (LIBSSL_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "openssl/md5.h not found.")
ENDIF (LIBSSL_INCLUDE_DIR)

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

INCLUDE_DIRECTORIES(${incdirs} ${LIBDAEMON_INCLUDE_DIR} ${LIBCONFUSE_INCLUDE_DIR} ${LIBPOKEREVAL_INCLUDE_DIR} ${LIBPOKEREVAL_INCLUDE_DIR2} ${LIBGSL_INCLUDE_DIR} ${LIBMYSQLCLIENT_INCLUDE_DIR} ${LIBSSL_INCLUDE_DIR} ${LIBPTHREAD_INCLUDE_DIR})
ADD_EXECUTABLE(pokerd byte.c conf.c daemon.c db.c deck.c eval.c main.c monitor.c net.c poker.c pokerserv.c rand.c sig.c)
TARGET_LINK_LIBRARIES(pokerd ${LIBDAEMON} ${LIBCONFUSE} ${LIBPOKEREVAL} ${LIBGSL} ${LIBGSLCBLAS} ${LIBMYSQLCLIENT} ${LIBSSL} ${LIBPTHREAD})

#############################################
# Installation
#############################################

INSTALL(PROGRAMS pokerd DESTINATION sbin PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
