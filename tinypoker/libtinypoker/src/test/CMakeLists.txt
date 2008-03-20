# Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
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
# Put everything together
#############################################

INCLUDE_DIRECTORIES(${LIBGNUTLS_INCLUDE_DIR} ${LIBPTHREAD_INCLUDE_DIR})

INCLUDE(CTest)
ENABLE_TESTING()

ADD_EXECUTABLE(testValidateCmdIpp testValidateCmdIpp.c)
TARGET_LINK_LIBRARIES(testValidateCmdIpp tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdIpp ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdIpp)

ADD_EXECUTABLE(testValidateCmdBuyin testValidateCmdBuyin.c)
TARGET_LINK_LIBRARIES(testValidateCmdBuyin tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdBuyin ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdBuyin)

ADD_EXECUTABLE(testValidateCmdWelcome testValidateCmdWelcome.c)
TARGET_LINK_LIBRARIES(testValidateCmdWelcome tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdWelcome ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdWelcome)

ADD_EXECUTABLE(testValidateCmdNewgame testValidateCmdNewgame.c)
TARGET_LINK_LIBRARIES(testValidateCmdNewgame tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdNewgame ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdNewgame)

ADD_EXECUTABLE(testValidateCmdPlayer testValidateCmdPlayer.c)
TARGET_LINK_LIBRARIES(testValidateCmdPlayer tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdPlayer ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdPlayer)

ADD_EXECUTABLE(testValidateCmdButton testValidateCmdButton.c)
TARGET_LINK_LIBRARIES(testValidateCmdButton tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdButton ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdButton)

ADD_EXECUTABLE(testValidateCmdAnte testValidateCmdAnte.c)
TARGET_LINK_LIBRARIES(testValidateCmdAnte tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdAnte ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdAnte)

ADD_EXECUTABLE(testValidateCmdDeal testValidateCmdDeal.c)
TARGET_LINK_LIBRARIES(testValidateCmdDeal tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdDeal ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdDeal)

ADD_EXECUTABLE(testValidateCmdFrom testValidateCmdFrom.c)
TARGET_LINK_LIBRARIES(testValidateCmdFrom tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdFrom ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdFrom)

ADD_EXECUTABLE(testValidateCmdFlop testValidateCmdFlop.c)
TARGET_LINK_LIBRARIES(testValidateCmdFlop tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdFlop ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdFlop)

ADD_EXECUTABLE(testValidateCmdTurn testValidateCmdTurn.c)
TARGET_LINK_LIBRARIES(testValidateCmdTurn tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdTurn ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdTurn)

ADD_EXECUTABLE(testValidateCmdRiver testValidateCmdRiver.c)
TARGET_LINK_LIBRARIES(testValidateCmdRiver tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdRiver ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdRiver)

ADD_EXECUTABLE(testValidateCmdDraw testValidateCmdDraw.c)
TARGET_LINK_LIBRARIES(testValidateCmdDraw tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdDraw ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdDraw)

ADD_EXECUTABLE(testValidateCmdDrawn testValidateCmdDrawn.c)
TARGET_LINK_LIBRARIES(testValidateCmdDrawn tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdDrawn ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdDrawn)

ADD_EXECUTABLE(testValidateCmdDrawQ testValidateCmdDrawQ.c)
TARGET_LINK_LIBRARIES(testValidateCmdDrawQ tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdDrawQ ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdDrawQ)

ADD_EXECUTABLE(testValidateCmdFold testValidateCmdFold.c)
TARGET_LINK_LIBRARIES(testValidateCmdFold tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdFold ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdFold)

ADD_EXECUTABLE(testParseMessage testParseMessage.c)
TARGET_LINK_LIBRARIES(testParseMessage tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testParseMessage ${EXECUTABLE_OUTPUT_PATH}/testParseMessage)

ADD_EXECUTABLE(testValidateCmdUp testValidateCmdUp.c)
TARGET_LINK_LIBRARIES(testValidateCmdUp tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdUp ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdUp)

ADD_EXECUTABLE(testValidateCmdDown testValidateCmdDown.c)
TARGET_LINK_LIBRARIES(testValidateCmdDown tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdDown ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdDown)

ADD_EXECUTABLE(testValidateCmdAction testValidateCmdAction.c)
TARGET_LINK_LIBRARIES(testValidateCmdAction tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdAction ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdAction)

