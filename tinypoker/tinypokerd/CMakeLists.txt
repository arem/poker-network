# Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010 Thomas Cort <linuxgeek@gmail.com>
#
# This file is part of TinyPoker.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

PROJECT(tinypokerd C)

# Version Information
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(VERSION "${CPACK_PACKAGE_VERSION}")

ADD_SUBDIRECTORY(src)

#############################################
# Packaging
#############################################

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "TinyPoker Server")
SET(CPACK_PACKAGE_VENDOR "Thomas Cort")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_NAME "tinypokerd")
SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
SET(CPACK_PACKAGE_CONTACT "linuxgeek@gmail.com")

SET(CPACK_GENERATOR "DEB;RPM")

# DEB Packaging
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libtinypoker (>= 0.2.0), libglib2.0-0 (>= 2.16)")
SET(CPACK_DEBIAN_PACKAGE_BUILDS_DEPENDS "libtinypoker (>= 0.2.0), libglib2.0-dev (>= 2.16), cmake, pkg-config")
SET(CPACK_DEBIAN_PACKAGE_SECTION "devel")
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")

# RPM Packaging
SET(CPACK_RPM_PACKAGE_RELEASE "1")
SET(CPACK_RPM_PACKAGE_LICENSE "AGPLv3+")
SET(CPACK_RPM_PACKAGE_ARCHITECTURE "i386")

INCLUDE(CPack)

