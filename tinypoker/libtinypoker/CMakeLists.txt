# Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
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

PROJECT(libtinypoker C)

INCLUDE(InstallRequiredSystemLibraries)

# Version Information
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(VERSION "${CPACK_PACKAGE_VERSION}")
SET(SOVERSION "${CPACK_PACKAGE_VERSION}")
ADD_DEFINITIONS("-DVERSION=\\\"${VERSION}\\\"")

INCLUDE(CTest)
ENABLE_TESTING()

ADD_SUBDIRECTORY(src)