ADD_EXECUTABLE(testValidateCmdBlind testValidateCmdBlind.c)
TARGET_LINK_LIBRARIES(testValidateCmdBlind tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdBlind ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdBlind)

ADD_EXECUTABLE(testValidateCmdTapout testValidateCmdTapout.c)
TARGET_LINK_LIBRARIES(testValidateCmdTapout tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdTapout ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdTapout)

ADD_EXECUTABLE(testValidateCmdStraddle testValidateCmdStraddle.c)
TARGET_LINK_LIBRARIES(testValidateCmdStraddle tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdStraddle ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdStraddle)

ADD_EXECUTABLE(testValidateCmdCall testValidateCmdCall.c)
TARGET_LINK_LIBRARIES(testValidateCmdCall tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdCall ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdCall)

ADD_EXECUTABLE(testValidateCmdRaise testValidateCmdRaise.c)
TARGET_LINK_LIBRARIES(testValidateCmdRaise tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdRaise ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdRaise)

ADD_EXECUTABLE(testValidateCmdOpen testValidateCmdOpen.c)
TARGET_LINK_LIBRARIES(testValidateCmdOpen tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdOpen ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdOpen)

ADD_EXECUTABLE(testValidateCmdCheck testValidateCmdCheck.c)
TARGET_LINK_LIBRARIES(testValidateCmdCheck tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdCheck ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdCheck)

ADD_EXECUTABLE(testValidateCmdQuit testValidateCmdQuit.c)
TARGET_LINK_LIBRARIES(testValidateCmdQuit tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdQuit ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdQuit)

ADD_EXECUTABLE(testValidateCmdBusted testValidateCmdBusted.c)
TARGET_LINK_LIBRARIES(testValidateCmdBusted tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdBusted ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdBusted)

ADD_EXECUTABLE(testValidateCmdGameOver testValidateCmdGameOver.c)
TARGET_LINK_LIBRARIES(testValidateCmdGameOver tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdGameOver ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdGameOver)

ADD_EXECUTABLE(testValidateCmdWinner testValidateCmdWinner.c)
TARGET_LINK_LIBRARIES(testValidateCmdWinner tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdWinner ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdWinner)

ADD_EXECUTABLE(testValidateCmdShowQ testValidateCmdShowQ.c)
TARGET_LINK_LIBRARIES(testValidateCmdShowQ tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdShowQ ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdShowQ)

ADD_EXECUTABLE(testValidateCmdShow testValidateCmdShow.c)
TARGET_LINK_LIBRARIES(testValidateCmdShow tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdShow ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdShow)

ADD_EXECUTABLE(testValidateCmdNo testValidateCmdNo.c)
TARGET_LINK_LIBRARIES(testValidateCmdNo tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdNo ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdNo)

ADD_EXECUTABLE(testValidateCmdYes testValidateCmdYes.c)
TARGET_LINK_LIBRARIES(testValidateCmdYes tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdYes ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdYes)

ADD_EXECUTABLE(testValidateCmdBeatQ testValidateCmdBeatQ.c)
TARGET_LINK_LIBRARIES(testValidateCmdBeatQ tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdBeatQ ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdBeatQ)

ADD_EXECUTABLE(testValidateCmdError testValidateCmdError.c)
TARGET_LINK_LIBRARIES(testValidateCmdError tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdError ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdError)

ADD_EXECUTABLE(testValidateCmdOk testValidateCmdOk.c)
TARGET_LINK_LIBRARIES(testValidateCmdOk tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateCmdOk ${EXECUTABLE_OUTPUT_PATH}/testValidateCmdOk)

ADD_EXECUTABLE(testValidateUnknownMsg testValidateUnknownMsg.c)
TARGET_LINK_LIBRARIES(testValidateUnknownMsg tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testValidateUnknownMsg ${EXECUTABLE_OUTPUT_PATH}/testValidateUnknownMsg)

ADD_EXECUTABLE(testNormalizeMessage testNormalizeMessage.c)
TARGET_LINK_LIBRARIES(testNormalizeMessage tinypoker ${LIBGNUTLS} ${LIBPTHREAD})
ADD_TEST(testNormalizeMessage ${EXECUTABLE_OUTPUT_PATH}/testNormalizeMessage)
