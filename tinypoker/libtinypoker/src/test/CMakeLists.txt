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

#############################################
# libpthread checks
#############################################

FIND_LIBRARY(LIBPTHREAD pthread ${libdirs})

IF (LIBPTHREAD)
  MESSAGE(STATUS "Found libpthread: ${LIBPTHREAD}")
ELSE (LIBPTHREAD)
  MESSAGE (FATAL_ERROR "libpthread not found. See: http://www.gnu.org/software/libc/")
ENDIF (LIBPTHREAD)

FIND_PATH(LIBPTHREAD_INCLUDE_DIR pthread.h ${incdirs})

IF (LIBPTHREAD_INCLUDE_DIR)
  MESSAGE(STATUS "Found pthread.h in ${LIBPTHREAD_INCLUDE_DIR}")
ELSE (LIBPTHREAD_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "pthread.h not found. See: http://www.gnu.org/software/libc/")
ENDIF (LIBPTHREAD_INCLUDE_DIR)

#############################################
# libgnutls checks
#############################################

FIND_LIBRARY(LIBGNUTLS gnutls ${libdirs})

IF (LIBGNUTLS)
  MESSAGE(STATUS "Found libgnutls: ${LIBGNUTLS}")
ELSE (LIBGNUTLS)
  MESSAGE (FATAL_ERROR "libgnutls not found. See: http://www.gnu.org/software/gnutls/")
ENDIF (LIBGNUTLS)

FIND_PATH(LIBGNUTLS_INCLUDE_DIR gnutls/gnutls.h ${incdirs})

IF (LIBGNUTLS_INCLUDE_DIR)
  MESSAGE(STATUS "Found gnutls/gnutls.h in ${LIBGNUTLS_INCLUDE_DIR}")
ELSE (LIBGNUTLS_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "gnutls/gnutls.h not found. See: http://www.gnu.org/software/gnutls/")
ENDIF (LIBGNUTLS_INCLUDE_DIR)

#############################################
# libgsl checks
#############################################

FIND_LIBRARY(LIBGSL gsl ${libdirs})

IF (LIBGSL)
  MESSAGE(STATUS "Found libgsl: ${LIBGSL}")
ELSE (LIBGSL)
  MESSAGE (FATAL_ERROR "libgsl not found.")
ENDIF (LIBGSL)

FIND_LIBRARY(LIBGSLCBLAS gslcblas ${libdirs})

IF (LIBGSLCBLAS)
  MESSAGE(STATUS "Found libgslcblas: ${LIBGSLCBLAS}")
ELSE (LIBGSLCBLAS)
  MESSAGE (FATAL_ERROR "libgslcblas not found.")
ENDIF (LIBGSLCBLAS)

FIND_PATH(LIBGSL_INCLUDE_DIR gsl/gsl_rng.h ${incdirs})

IF (LIBGSL_INCLUDE_DIR)
  MESSAGE(STATUS "Found gsl/gsl_rng.h in ${LIBGSL_INCLUDE_DIR}")
ELSE (LIBGSL_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "gsl/gsl_rng.h not found.")
ENDIF (LIBGSL_INCLUDE_DIR)

#############################################
# Put everything together
#############################################

INCLUDE_DIRECTORIES(${LIBGNUTLS_INCLUDE_DIR} ${LIBPTHREAD_INCLUDE_DIR} ${LIBGSL_INCLUDE_DIR})

INCLUDE(CTest)
ENABLE_TESTING()

ADD_EXECUTABLE(testValidateCmdIpp testValidateCmdIpp.c)
TARGET_LINK_LIBRARIES(testValidateCmdIpp tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdIpp testValidateCmdIpp)

ADD_EXECUTABLE(testValidateCmdBuyin testValidateCmdBuyin.c)
TARGET_LINK_LIBRARIES(testValidateCmdBuyin tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdBuyin testValidateCmdBuyin)

ADD_EXECUTABLE(testValidateCmdWelcome testValidateCmdWelcome.c)
TARGET_LINK_LIBRARIES(testValidateCmdWelcome tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdWelcome testValidateCmdWelcome)

ADD_EXECUTABLE(testValidateCmdNewgame testValidateCmdNewgame.c)
TARGET_LINK_LIBRARIES(testValidateCmdNewgame tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdNewgame testValidateCmdNewgame)

ADD_EXECUTABLE(testValidateCmdPlayer testValidateCmdPlayer.c)
TARGET_LINK_LIBRARIES(testValidateCmdPlayer tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdPlayer testValidateCmdPlayer)

ADD_EXECUTABLE(testValidateCmdButton testValidateCmdButton.c)
TARGET_LINK_LIBRARIES(testValidateCmdButton tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdButton testValidateCmdButton)

