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
# libgnet checks
#############################################

FIND_LIBRARY(LIBGNET gnet-2.0 ${libdirs})

IF (LIBGNET)
  MESSAGE(STATUS "Found libgnet-2.0: ${LIBGNET}")
ELSE (LIBGNET)
  MESSAGE (FATAL_ERROR "libgnet-2.0 not found.")
ENDIF (LIBGNET)

FIND_PATH(LIBGNET_INCLUDE_DIR gnet.h ${incdirs})

IF (LIBGNET_INCLUDE_DIR)
  MESSAGE(STATUS "Found gnet.h in ${LIBGNET_INCLUDE_DIR}")
ELSE (LIBGNET_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "gnet.h not found.")
ENDIF (LIBGNET_INCLUDE_DIR)

FIND_PATH(LIBGNET_INCLUDE_DIR2 gnetconfig.h ${incdirs})

IF (LIBGNET_INCLUDE_DIR2)
  MESSAGE(STATUS "Found gnetconfig.h in ${LIBGNET_INCLUDE_DIR2}")
ELSE (LIBGNET_INCLUDE_DIR2)
  MESSAGE (FATAL_ERROR "gnetconfig.h not found.")
ENDIF (LIBGNET_INCLUDE_DIR2)

#############################################
# libglib checks
#############################################

FIND_LIBRARY(LIBGLIB glib-2.0 ${libdirs})

IF (LIBGLIB)
  MESSAGE(STATUS "Found libglib-2.0: ${LIBGLIB}")
ELSE (LIBGLIB)
  MESSAGE (FATAL_ERROR "libglib-2.0 not found.")
ENDIF (LIBGLIB)

FIND_PATH(LIBGLIB_INCLUDE_DIR glib.h ${incdirs})

IF (LIBGLIB_INCLUDE_DIR)
  MESSAGE(STATUS "Found glib.h in ${LIBGLIB_INCLUDE_DIR}")
ELSE (LIBGLIB_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "glib.h not found.")
ENDIF (LIBGLIB_INCLUDE_DIR)

FIND_PATH(LIBGLIB_INCLUDE_DIR2 glibconfig.h ${incdirs})

IF (LIBGLIB_INCLUDE_DIR2)
  MESSAGE(STATUS "Found glibconfig.h in ${LIBGLIB_INCLUDE_DIR2}")
ELSE (LIBGLIB_INCLUDE_DIR2)
  MESSAGE (FATAL_ERROR "glibconfig.h not found.")
ENDIF (LIBGLIB_INCLUDE_DIR2)

#############################################
# Put everything together
#############################################

INCLUDE_DIRECTORIES(${incdirs} ${LIBGNET_INCLUDE_DIR} ${LIBGLIB_INCLUDE_DIR} ${LIBGNET_INCLUDE_DIR2} ${LIBGLIB_INCLUDE_DIR2})

INCLUDE(CTest)
ENABLE_TESTING()

ADD_EXECUTABLE(testValidateCmdIpp testValidateCmdIpp.c)
TARGET_LINK_LIBRARIES(testValidateCmdIpp tinypoker ${LIBGNET} ${LIBGLIB})
ADD_TEST(testValidateCmdIpp ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdIpp)

ADD_EXECUTABLE(testValidateCmdBuyin testValidateCmdBuyin.c)
TARGET_LINK_LIBRARIES(testValidateCmdBuyin tinypoker ${LIBGNET} ${LIBGLIB})
ADD_TEST(testValidateCmdBuyin ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdBuyin)

ADD_EXECUTABLE(testValidateCmdWelcome testValidateCmdWelcome.c)
TARGET_LINK_LIBRARIES(testValidateCmdWelcome tinypoker ${LIBGNET} ${LIBGLIB})
ADD_TEST(testValidateCmdWelcome ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdWelcome)

ADD_EXECUTABLE(testNormalizeMessage testNormalizeMessage.c)
TARGET_LINK_LIBRARIES(testNormalizeMessage tinypoker ${LIBGNET} ${LIBGLIB})
ADD_TEST(testNormalizeMessage ${EXECUTABLE_OUTPUT_PATH}/testNormalizeMessage)

