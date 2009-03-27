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

SET(TESTS testValidateCmdIpp testValidateCmdBuyin testValidateCmdWelcome testValidateCmdNewgame testValidateCmdPlayer testValidateCmdButton testValidateCmdAnte testValidateCmdDeal testValidateCmdFrom testValidateCmdFlop testValidateCmdTurn testValidateCmdRiver testValidateCmdDraw testValidateCmdDrawn testValidateCmdDrawQ testValidateCmdFold testParseMessage testValidateCmdUp testValidateCmdDown testValidateCmdAction testValidateCmdBlind testValidateCmdTapout testValidateCmdStraddle testValidateCmdCall testValidateCmdRaise testValidateCmdOpen testValidateCmdCheck testValidateCmdQuit testValidateCmdBusted testValidateCmdGameOver testValidateCmdWinner testValidateCmdShowQ testValidateCmdShow testValidateCmdNo testValidateCmdYes testValidateCmdBeatQ testValidateCmdError testValidateCmdOk testValidateUnknownMsg testNormalizeMessage testEvalStraight2Char testEvalPrime2Char testEvalChar2Prime testEval testDeck testHandCompar testAddPlayer testBestCombination testEnumerateAllHands testVersion testRankPlayers)

SET (incdirs /usr/include /usr/local/include /usr/pkg/include /usr/i586-mingw32msvc/include /usr/i686-pc-mingw32/sys-root/mingw/include)

INCLUDE(FindPkgConfig)

PKG_CHECK_MODULES(GLIB2 glib-2.0>=2.18 REQUIRED)
PKG_CHECK_MODULES(GTHREAD2 gthread-2.0>=2.18 REQUIRED)
PKG_CHECK_MODULES(GNUTLS gnutls>=2.4 REQUIRED)

#############################
## libgcrypt
#############################

FIND_LIBRARY(LIBGCRYPT gcrypt)

IF (LIBGCRYPT)
  MESSAGE(STATUS "Found libgcrypt: ${LIBGCRYPT}")
ELSE (LIBGCRYPT)
  MESSAGE (FATAL_ERROR "libgcrypt not found. See: http://www.gnupg.org/")
ENDIF (LIBGCRYPT)

FIND_PATH(LIBGCRYPT_INCDIR gcrypt.h ${incdirs})

IF (LIBGCRYPT_INCDIR)
  MESSAGE(STATUS "Found gcrypt.h in ${LIBGCRYPT_INCDIR}")
ELSE (LIBGCRYPT_INCDIR)
  MESSAGE (FATAL_ERROR "gcrypt.h not found. See: http://www.gnupg.org/")
ENDIF (LIBGCRYPT_INCDIR)

IF (WIN32)

#############################
## libwsock32
#############################

FIND_LIBRARY(LIBWSOCK32 wsock32)

IF (LIBWSOCK32)
  MESSAGE(STATUS "Found libwsock32: ${LIBWSOCK32}")
ELSE (LIBWSOCK32)
  MESSAGE (FATAL_ERROR "libwsock32 not found.")
ENDIF (LIBWSOCK32)
FIND_PATH(LIBWSOCK32_INCDIR winsock2.h ${incdirs})

IF (LIBWSOCK32_INCDIR)
  MESSAGE(STATUS "Found winsock2.h in ${LIBWSOCK32_INCDIR}")
ELSE (LIBWSOCK32_INCDIR)
  MESSAGE (FATAL_ERROR "winsock2.h not found.")
ENDIF (LIBWSOCK32_INCDIR)

#############################
## libws2_32
#############################

FIND_LIBRARY(LIBWS2_32 ws2_32)

IF (LIBWS2_32)
  MESSAGE(STATUS "Found libws2_32: ${LIBWS2_32}")
ELSE (LIBWS2_32)
  MESSAGE (FATAL_ERROR "libws2_32 not found.")
ENDIF (LIBWS2_32)

FIND_PATH(LIBWS2_32_INCDIR ws2tcpip.h ${incdirs})

FIND_PATH(LIBWS2_32_INCDIR ws2tcpip.h ${incdirs})

IF (LIBWS2_32_INCDIR)
  MESSAGE(STATUS "Found ws2tcpip.h in ${LIBWS2_32_INCDIR}")
ELSE (LIBWS2_32_INCDIR)
  MESSAGE (FATAL_ERROR "ws2tcpip.h not found.")
ENDIF (LIBWS2_32_INCDIR)

#############################
## wine
#############################

FIND_PROGRAM(WINE wine)
IF (WINE)
  MESSAGE(STATUS "Found wine at ${WINE}")
ELSE (WINE)
  MESSAGE(STATUS "wine not found. See http://www.winehq.org/")
ENDIF (WINE)

ENDIF (WIN32)

#############################################
# Put everything together
#############################################

INCLUDE(CTest)
ENABLE_TESTING()

IF (WIN32)
  INCLUDE_DIRECTORIES(${GNUTLS_INCLUDE_DIRS} ${GTHREAD2_INCLUDE_DIRS} ${GLIB2_INCLUDE_DIRS} ${LIBGCRYPT_INCDIR} ${LIBWSOCK32_INCDIR} ${LIBWS2_32_INCDIR})
ELSE (WIN32)
  INCLUDE_DIRECTORIES(${GNUTLS_INCLUDE_DIRS} ${GTHREAD2_INCLUDE_DIRS} ${GLIB2_INCLUDE_DIRS} ${LIBGCRYPT_INCDIR})
ENDIF  (WIN32)


FOREACH(TEST ${TESTS})
  ADD_EXECUTABLE(${TEST} ${TEST}.c)
  IF (WIN32)
    TARGET_LINK_LIBRARIES(${TEST} tinypoker ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES} ${GNUTLS_LIBRARIES} ${LIBGCRYPT} ${LIBWSOCK32} ${LIBWS2_32})
  ELSE (WIN32)
    TARGET_LINK_LIBRARIES(${TEST} tinypoker ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES} ${GNUTLS_LIBRARIES} ${LIBGCRYPT})
  ENDIF (WIN32)
  IF (RUN_TESTS_WITH_WINE)
    ADD_TEST(${TEST} ${WINE} ${TEST})
  ELSE (RUN_TESTS_WITH_WINE)
    ADD_TEST(${TEST} ${TEST})
  ENDIF (RUN_TESTS_WITH_WINE)
ENDFOREACH(TEST)

