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

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

PROJECT(libtinypoker C)

INCLUDE(InstallRequiredSystemLibraries)

# Version Information
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "2")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(VERSION "${CPACK_PACKAGE_VERSION}")
SET(SOVERSION "${CPACK_PACKAGE_VERSION}")

# Testing
INCLUDE(CTest)
ENABLE_TESTING()

ADD_SUBDIRECTORY(src)

# Packaging
SET(CPACK_GENERATOR "DEB;RPM")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Internet Poker Protocol (IPP) 2.0 Implementation")
SET(CPACK_PACKAGE_VENDOR "Thomas Cort")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_NAME "libtinypoker")
SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
SET(CPACK_PACKAGE_CONTACT "linuxgeek@gmail.com")

# DEB Packaging
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libglib2.0 (>= 2.16)")
SET(CPACK_DEBIAN_PACKAGE_BUILDS_DEPENDS "libglib2.0-dev (>= 2.16), cmake, pkg-config")
SET(CPACK_DEBIAN_PACKAGE_SECTION "devel")
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")

# RPM Packaging
SET(CPACK_RPM_PACKAGE_RELEASE "1")
SET(CPACK_RPM_PACKAGE_LICENSE "GPLv3+")
SET(CPACK_RPM_PACKAGE_ARCHITECTURE "i386")

INCLUDE(CPack)

FIND_PACKAGE(Doxygen)
IF (DOXYGEN_FOUND)
  MESSAGE(STATUS "doxygen found. Documentation generation enabled.")
  ADD_CUSTOM_TARGET(doc VERBATIM COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile")
ELSE (DOXYGEN_FOUND)
  MESSAGE(STATUS "doxygen not found. Documentation generation disabled.")  
ENDIF (DOXYGEN_FOUND)

# pkg-config
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/tinypoker.pc.in ${CMAKE_CURRENT_BINARY_DIR}/tinypoker.pc @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tinypoker.pc DESTINATION lib/pkgconfig)
