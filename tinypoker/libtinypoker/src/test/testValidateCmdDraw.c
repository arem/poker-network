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
	assertTrue("Draw String should be valid", ipp_validate_msg(REGEX_MSG_DRAW, "DRAW 1 AS"));
	assertTrue("Draw String should be valid", ipp_validate_msg(REGEX_MSG_DRAW, "DRAW 2 AS KS"));
	assertTrue("Draw String should be valid", ipp_validate_msg(REGEX_MSG_DRAW, "DRAW 3 AS KS QS"));
	assertTrue("Draw String should be valid", ipp_validate_msg(REGEX_MSG_DRAW, "DRAW 4 AS KS QS JS"));
	assertTrue("Draw String should be valid", ipp_validate_msg(REGEX_MSG_DRAW, "DRAW 5 AS KS QS JS TS"));

	assertFalse("Draw String should not be valid", ipp_validate_msg(REGEX_MSG_DRAW, "DRAW 0"));
	assertFalse("Draw String should not be valid", ipp_validate_msg(REGEX_MSG_DRAW, "DRAW "));
	assertFalse("Draw String should not be valid", ipp_validate_msg(REGEX_MSG_DRAW, "DRAW"));
	assertFalse("Draw String should not be valid", ipp_validate_msg(REGEX_MSG_DRAW, "DRAW 1 AS KS"));
	assertFalse("Draw String should not be valid", ipp_validate_msg(REGEX_MSG_DRAW, "DRAW 2 AS"));
	assertFalse("Draw String should not be valid", ipp_validate_msg(REGEX_MSG_DRAW, "DRAW 6 AS KS QS JS TS 9S"));
	assertFalse("Draw String should not be valid", ipp_validate_msg(REGEX_MSG_DRAW, ""));
	return PASS;
}
