/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
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

int main()
{
	assertTrue("Error String should be valid", ipp_validate_msg(REGEX_MSG_ERROR, "ERROR Insufficient funds for BLIND"));
	assertTrue("Error String should be valid", ipp_validate_msg(REGEX_MSG_ERROR, "ERROR BLIND amount should be 10 not 5"));
	assertTrue("Error String should be valid", ipp_validate_msg(REGEX_MSG_ERROR, "ERROR Too rich to TAPOUT"));
	assertTrue("Error String should be valid", ipp_validate_msg(REGEX_MSG_ERROR, "ERROR TAPOUT amount must be all your cash"));
	assertTrue("Error String should be valid", ipp_validate_msg(REGEX_MSG_ERROR, "ERROR Too many bogus messages -- goodbye"));

	assertFalse("Error String should not be valid", ipp_validate_msg(REGEX_MSG_ERROR, "ERROR "));
	assertFalse("Error String should not be valid", ipp_validate_msg(REGEX_MSG_ERROR, " ERROR "));
	assertFalse("Error String should not be valid", ipp_validate_msg(REGEX_MSG_ERROR, "ERROR ~!!@#$%^&*()<><"));
	assertFalse("Error String should not be valid", ipp_validate_msg(REGEX_MSG_ERROR, ""));
	return PASS;
}
