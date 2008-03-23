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
	assertTrue("New game String should be valid", ipp_validate_msg(REGEX_MSG_NEWGAME, "NEWGAME HOLDEM 10 20 3"));
	assertTrue("New game String should be valid", ipp_validate_msg(REGEX_MSG_NEWGAME, "NEWGAME DRAW 10 20 3"));
	assertTrue("New game String should be valid", ipp_validate_msg(REGEX_MSG_NEWGAME, "NEWGAME STUD 7 10 20 3"));

	assertFalse("New game String should not be valid", ipp_validate_msg(REGEX_MSG_NEWGAME, ""));
	assertFalse("New game String should not be valid", ipp_validate_msg(REGEX_MSG_NEWGAME, "NEWGAME HOLDEM 7 10 20 3"));
	assertFalse("New game String should not be valid", ipp_validate_msg(REGEX_MSG_NEWGAME, "NEWGAME HOLDEM 10 20 -3"));
	assertFalse("New game String should not be valid", ipp_validate_msg(REGEX_MSG_NEWGAME, "NEWGAME STUD 6 10 20 3"));
	assertFalse("New game String should not be valid", ipp_validate_msg(REGEX_MSG_NEWGAME, "NEWGAME DRAW 10 20"));
	return PASS;
}
