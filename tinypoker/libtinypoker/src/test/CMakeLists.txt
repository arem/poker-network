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


INCLUDE(CTest)
ENABLE_TESTING()

ADD_EXECUTABLE(testValidateCmdIpp testValidateCmdIpp.c)
TARGET_LINK_LIBRARIES(testValidateCmdIpp tinypoker)
ADD_TEST(testValidateCmdIpp ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdIpp)

ADD_EXECUTABLE(testValidateCmdBuyin testValidateCmdBuyin.c)
TARGET_LINK_LIBRARIES(testValidateCmdBuyin tinypoker)
ADD_TEST(testValidateCmdBuyin ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdBuyin)

ADD_EXECUTABLE(testNormalizeMessage testNormalizeMessage.c)
TARGET_LINK_LIBRARIES(testNormalizeMessage tinypoker)
ADD_TEST(testNormalizeMessage ${EXECUTABLE_OUTPUT_PATH}/testNormalizeMessage)
