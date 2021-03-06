#!/bin/sh
#
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

if [ -f Makefile ]
then
	make clean
fi

set -e

rm -rf Testing CMakeCache.txt DartConfiguration.tcl install_manifest.txt DartTestfile.txt Makefile cmake_install.cmake CMakeFiles 
rm -rf CTestTestfile.cmake CPackConfig.cmake CPackSourceConfig.cmake _CPack_Packages libtinypoker-0.2.0.deb libtinypoker-0.2.0.rpm
rm -rf src/CMakeFiles src/Makefile src/cmake_install.cmake src/DartTestfile.txt src/CTestTestfile.cmake doxygen tinypoker.pc
rm -rf src/main/CMakeFiles src/main/cmake_install.cmake src/main/DartTestfile.txt src/main/Makefile src/main/CTestTestfile.cmake	
rm -rf src/examples/CMakeFiles src/examples/cmake_install.cmake src/examples/DartTestfile.txt src/examples/Makefile src/examples/CTestTestfile.cmake	
rm -rf src/test/CMakeFiles src/test/cmake_install.cmake src/test/DartTestfile.txt src/test/Makefile src/test/CTestTestfile.cmake	
rm -f src/test/libtinypoker.dll
