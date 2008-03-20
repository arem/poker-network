/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This file is part of libtinypoker.
 *
 * libtinypoker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * libtinypoker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libtinypoker.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../main/tinypoker.h"
#include "test.h"

int main()
{
	assertTrue("Quit String should be valid", ipp_validate_msg(REGEX_MSG_QUIT, "QUIT BAGELBOT"));
	assertTrue("Quit String should be valid", ipp_validate_msg(REGEX_MSG_QUIT, "QUIT Coat_Hanger"));
	assertTrue("Quit String should be valid", ipp_validate_msg(REGEX_MSG_QUIT, "QUIT Alpha"));
	assertTrue("Quit String should be valid", ipp_validate_msg(REGEX_MSG_QUIT, "QUIT Beta"));
	assertTrue("Quit String should be valid", ipp_validate_msg(REGEX_MSG_QUIT, "QUIT Gamma"));

	assertFalse("Quit String should not be valid", ipp_validate_msg(REGEX_MSG_QUIT, ""));
	assertFalse("Quit String should not be valid", ipp_validate_msg(REGEX_MSG_QUIT, "QUIT "));
	assertFalse("Quit String should not be valid", ipp_validate_msg(REGEX_MSG_QUIT, "QUIT\n"));
	assertFalse("Quit String should not be valid", ipp_validate_msg(REGEX_MSG_QUIT, "QUIT \n"));
	assertFalse("Quit String should not be valid", ipp_validate_msg(REGEX_MSG_QUIT, "QUIT X 1 1"));
	return PASS;
}
