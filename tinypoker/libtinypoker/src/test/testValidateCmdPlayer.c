/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010 Thomas Cort <linuxgeek@gmail.com>
 *
 * This file is part of TinyPoker.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../main/tinypoker.h"
#include "test.h"

int main()
{
	ipp_init();

	assertTrue("Player String should be valid", ipp_validate_msg(REGEX_MSG_PLAYER, "PLAYER Alpha 1000"));
	assertTrue("Player String should be valid", ipp_validate_msg(REGEX_MSG_PLAYER, "PLAYER Beta 1000"));
	assertTrue("Player String should be valid", ipp_validate_msg(REGEX_MSG_PLAYER, "PLAYER Gamma 1000"));
	assertTrue("Player String should be valid", ipp_validate_msg(REGEX_MSG_PLAYER, "PLAYER Fred 1000"));
	assertTrue("Player String should be valid", ipp_validate_msg(REGEX_MSG_PLAYER, "PLAYER Scott 0"));
	assertTrue("Player String should be valid", ipp_validate_msg(REGEX_MSG_PLAYER, "PLAYER x 1"));
	assertTrue("Player String should be valid", ipp_validate_msg(REGEX_MSG_PLAYER, "PLAYER x 10000"));
	assertTrue("Player String should be valid", ipp_validate_msg(REGEX_MSG_PLAYER, "PLAYER 01234567890123456789012345678912 10000"));

	assertFalse("Player String should not be valid", ipp_validate_msg(REGEX_MSG_PLAYER, "PLAYER 0 Scott"));
	assertFalse("Player String should not be valid", ipp_validate_msg(REGEX_MSG_PLAYER, "PLAYER 012345678901234567890123456789121 10000"));
	assertFalse("Player String should not be valid", ipp_validate_msg(REGEX_MSG_PLAYER, "PLAYER S -1"));
	assertFalse("Player String should not be valid", ipp_validate_msg(REGEX_MSG_PLAYER, "PLAYER Scott 0123456789"));
	assertFalse("Player String should not be valid", ipp_validate_msg(REGEX_MSG_PLAYER, ""));

	ipp_exit();
	return PASS;
}
