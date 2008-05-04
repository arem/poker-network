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

#include <tinypoker/tinypoker.h>

/**
 * The major version number of the library.
 * Example, the '1' in the version "1.2.3".
 * Use this to determine which version of the library you've linked against.
 * Use TP_MAJOR_VERSION to find which version of this header you've included.
 * @see TP_MAJOR_VERSION
 */
const guint tp_major_version = 0;

/**
 * The minor version number of the library.
 * Example, the '2' in the version "1.2.3".
 * Use this to determine which version of the library you've linked against.
 * Use TP_MINOR_VERSION to find which version of this header you've included.
 * @see TP_MINOR_VERSION
 */
const guint tp_minor_version = 0;

/**
 * The micro version number of the library.
 * Example, the '3' in the version "1.2.3".
 * Use this to determine which version of the library you've linked against.
 * Use TP_MICRO_VERSION to find which version of this header you've included.
 * @see TP_MICRO_VERSION
 */
const guint tp_micro_version = 0;

/**
 * The binary age. Useful for ABI compatibility checks.
 */
const guint tp_binary_age = 0;

/**
 * The interface age. Useful for API compatibility checks.
 */
const guint tp_interface_age = 0;

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
const gchar* tp_check_version(guint required_major, guint required_minor, guint required_micro) {
	/* No releases have been made, therefore there are no imcompatible versions :-) */
	return NULL;
}

