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

SET(TESTS testHandshake testValidateCmdIpp testValidateCmdBuyin testValidateCmdWelcome testValidateCmdNewgame testValidateCmdPlayer testValidateCmdButton testValidateCmdAnte testValidateCmdDeal testValidateCmdFrom testValidateCmdFlop testValidateCmdTurn testValidateCmdRiver testValidateCmdDraw testValidateCmdDrawn testValidateCmdDrawQ testValidateCmdFold testParseMessage testValidateCmdUp testValidateCmdDown testValidateCmdAction testValidateCmdBlind testValidateCmdTapout testValidateCmdStraddle testValidateCmdCall testValidateCmdRaise testValidateCmdOpen testValidateCmdCheck testValidateCmdQuit testValidateCmdBusted testValidateCmdGameOver testValidateCmdWinner testValidateCmdShowQ testValidateCmdShow testValidateCmdNo testValidateCmdYes testValidateCmdBeatQ testValidateCmdError testValidateCmdOk testValidateUnknownMsg testNormalizeMessage testEvalStraight2Char testEvalPrime2Char testEvalChar2Prime testEval testDeck testHandCompar testAddPlayer testBestCombination testEnumerateAllHands testVersion testRankPlayers)

SET (incdirs /usr/include /usr/local/include /usr/pkg/include /usr/i586-mingw32msvc/include /usr/i686-pc-mingw32/sys-root/mingw/include)

INCLUDE(FindPkgConfig)

PKG_CHECK_MODULES(GLIB2 glib-2.0>=2.16 REQUIRED)
PKG_CHECK_MODULES(GTHREAD2 gthread-2.0>=2.16 REQUIRED)

#############################################
# Put everything together
#############################################

INCLUDE(CTest)
ENABLE_TESTING()

INCLUDE_DIRECTORIES(${GTHREAD2_INCLUDE_DIRS} ${GLIB2_INCLUDE_DIRS})

FOREACH(TEST ${TESTS})
  ADD_EXECUTABLE(${TEST} ${TEST}.c)
  TARGET_LINK_LIBRARIES(${TEST} tinypoker ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES})
  ADD_TEST(${TEST} ${TEST})
ENDFOREACH(TEST)

