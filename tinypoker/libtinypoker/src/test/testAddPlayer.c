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
	ipp_player *player;
	ipp_table *table;
	int rc;

	ipp_init();

	table = ipp_new_table();
	player = ipp_new_player();

	assertNotNull("Table should not be null.", table);
	assertNotNull("Player should not be null.", player);
	assertEqual("Table shouldn't have any players", table->nplayers, 0);

	rc = ipp_add_player(table, player);
	assertNotEqual("Should have been able to add player", rc, -1);
	assertEqual("Should have one player", table->nplayers, 1);

	ipp_free_table(table);	/* will also free player */

	ipp_exit();
	return PASS;
}
