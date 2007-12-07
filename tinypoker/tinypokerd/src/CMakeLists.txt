# Copyright (C) 2005, 2006, 2007 Thomas Cort <code@member.fsf.org>
#
# This file is part of tinypokerd.
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
# Put everything together
#############################################

INCLUDE_DIRECTORIES(${incdirs} )
ADD_EXECUTABLE(tinypokerd tinypokerd.c)
TARGET_LINK_LIBRARIES(tinypokerd )

#############################################
# Installation
#############################################

INSTALL(PROGRAMS tinypokerd DESTINATION sbin PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
