/*
 * Copyright (C) 2007 Thomas Cort <code@member.fsf.org>
 *
 * This file is part of libtinypoker.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "../main/tinypoker.h"
#include "test.h"

#include <string.h>

#define INPUT_SIZE (64)

char *newString() {
	char *str;
	str = (char*) malloc(sizeof(char) * (INPUT_SIZE+1));
	assertNotNull("malloc() failed", str);
	memset(str, '\0', INPUT_SIZE+1);
	return str;
}

int main() {
	char *str;

	str = newString();
	strncpy(str, "IPP 2.0 XoXoX", INPUT_SIZE);
	ipp_normalize_msg(str);
	assertStringEqual("Normalize failed to properly toupper()", str, "IPP 2.0 XOXOX");
	free(str);

	str = newString();
	strncpy(str, "  IPP  2.0  TEST  ", INPUT_SIZE);
	ipp_normalize_msg(str);
	printf("'%s'\n",str);
	assertStringEqual("Normalize failed to properly trim", str, "IPP 2.0 TEST");
	free(str);

	str = newString();
	strncpy(str, " IPP 2.0 TEST ", INPUT_SIZE);
	ipp_normalize_msg(str);
	printf("'%s'\n",str);
	assertStringEqual("Normalize failed to properly trim", str, "IPP 2.0 TEST");
	free(str);

	str = newString();
	strncpy(str, " IPP 2.0 TEST", INPUT_SIZE);
	ipp_normalize_msg(str);
	printf("'%s'\n",str);
	assertStringEqual("Normalize failed to properly trim", str, "IPP 2.0 TEST");
	free(str);

	return PASS;
}
