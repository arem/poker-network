/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <linuxgeek@gmail.com>
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

int main()
{
	ipp_init();

	assertTrue("Up String should be valid", ipp_validate_msg(REGEX_MSG_UP, "UP 2D"));
	assertTrue("Up String should be valid", ipp_validate_msg(REGEX_MSG_UP, "UP KS"));

	assertFalse("Up String should not be valid", ipp_validate_msg(REGEX_MSG_UP, "UP 7C 2"));
	assertFalse("Up String should not be valid", ipp_validate_msg(REGEX_MSG_UP, "UP 7C2"));
	assertFalse("Up String should not be valid", ipp_validate_msg(REGEX_MSG_UP, "UP 9H "));
	assertFalse("Up String should not be valid", ipp_validate_msg(REGEX_MSG_UP, "UP "));
	assertFalse("Up String should not be valid", ipp_validate_msg(REGEX_MSG_UP, "UP"));
	assertFalse("Up String should not be valid", ipp_validate_msg(REGEX_MSG_UP, ""));

	ipp_exit();
	return PASS;
}
