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

int 
main()
{
	assertTrue("Button String should be valid", ipp_validate_msg(REGEX_MSG_BUTTON, "BUTTON Alpha"));
	assertTrue("Button String should be valid", ipp_validate_msg(REGEX_MSG_BUTTON, "BUTTON Beta"));
	assertTrue("Button String should be valid", ipp_validate_msg(REGEX_MSG_BUTTON, "BUTTON Gamma"));
	assertTrue("Button String should be valid", ipp_validate_msg(REGEX_MSG_BUTTON, "BUTTON Fred"));
	assertTrue("Button String should be valid", ipp_validate_msg(REGEX_MSG_BUTTON, "BUTTON TOM"));
	assertTrue("Button String should be valid", ipp_validate_msg(REGEX_MSG_BUTTON, "BUTTON a"));
	assertTrue("Button String should be valid", ipp_validate_msg(REGEX_MSG_BUTTON, "BUTTON 01234567890123456789012345678912"));

	assertFalse("Button String should not be valid", ipp_validate_msg(REGEX_MSG_BUTTON, "BUTTON 0 Scott"));
	assertFalse("Button String should not be valid", ipp_validate_msg(REGEX_MSG_BUTTON, "BUTTON 0 t"));
	assertFalse("Button String should not be valid", ipp_validate_msg(REGEX_MSG_BUTTON, "BUTTON 012345678901234567890123456789121"));
	assertFalse("Button String should not be valid", ipp_validate_msg(REGEX_MSG_BUTTON, "BUTTON "));
	assertFalse("Button String should not be valid", ipp_validate_msg(REGEX_MSG_BUTTON, "BUTTON"));
	assertFalse("Button String should not be valid", ipp_validate_msg(REGEX_MSG_BUTTON, ""));
	return PASS;
}
