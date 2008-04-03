/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
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

#include "../main/tinypoker.h"
#include "test.h"

#include <string.h>

#define INPUT_SIZE (64)

char *newString()
{
	char *str;
	str = (char *) malloc(sizeof(char) * (INPUT_SIZE + 1));
	assertNotNull("malloc() failed", str);
	memset(str, '\0', INPUT_SIZE + 1);
	return str;
}

int main()
{
	char *str;

	ipp_init();

	str = newString();
	strncpy(str, "IPP 2.0 XoXoX", INPUT_SIZE);
	ipp_normalize_msg(str);
	printf("'%s'\n", str);
	assertStringEqual("Normalize failed to properly toupper()", str, "IPP 2.0 XOXOX");
	free(str);

	str = newString();
	strncpy(str, "  IPP  2.0  TEST  ", INPUT_SIZE);
	ipp_normalize_msg(str);
	printf("'%s'\n", str);
	assertStringEqual("Normalize failed to properly trim", str, "IPP 2.0 TEST");
	free(str);

	str = newString();
	strncpy(str, " IPP 2.0 TEST ", INPUT_SIZE);
	ipp_normalize_msg(str);
	printf("'%s'\n", str);
	assertStringEqual("Normalize failed to properly trim", str, "IPP 2.0 TEST");
	free(str);

	str = newString();
	strncpy(str, " IPP 2.0 TEST", INPUT_SIZE);
	ipp_normalize_msg(str);
	printf("'%s'\n", str);
	assertStringEqual("Normalize failed to properly trim", str, "IPP 2.0 TEST");
	free(str);

	str = newString();
	strncpy(str, "IPP 2.0 TEST ", INPUT_SIZE);
	ipp_normalize_msg(str);
	printf("'%s'\n", str);
	assertStringEqual("Normalize failed to properly trim", str, "IPP 2.0 TEST");
	free(str);

	ipp_exit();
	return PASS;
}
