/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This file is part of libtinypoker.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "../main/tinypoker.h"
#include "test.h"

int main()
{
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
	return PASS;
}
