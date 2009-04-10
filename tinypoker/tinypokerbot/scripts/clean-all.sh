#!/bin/sh
#
# Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <tcort@tomcort.com>
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

if [ -f Makefile ]
then
	make clean
fi

set -e

rm -rf Testing CMakeCache.txt DartConfiguration.tcl install_manifest.txt DartTestfile.txt Makefile cmake_install.cmake CMakeFiles CTestTestfile.cmake	