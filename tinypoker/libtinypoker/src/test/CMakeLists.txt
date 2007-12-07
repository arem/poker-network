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
# Put everything together
#############################################

INCLUDE(CTest)
ENABLE_TESTING()

ADD_EXECUTABLE(testValidateCmdIpp testValidateCmdIpp.c)
TARGET_LINK_LIBRARIES(testValidateCmdIpp tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdIpp ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdIpp)

ADD_EXECUTABLE(testValidateCmdBuyin testValidateCmdBuyin.c)
TARGET_LINK_LIBRARIES(testValidateCmdBuyin tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdBuyin ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdBuyin)

ADD_EXECUTABLE(testValidateCmdWelcome testValidateCmdWelcome.c)
TARGET_LINK_LIBRARIES(testValidateCmdWelcome tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdWelcome ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdWelcome)

ADD_EXECUTABLE(testValidateCmdNewgame testValidateCmdNewgame.c)
TARGET_LINK_LIBRARIES(testValidateCmdNewgame tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdNewgame ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdNewgame)

ADD_EXECUTABLE(testNormalizeMessage testNormalizeMessage.c)
TARGET_LINK_LIBRARIES(testNormalizeMessage tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testNormalizeMessage ${EXECUTABLE_OUTPUT_PATH}/testNormalizeMessage)

