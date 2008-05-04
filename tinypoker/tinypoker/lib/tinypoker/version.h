/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This file is part of TinyPoker.
 *
 * TinyPoker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TinyPoker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TinyPoker.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TINYPOKER_VERSION_H

/**
 * Used interally to prevent double inclusion of this header file.
 */
#define __TINYPOKER_VERSION_H

#include <glib.h>

/**
 * The major version number of this header.
 * Example, the '1' in the version "1.2.3".
 * Use this to determine which version of this header you've included.
 * Use tp_major_version to determine which version of the library you've linked against.
 * @see tp_major_version
 */
#define TP_MAJOR_VERSION (0)

/**
 * The minor version number of this header.
 * Example, the '2' in the version "1.2.3".
 * Use this to determine which version of this header you've included.
 * Use tp_minor_version to determine which version of the library you've linked against.
 * @see tp_minor_version
 */
#define TP_MINOR_VERSION (0)

/**
 * The micro version number of this header.
 * Example, the '3' in the version "1.2.3".
 * Use this to determine which version of this header you've included.
 * Use tp_micro_version to determine which version of the library you've linked against.
 * @see tp_micro_version
 */
#define TP_MICRO_VERSION (0)

/**
 * Check the version parameters and compare them against the version of this header.
 * This also calls GLIB_CHECK_VERSION and GNET_CHECK_VERSION for you.
 * Use this at compile time to make sure you're including the desired minimum version of this header.
 * Use tp_check_version at runtime to determine if the library used at runtime is compatible with the one originally linked against.
 * @param major the minimum major version number of this header your program requires. (an integer)
 * @param minor the minimum minor version number of this header your program requires. (an integer)
 * @param micro the minimum micro version number of this header your program requires. (an integer)
 * @return TRUE if the version of this header file is greater than or equal to the parameters. FALSE otherwise.
 * @see TP_MAJOR_VERSION
 * @see TP_MINOR_VERSION
 * @see TP_MICRO_VERSION
 * @see tp_check_version()
 */
#define TP_CHECK_VERSION(major,minor,micro)									\
(														\
	(TP_MAJOR_VERSION  > (major))									||	\
	(TP_MAJOR_VERSION == (major) && TP_MINOR_VERSION  > (minor))					||	\
	(TP_MAJOR_VERSION == (major) && TP_MINOR_VERSION == (minor) && TP_MICRO_VERSION >= (micro))		\
) && GNET_CHECK_VERSION(2, 0, 7) && GLIB_CHECK_VERSION(2, 16, 3) 

/**
 * The major version number of the library.
 * Example, the '1' in the version "1.2.3".
 * Use this to determine which version of the library you've linked against.
 * Use TP_MAJOR_VERSION to find which version of this header you've included.
 * @see TP_MAJOR_VERSION
 */
extern const guint tp_major_version;

/**
 * The minor version number of the library.
 * Example, the '2' in the version "1.2.3".
 * Use this to determine which version of the library you've linked against.
 * Use TP_MINOR_VERSION to find which version of this header you've included.
 * @see TP_MINOR_VERSION
 */
extern const guint tp_minor_version;

/**
 * The micro version number of the library.
 * Example, the '3' in the version "1.2.3".
 * Use this to determine which version of the library you've linked against.
 * Use TP_MICRO_VERSION to find which version of this header you've included.
 * @see TP_MICRO_VERSION
 */
extern const guint tp_micro_version;

/**
 * The binary age. Useful for ABI compatibility checks.
 */
extern const guint tp_binary_age;

/**
 * The interface age. Useful for API compatibility checks.
 */
extern const guint tp_interface_age;

/**
 * Determine if the library used at runtime is compatible with the one originally linked against.
 * In general you should pass TP_MAJOR_VERSION, TP_MINOR_VERSION and TP_MICRO_VERSION as parameters.
 * Use TP_CHECK_VERSION at compile time to make sure you're including the desired minimum version of this header.
 * @param required_major should be TP_MAJOR_VERSION
 * @param required_minor should be TP_MINOR_VERSION
 * @param required_micro should be TP_MICRO_VERSION
 * @return NULL if compatible, or a string if incompatible. The string must not be modified or free()'d.
 * @see TP_MAJOR_VERSION
 * @see TP_MINOR_VERSION
 * @see TP_MICRO_VERSION
 * @see TP_CHECK_VERSION()
 */
const gchar* tp_check_version(guint required_major, guint required_minor, guint required_micro);

#endif /* __TINYPOKER_VERSION_H */

#ifdef __cplusplus
}
#endif

