#!/bin/sh
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

set -e

echo "*************************************************************************"
echo "*************************************************************************"
echo "*** WARNING: The wine port is BROKEN. Script only works on Fedora 10. ***"
echo "*************************************************************************"
echo "*************************************************************************"

scripts/clean-all.sh
cmake -DCMAKE_TOOLCHAIN_FILE=toolchains/i686-pc-mingw32.cmake .
make && make test
