/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <linuxgeek@gmail.com>
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

	assertTrue("BeatQ String should be valid", ipp_validate_msg(REGEX_MSG_BEATQ, "BEAT? STRAIGHTFLUSH J"));
	assertTrue("BeatQ String should be valid", ipp_validate_msg(REGEX_MSG_BEATQ, "BEAT? FOUROFAKIND 7 A"));
	assertTrue("BeatQ String should be valid", ipp_validate_msg(REGEX_MSG_BEATQ, "BEAT? FULLHOUSE Q 3"));
	assertTrue("BeatQ String should be valid", ipp_validate_msg(REGEX_MSG_BEATQ, "BEAT? FLUSH T 9 8 6 2"));
	assertTrue("BeatQ String should be valid", ipp_validate_msg(REGEX_MSG_BEATQ, "BEAT? STRAIGHT K"));
	assertTrue("BeatQ String should be valid", ipp_validate_msg(REGEX_MSG_BEATQ, "BEAT? THREEOFAKIND 2 Q 7"));
	assertTrue("BeatQ String should be valid", ipp_validate_msg(REGEX_MSG_BEATQ, "BEAT? TWOPAIR J 8 A"));
	assertTrue("BeatQ String should be valid", ipp_validate_msg(REGEX_MSG_BEATQ, "BEAT? ONEPAIR T A 5 3"));
	assertTrue("BeatQ String should be valid", ipp_validate_msg(REGEX_MSG_BEATQ, "BEAT? HIGHCARD Q T 8 7 2"));

	assertFalse("BeatQ String should not be valid", ipp_validate_msg(REGEX_MSG_BEATQ, "BEAT? HIGHCARD Q T 8 7 "));
	assertFalse("BeatQ String should not be valid", ipp_validate_msg(REGEX_MSG_BEATQ, "BEAT? HIGHCARD Q T 8 7 S"));
	assertFalse("BeatQ String should not be valid", ipp_validate_msg(REGEX_MSG_BEATQ, "BEAT? FLUSH T 9 8 6 22"));
	assertFalse("BeatQ String should not be valid", ipp_validate_msg(REGEX_MSG_BEATQ, "? FULLHOUSE Q 3"));
	assertFalse("BeatQ String should not be valid", ipp_validate_msg(REGEX_MSG_BEATQ, "B? 9 8 6 2"));
	assertFalse("BeatQ String should not be valid", ipp_validate_msg(REGEX_MSG_BEATQ, ""));
	assertFalse("BeatQ String should not be valid", ipp_validate_msg(REGEX_MSG_BEATQ, "BEAT? X 1 1"));

	ipp_exit();
	return PASS;
}