ADD_EXECUTABLE(testValidateCmdAnte testValidateCmdAnte.c)
TARGET_LINK_LIBRARIES(testValidateCmdAnte tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdAnte testValidateCmdAnte)

ADD_EXECUTABLE(testValidateCmdDeal testValidateCmdDeal.c)
TARGET_LINK_LIBRARIES(testValidateCmdDeal tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdDeal testValidateCmdDeal)

ADD_EXECUTABLE(testValidateCmdFrom testValidateCmdFrom.c)
TARGET_LINK_LIBRARIES(testValidateCmdFrom tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdFrom testValidateCmdFrom)

ADD_EXECUTABLE(testValidateCmdFlop testValidateCmdFlop.c)
TARGET_LINK_LIBRARIES(testValidateCmdFlop tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdFlop testValidateCmdFlop)

ADD_EXECUTABLE(testValidateCmdTurn testValidateCmdTurn.c)
TARGET_LINK_LIBRARIES(testValidateCmdTurn tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdTurn testValidateCmdTurn)

ADD_EXECUTABLE(testValidateCmdRiver testValidateCmdRiver.c)
TARGET_LINK_LIBRARIES(testValidateCmdRiver tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdRiver testValidateCmdRiver)

ADD_EXECUTABLE(testValidateCmdDraw testValidateCmdDraw.c)
TARGET_LINK_LIBRARIES(testValidateCmdDraw tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdDraw testValidateCmdDraw)

ADD_EXECUTABLE(testValidateCmdDrawn testValidateCmdDrawn.c)
TARGET_LINK_LIBRARIES(testValidateCmdDrawn tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdDrawn testValidateCmdDrawn)

ADD_EXECUTABLE(testValidateCmdDrawQ testValidateCmdDrawQ.c)
TARGET_LINK_LIBRARIES(testValidateCmdDrawQ tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdDrawQ testValidateCmdDrawQ)

ADD_EXECUTABLE(testValidateCmdFold testValidateCmdFold.c)
TARGET_LINK_LIBRARIES(testValidateCmdFold tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdFold testValidateCmdFold)

ADD_EXECUTABLE(testParseMessage testParseMessage.c)
TARGET_LINK_LIBRARIES(testParseMessage tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testParseMessage testParseMessage)

ADD_EXECUTABLE(testValidateCmdUp testValidateCmdUp.c)
TARGET_LINK_LIBRARIES(testValidateCmdUp tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdUp testValidateCmdUp)

ADD_EXECUTABLE(testValidateCmdDown testValidateCmdDown.c)
TARGET_LINK_LIBRARIES(testValidateCmdDown tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdDown testValidateCmdDown)

ADD_EXECUTABLE(testValidateCmdAction testValidateCmdAction.c)
TARGET_LINK_LIBRARIES(testValidateCmdAction tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdAction testValidateCmdAction)

ADD_EXECUTABLE(testValidateCmdBlind testValidateCmdBlind.c)
TARGET_LINK_LIBRARIES(testValidateCmdBlind tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdBlind testValidateCmdBlind)

ADD_EXECUTABLE(testValidateCmdTapout testValidateCmdTapout.c)
TARGET_LINK_LIBRARIES(testValidateCmdTapout tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdTapout testValidateCmdTapout)

ADD_EXECUTABLE(testValidateCmdStraddle testValidateCmdStraddle.c)
TARGET_LINK_LIBRARIES(testValidateCmdStraddle tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdStraddle testValidateCmdStraddle)

ADD_EXECUTABLE(testValidateCmdCall testValidateCmdCall.c)
TARGET_LINK_LIBRARIES(testValidateCmdCall tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdCall testValidateCmdCall)

ADD_EXECUTABLE(testValidateCmdRaise testValidateCmdRaise.c)
TARGET_LINK_LIBRARIES(testValidateCmdRaise tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdRaise testValidateCmdRaise)

ADD_EXECUTABLE(testValidateCmdOpen testValidateCmdOpen.c)
TARGET_LINK_LIBRARIES(testValidateCmdOpen tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdOpen testValidateCmdOpen)

ADD_EXECUTABLE(testValidateCmdCheck testValidateCmdCheck.c)
TARGET_LINK_LIBRARIES(testValidateCmdCheck tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdCheck testValidateCmdCheck)

ADD_EXECUTABLE(testValidateCmdQuit testValidateCmdQuit.c)
TARGET_LINK_LIBRARIES(testValidateCmdQuit tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdQuit testValidateCmdQuit)

