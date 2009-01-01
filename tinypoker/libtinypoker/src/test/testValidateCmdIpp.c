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

	assertTrue("Server String should be valid", ipp_validate_msg(REGEX_MSG_IPP, "IPP 2.0 TinyPoker/0.0.0"));
	assertTrue("Server String should be valid", ipp_validate_msg(REGEX_MSG_IPP, "IPP 2.0 Cheese"));

	assertFalse("Server String should not be valid", ipp_validate_msg(REGEX_MSG_IPP, "IPP\n2.0 TinyPoker/0.0.0"));
	assertFalse("Server String should not be valid", ipp_validate_msg(REGEX_MSG_IPP, "IPP 2.0 "));
	assertFalse("Server String should not be valid", ipp_validate_msg(REGEX_MSG_IPP, "IPP 2 TinyPoker/0.0.0"));
	assertFalse("Server String should not be valid", ipp_validate_msg(REGEX_MSG_IPP, "IPP OK TinyPoker/0.0.0"));
	assertFalse("Server String should not be valid", ipp_validate_msg(REGEX_MSG_IPP, ""));

	ipp_exit();
	return PASS;
}
