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

int main()
{
	ipp_init();

	assertTrue("Straddle String should be valid", ipp_validate_msg(REGEX_MSG_STRADDLE, "STRADDLE 5"));
	assertTrue("Straddle String should be valid", ipp_validate_msg(REGEX_MSG_STRADDLE, "STRADDLE 0"));
	assertTrue("Straddle String should be valid", ipp_validate_msg(REGEX_MSG_STRADDLE, "STRADDLE 1234"));
	assertTrue("Straddle String should be valid", ipp_validate_msg(REGEX_MSG_STRADDLE, "STRADDLE 012345678"));

	assertFalse("Straddle String should not be valid", ipp_validate_msg(REGEX_MSG_STRADDLE, "STRADDLE -1"));
	assertFalse("Straddle String should not be valid", ipp_validate_msg(REGEX_MSG_STRADDLE, "STRADDLE 01234567890123456"));
	assertFalse("Straddle String should not be valid", ipp_validate_msg(REGEX_MSG_STRADDLE, "STRADDLE S"));
	assertFalse("Straddle String should not be valid", ipp_validate_msg(REGEX_MSG_STRADDLE, "STRADDLE "));
	assertFalse("Straddle String should not be valid", ipp_validate_msg(REGEX_MSG_STRADDLE, ""));

	ipp_exit();
	return PASS;
}