ADD_EXECUTABLE(testValidateCmdBusted testValidateCmdBusted.c)
TARGET_LINK_LIBRARIES(testValidateCmdBusted tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdBusted testValidateCmdBusted)

ADD_EXECUTABLE(testValidateCmdGameOver testValidateCmdGameOver.c)
TARGET_LINK_LIBRARIES(testValidateCmdGameOver tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdGameOver testValidateCmdGameOver)

ADD_EXECUTABLE(testValidateCmdWinner testValidateCmdWinner.c)
TARGET_LINK_LIBRARIES(testValidateCmdWinner tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdWinner testValidateCmdWinner)

ADD_EXECUTABLE(testValidateCmdShowQ testValidateCmdShowQ.c)
TARGET_LINK_LIBRARIES(testValidateCmdShowQ tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdShowQ testValidateCmdShowQ)

ADD_EXECUTABLE(testValidateCmdShow testValidateCmdShow.c)
TARGET_LINK_LIBRARIES(testValidateCmdShow tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdShow testValidateCmdShow)

ADD_EXECUTABLE(testValidateCmdNo testValidateCmdNo.c)
TARGET_LINK_LIBRARIES(testValidateCmdNo tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdNo testValidateCmdNo)

ADD_EXECUTABLE(testValidateCmdYes testValidateCmdYes.c)
TARGET_LINK_LIBRARIES(testValidateCmdYes tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdYes testValidateCmdYes)

ADD_EXECUTABLE(testValidateCmdBeatQ testValidateCmdBeatQ.c)
TARGET_LINK_LIBRARIES(testValidateCmdBeatQ tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdBeatQ testValidateCmdBeatQ)

ADD_EXECUTABLE(testValidateCmdError testValidateCmdError.c)
TARGET_LINK_LIBRARIES(testValidateCmdError tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdError testValidateCmdError)

ADD_EXECUTABLE(testValidateCmdOk testValidateCmdOk.c)
TARGET_LINK_LIBRARIES(testValidateCmdOk tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateCmdOk testValidateCmdOk)

ADD_EXECUTABLE(testValidateUnknownMsg testValidateUnknownMsg.c)
TARGET_LINK_LIBRARIES(testValidateUnknownMsg tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testValidateUnknownMsg testValidateUnknownMsg)

ADD_EXECUTABLE(testNormalizeMessage testNormalizeMessage.c)
TARGET_LINK_LIBRARIES(testNormalizeMessage tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testNormalizeMessage testNormalizeMessage)

ADD_EXECUTABLE(testEvalStraight2Char testEvalStraight2Char.c)
TARGET_LINK_LIBRARIES(testEvalStraight2Char tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testEvalStraight2Char testEvalStraight2Char)

ADD_EXECUTABLE(testEvalPrime2Char testEvalPrime2Char.c)
TARGET_LINK_LIBRARIES(testEvalPrime2Char tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testEvalPrime2Char testEvalPrime2Char)

ADD_EXECUTABLE(testEvalChar2Prime testEvalChar2Prime.c)
TARGET_LINK_LIBRARIES(testEvalChar2Prime tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testEvalChar2Prime testEvalChar2Prime)

ADD_EXECUTABLE(testEval testEval.c)
TARGET_LINK_LIBRARIES(testEval tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testEval testEval)

ADD_EXECUTABLE(testDeck testDeck.c)
TARGET_LINK_LIBRARIES(testDeck tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testDeck testDeck)

ADD_EXECUTABLE(testHandCompar testHandCompar.c)
TARGET_LINK_LIBRARIES(testHandCompar tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testHandCompar testHandCompar)

ADD_EXECUTABLE(testAddPlayer testAddPlayer.c)
TARGET_LINK_LIBRARIES(testAddPlayer tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testAddPlayer testAddPlayer)

ADD_EXECUTABLE(testBestCombination testBestCombination.c)
TARGET_LINK_LIBRARIES(testBestCombination tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testBestCombination testBestCombination)

ADD_EXECUTABLE(testEnumerateAllHands testEnumerateAllHands.c)
TARGET_LINK_LIBRARIES(testEnumerateAllHands tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testEnumerateAllHands testEnumerateAllHands)

ADD_EXECUTABLE(testVersion testVersion.c)
TARGET_LINK_LIBRARIES(testVersion tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testVersion testVersion)

ADD_EXECUTABLE(testRankPlayers testRankPlayers.c)
TARGET_LINK_LIBRARIES(testRankPlayers tinypoker ${LIBGNUTLS} ${LIBPTHREAD} ${LIBGSLCBLAS} ${LIBGSL})
ADD_TEST(testRankPlayers testRankPlayers)
