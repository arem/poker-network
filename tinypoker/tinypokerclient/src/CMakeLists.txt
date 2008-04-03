# Copyright (C) 2008 Thomas Cort <tom@tomcort.com>
#
# This file is part of tinypokerclient.
#
# tinypokerclient is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# tinypokerclient is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with tinypokerclient.  If not, see <http://www.gnu.org/licenses/>.

SET (libdirs /usr/lib /usr/pkg/lib /usr/local/lib /usr/lib64 /usr/local/lib64 /usr/lib32 /usr/local/lib32)
SET (bindirs /usr/bin /usr/pkg/bin /usr/local/bin)
SET (incdirs /usr/include /usr/local/include /usr/pkg/include)

FIND_PACKAGE(wxWidgets REQUIRED)
IF(wxWidgets_FOUND)
  MESSAGE (STATUS "Found wxWidgets")
ELSE(wxWidgets_FOUND)
  MESSAGE (FATAL_ERROR "wxWidgets not found")
ENDIF(wxWidgets_FOUND)

FIND_LIBRARY(LIBTINYPOKER tinypoker ${libdirs})

IF (LIBTINYPOKER)
  MESSAGE(STATUS "Found libtinypoker: ${LIBTINYPOKER}")
ELSE (LIBTINYPOKER)
  MESSAGE (FATAL_ERROR "libtinypoker not found. See: http://tinypoker.org/")
ENDIF (LIBTINYPOKER)

FIND_PATH(LIBTINYPOKER_INCLUDE_DIR tinypoker.h ${incdirs})

IF (LIBTINYPOKER_INCLUDE_DIR)
  MESSAGE(STATUS "Found tinypoker.h in ${LIBTINYPOKER_INCLUDE_DIR}")
ELSE (LIBTINYPOKER_INCLUDE_DIR)
  MESSAGE (FATAL_ERROR "tinypoker.h not found. See: http://tinypoker.org/")
ENDIF (LIBTINYPOKER_INCLUDE_DIR)

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

INCLUDE_DIRECTORIES(${incdirs} ${LIBTINYPOKER_INCLUDE_DIR} ${LIBGNUTLS_INCLUDE_DIR} ${LIBGSL_INCLUDE_DIR})
INCLUDE(${wxWidgets_USE_FILE})
ADD_EXECUTABLE(tinypokerclient tpApp.cc tpConnectFrame.cc tpFrame.cc tpLog.cc main.cc)
TARGET_LINK_LIBRARIES(tinypokerclient ${wxWidgets_LIBRARIES} ${LIBTINYPOKER} ${LIBGNUTLS} ${LIBGSL} ${LIBGSLCBLAS})
