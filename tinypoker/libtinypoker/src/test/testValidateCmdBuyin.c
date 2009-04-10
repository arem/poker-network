/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <tcort@tomcort.com>
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

	assertTrue("Buyin String should be valid", ipp_validate_msg(REGEX_MSG_BUYIN, "BUYIN Alpha 1000"));
	assertTrue("Buyin String should be valid", ipp_validate_msg(REGEX_MSG_BUYIN, "BUYIN Beta 1000"));
	assertTrue("Buyin String should be valid", ipp_validate_msg(REGEX_MSG_BUYIN, "BUYIN Gamma 1000"));
	assertTrue("Buyin String should be valid", ipp_validate_msg(REGEX_MSG_BUYIN, "BUYIN BAGELBOT 1234"));
	assertTrue("Buyin String should be valid", ipp_validate_msg(REGEX_MSG_BUYIN, "BUYIN Coat_Hanger 1239"));

	assertTrue("Buyin String should be valid", ipp_validate_msg(REGEX_MSG_BUYIN, "BUYIN Alpha:FFF 1000"));
	assertTrue("Buyin String should be valid", ipp_validate_msg(REGEX_MSG_BUYIN, "BUYIN Beta:l37m31n 1000"));
	assertTrue("Buyin String should be valid", ipp_validate_msg(REGEX_MSG_BUYIN, "BUYIN Gamma:1337h4x04 1000"));
	assertTrue("Buyin String should be valid", ipp_validate_msg(REGEX_MSG_BUYIN, "BUYIN BAGELBOT:secret 1234"));
	assertTrue("Buyin String should be valid", ipp_validate_msg(REGEX_MSG_BUYIN, "BUYIN Coat_Hanger:abc123 1239"));

	assertFalse("Buyin String should not be valid", ipp_validate_msg(REGEX_MSG_BUYIN, ""));
	assertFalse("Buyin String should not be valid", ipp_validate_msg(REGEX_MSG_BUYIN, "BUYIN X 1 1"));
	assertFalse("Buyin String should not be valid", ipp_validate_msg(REGEX_MSG_BUYIN, "BUYIN Beta: 1000"));
	assertFalse("Buyin String should not be valid", ipp_validate_msg(REGEX_MSG_BUYIN, "BUYIN Beta! 1000"));
	assertFalse("Buyin String should not be valid", ipp_validate_msg(REGEX_MSG_BUYIN, "BUYIN Beta:! 1000"));

	ipp_exit();
	return PASS;
}
