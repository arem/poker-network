# Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
#
# This file is part of TinyPoker.
#
# TinyPoker is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# TinyPoker is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with TinyPoker.  If not, see <http://www.gnu.org/licenses/>.

PROJECT(libtinypoker C)
INCLUDE(InstallRequiredSystemLibraries)

# CPack Information -- leave this here even though we don't use CPack yet.
# pkg-config generator also uses some of this.
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Library for TinyPoker")
SET(CPACK_PACKAGE_VENDOR "Thomas Cort <tom@tomcort.com>")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/../COPYING")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../README")
SET(CPACK_SOURCE_GENERATOR "TGZ;TZ")

# Version Information
SET(PACKAGE_NAME "tinypoker")
SET(PACKAGE_REQUIRES "glib-2.0 gnet-2.0")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(VERSION "${CPACK_PACKAGE_VERSION}")
SET(SOVERSION "0")

# pkg-config module for CMake
INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(LIBS REQUIRED glib-2.0>=2.16 gnet-2.0>=2.0)

# build libtinypoker
INCLUDE_DIRECTORIES(. ${LIBS_INCLUDE_DIRS})
ADD_LIBRARY(tinypoker SHARED init.c network.c version.c)
SET_TARGET_PROPERTIES(tinypoker PROPERTIES SOVERSION "${SOVERSION}" COMPILE_FLAGS "${COMPILE_FLAGS} -ansi -pedantic -Wpointer-arith -Wstrict-prototypes -Wall -Werror")
LINK_LIBRARIES(tinypoker ${LIBS_LIBRARIES})

# Make .pc file for pkg-config
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/tinypoker.pc.in ${CMAKE_CURRENT_BINARY_DIR}/tinypoker.pc @ONLY)

# Installation
INSTALL(TARGETS tinypoker DESTINATION lib)
INSTALL(FILES tinypoker/network.h tinypoker/tinypoker.h tinypoker/version.h DESTINATION include/tinypoker)
INSTALL(FILES tinypoker.pc DESTINATION lib/pkgconfig)
