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

	assertTrue("Show String should be valid", ipp_validate_msg(REGEX_MSG_SHOW, "SHOW STRAIGHTFLUSH J"));
	assertTrue("Show String should be valid", ipp_validate_msg(REGEX_MSG_SHOW, "SHOW FOUROFAKIND 7 A"));
	assertTrue("Show String should be valid", ipp_validate_msg(REGEX_MSG_SHOW, "SHOW FULLHOUSE Q 3"));
	assertTrue("Show String should be valid", ipp_validate_msg(REGEX_MSG_SHOW, "SHOW FLUSH T 9 8 6 2"));
	assertTrue("Show String should be valid", ipp_validate_msg(REGEX_MSG_SHOW, "SHOW STRAIGHT K"));
	assertTrue("Show String should be valid", ipp_validate_msg(REGEX_MSG_SHOW, "SHOW THREEOFAKIND 2 Q 7"));
	assertTrue("Show String should be valid", ipp_validate_msg(REGEX_MSG_SHOW, "SHOW TWOPAIR J 8 A"));
	assertTrue("Show String should be valid", ipp_validate_msg(REGEX_MSG_SHOW, "SHOW ONEPAIR T A 5 3"));
	assertTrue("Show String should be valid", ipp_validate_msg(REGEX_MSG_SHOW, "SHOW HIGHCARD Q T 8 7 2"));

	assertFalse("Show String should not be valid", ipp_validate_msg(REGEX_MSG_SHOW, "SHOW HIGHCARD Q T 8 7 "));
	assertFalse("Show String should not be valid", ipp_validate_msg(REGEX_MSG_SHOW, "SHOW HIGHCARD Q T 8 7 S"));
	assertFalse("Show String should not be valid", ipp_validate_msg(REGEX_MSG_SHOW, "SHOW FLUSH T 9 8 6 22"));
	assertFalse("Show String should not be valid", ipp_validate_msg(REGEX_MSG_SHOW, "W FULLHOUSE Q 3"));
	assertFalse("Show String should not be valid", ipp_validate_msg(REGEX_MSG_SHOW, "R 9 8 6 2"));
	assertFalse("Show String should not be valid", ipp_validate_msg(REGEX_MSG_SHOW, ""));
	assertFalse("Show String should not be valid", ipp_validate_msg(REGEX_MSG_SHOW, "SHOW X 1 1"));

	ipp_exit();
	return PASS;
}
