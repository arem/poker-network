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
 * Test the Version Information and Functions.
 * This is less of a test of the versioning functions
 * and more of a test that the version numbers are accurate.
 * @param argc the number of command line arguments.
 * @param argv the command line argument list.
 * @return 0 if test passes, non-zero if test fails.
 */
int main(int argc, char *argv[]) {

	/* The version related functions are the only functions that should be called before tp_init(); */

	/* version of header and library should match */
	g_assert(TP_MAJOR_VERSION == tp_major_version);
	g_assert(TP_MINOR_VERSION == tp_minor_version);
	g_assert(TP_MICRO_VERSION == tp_micro_version);

	/* version number must be a positive number */
	g_assert(TP_MAJOR_VERSION >= 0);
	g_assert(TP_MINOR_VERSION >= 0);
	g_assert(TP_MICRO_VERSION >= 0);

	/* version number must be a positive number */
	g_assert(tp_major_version >= 0);
	g_assert(tp_minor_version >= 0);
	g_assert(tp_micro_version >= 0);

	/* Cross check that the header and library are compatible */
	g_assert(TP_CHECK_VERSION(tp_major_version, tp_minor_version, tp_micro_version));
	g_assert(tp_check_version(TP_MAJOR_VERSION, TP_MINOR_VERSION, TP_MICRO_VERSION) == NULL);

	return 0;
}
