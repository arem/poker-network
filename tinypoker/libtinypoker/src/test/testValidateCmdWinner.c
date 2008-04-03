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

	assertTrue("Winner String should be valid", ipp_validate_msg(REGEX_MSG_WINNER, "WINNER Alpha 1000 STRAIGHTFLUSH J"));
	assertTrue("Winner String should be valid", ipp_validate_msg(REGEX_MSG_WINNER, "WINNER Beta 1000 FOUROFAKIND 7 A"));
	assertTrue("Winner String should be valid", ipp_validate_msg(REGEX_MSG_WINNER, "WINNER Gamma 1000 FULLHOUSE Q 3"));
	assertTrue("Winner String should be valid", ipp_validate_msg(REGEX_MSG_WINNER, "WINNER BAGELBOT 1234 FLUSH T 9 8 6 2"));
	assertTrue("Winner String should be valid", ipp_validate_msg(REGEX_MSG_WINNER, "WINNER Coat_Hanger 1239 STRAIGHT K"));
	assertTrue("Winner String should be valid", ipp_validate_msg(REGEX_MSG_WINNER, "WINNER Amy 1239 THREEOFAKIND 2 Q 7"));
	assertTrue("Winner String should be valid", ipp_validate_msg(REGEX_MSG_WINNER, "WINNER Amy 1239 TWOPAIR J 8 A"));
	assertTrue("Winner String should be valid", ipp_validate_msg(REGEX_MSG_WINNER, "WINNER Amy 1239 ONEPAIR T A 5 3"));
	assertTrue("Winner String should be valid", ipp_validate_msg(REGEX_MSG_WINNER, "WINNER Amy 1239 HIGHCARD Q T 8 7 2"));

	assertFalse("Winner String should not be valid", ipp_validate_msg(REGEX_MSG_WINNER, "WINNER Amy 1239 HIGHCARD Q T 8 7 "));
	assertFalse("Winner String should not be valid", ipp_validate_msg(REGEX_MSG_WINNER, "WINNER Amy 1239 HIGHCARD Q T 8 7 S"));
	assertFalse("Winner String should not be valid", ipp_validate_msg(REGEX_MSG_WINNER, "WINNER 1239 ONEPAIR T A 5 3"));
	assertFalse("Winner String should not be valid", ipp_validate_msg(REGEX_MSG_WINNER, "WINNER BAGELBOT 1234 FLUSH T 9 8 6 22"));
	assertFalse("Winner String should not be valid", ipp_validate_msg(REGEX_MSG_WINNER, "W Gamma 1000 FULLHOUSE Q 3"));
	assertFalse("Winner String should not be valid", ipp_validate_msg(REGEX_MSG_WINNER, "R BAGELBOT 1234 FLUSH T 9 8 6 2"));
	assertFalse("Winner String should not be valid", ipp_validate_msg(REGEX_MSG_WINNER, ""));
	assertFalse("Winner String should not be valid", ipp_validate_msg(REGEX_MSG_WINNER, "WINNER X 1 1"));

	ipp_exit();
	return PASS;
}
