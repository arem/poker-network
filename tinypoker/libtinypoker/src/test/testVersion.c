/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <linuxgeek@gmail.com>
 * 
 * This file is part of libtinypoker.
 * 
 * libtinypoker is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * libtinypoker is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * libtinypoker.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>

#include "../main/tinypoker.h"
#include "test.h"

/**
 * Help the maintainer avoid mistakes by ensuring that the version 
 * numbers match the version string.
 */
int main()
{
	int len = 0;
	char *version_expected, *version_actual;

	/* compile time check to see that we defined these as integers */
	int major = LIBTINYPOKER_MAJOR_VERSION;
	int minor = LIBTINYPOKER_MINOR_VERSION;
	int patch = LIBTINYPOKER_PATCH_VERSION;

	/* compile time check to see that we defined this as a string */
	len = strlen(LIBTINYPOKER_VERSION);

	version_expected = (char *) malloc(sizeof(char) * (len + 1));
	assertNotNull("malloc failed", version_expected);	
	memset(version_expected, '\0', sizeof(char) * (len + 1));
	memcpy(version_expected, LIBTINYPOKER_VERSION, len);

	version_actual = (char *) malloc(sizeof(char) * (len + 2));
	assertNotNull("malloc failed", version_actual);	
	memset(version_actual, '\0', sizeof(char) * (len + 2));
	snprintf(version_actual, len + 1, "%d.%d.%d", major, minor, patch);

	assertStringEqual("Numeric version not equal to string version", version_expected, version_actual);

	free(version_expected);
	version_expected = NULL;

	free(version_actual);
	version_actual = NULL;

	return PASS;
}
