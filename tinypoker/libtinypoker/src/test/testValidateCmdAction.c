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

	assertTrue("Action String should be valid", ipp_validate_msg(REGEX_MSG_ACTION, "ACTION? BLIND 5"));
	assertTrue("Action String should be valid", ipp_validate_msg(REGEX_MSG_ACTION, "ACTION? BLIND 10"));
	assertTrue("Action String should be valid", ipp_validate_msg(REGEX_MSG_ACTION, "ACTION? STRADDLE 4"));
	assertTrue("Action String should be valid", ipp_validate_msg(REGEX_MSG_ACTION, "ACTION? OWING 0"));

	assertFalse("Action String should not be valid", ipp_validate_msg(REGEX_MSG_ACTION, "ACTION? OWING 4d"));
	assertFalse("Action String should not be valid", ipp_validate_msg(REGEX_MSG_ACTION, "ACTION? BLIND f "));
	assertFalse("Action String should not be valid", ipp_validate_msg(REGEX_MSG_ACTION, "ACTION? BLIND 2 "));
	assertFalse("Action String should not be valid", ipp_validate_msg(REGEX_MSG_ACTION, "ACTION? BLIND -1"));
	assertFalse("Action String should not be valid", ipp_validate_msg(REGEX_MSG_ACTION, "ACTION? BLIND"));
	assertFalse("Action String should not be valid", ipp_validate_msg(REGEX_MSG_ACTION, "ACTION?"));
	assertFalse("Action String should not be valid", ipp_validate_msg(REGEX_MSG_ACTION, ""));

	ipp_exit();
	return PASS;
}
