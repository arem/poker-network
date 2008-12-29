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
	int msg_type;

	ipp_init();

	msg_type = ipp_validate_unknown_msg("YES STRAIGHTFLUSH J");
	assertNotEqual("Yes message should be valid", msg_type, -1);
	assertEqual("Yes message should have message type of MSG_YES", msg_type, MSG_YES);

	msg_type = ipp_validate_unknown_msg("FOLD");
	assertNotEqual("Fold message should be valid", msg_type, -1);
	assertEqual("Fold message should have message type of MSG_FOLD", msg_type, MSG_FOLD);

	ipp_exit();
	return PASS;
}
