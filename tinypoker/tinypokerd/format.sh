#!/bin/sh
# Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <linuxgeek@gmail.com>
#
# This file is part of tinypokerd.
#
# tinypokerd is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# tinypokerd is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with tinypokerd.  If not, see <http://www.gnu.org/licenses/>.

find ./ -name \*.[ch] -exec indent -nbad -nbfda -bap -nbc -br -brs -c33 \
-cd33 -ncdb -ce -ci4 -cli0 -cp33 -d0 -di1 -nfc1 -nfca -i8 -ip0 -l240    \
-lp -npcs -npsl -nsc -nsob -nss -ts8 -cs -ut {} \;
find ./ -name \*.[ch]~ -exec rm {} \;
