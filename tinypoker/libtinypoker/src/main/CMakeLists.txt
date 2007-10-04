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

SET (libdirs /usr/lib /usr/pkg/lib /usr/local/lib /usr/lib64 /usr/local/lib64 /usr/lib32 /usr/local/lib32)
SET (bindirs /usr/bin /usr/pkg/bin /usr/local/bin)
SET (incdirs /usr/include /usr/local/include /usr/pkg/include /usr/include/mysql /usr/local/include/mysql /usr/pkg/include/mysql /usr/include/poker-eval /usr/local/include/poker-eval /usr/pkg/include/poker-eval)

#############################################
# libgnet checks
#############################################

FIND_LIBRARY(LIBGNET gnet-2.0 ${libdirs})

IF (LIBGNET)
  MESSAGE(STATUS "Found libgnet-2.0: ${LIBGNET}")
ELSE (LIBGNET)
  MESSAGE (FATAL_ERROR "libgnet-2.0 not found.")
ENDIF (LIBGNET)

#############################################
# Put everything together
#############################################

INCLUDE_DIRECTORIES(${incdirs})
ADD_LIBRARY(tinypoker tinypoker.c)
LINK_LIBRARIES(tinypoker ${LIBGNET})
INSTALL(TARGETS tinypoker DESTINATION lib)
