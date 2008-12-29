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

	assertTrue("Drawn String should be valid", ipp_validate_msg(REGEX_MSG_DRAWN, "DRAWN 1 AS"));
	assertTrue("Drawn String should be valid", ipp_validate_msg(REGEX_MSG_DRAWN, "DRAWN 2 AS KS"));
	assertTrue("Drawn String should be valid", ipp_validate_msg(REGEX_MSG_DRAWN, "DRAWN 3 AS KS QS"));
	assertTrue("Drawn String should be valid", ipp_validate_msg(REGEX_MSG_DRAWN, "DRAWN 4 AS KS QS JS"));
	assertTrue("Drawn String should be valid", ipp_validate_msg(REGEX_MSG_DRAWN, "DRAWN 5 AS KS QS JS TS"));

	assertFalse("Drawn String should not be valid", ipp_validate_msg(REGEX_MSG_DRAWN, "DRAWN 0"));
	assertFalse("Drawn String should not be valid", ipp_validate_msg(REGEX_MSG_DRAWN, "DRAWN "));
	assertFalse("Drawn String should not be valid", ipp_validate_msg(REGEX_MSG_DRAWN, "DRAWN"));
	assertFalse("Drawn String should not be valid", ipp_validate_msg(REGEX_MSG_DRAWN, "DRAWN 1 AS KS"));
	assertFalse("Drawn String should not be valid", ipp_validate_msg(REGEX_MSG_DRAWN, "DRAWN 2 AS"));
	assertFalse("Drawn String should not be valid", ipp_validate_msg(REGEX_MSG_DRAWN, "DRAWN 6 AS KS QS JS TS 9S"));
	assertFalse("Drawn String should not be valid", ipp_validate_msg(REGEX_MSG_DRAWN, ""));

	ipp_exit();
	return PASS;
}
