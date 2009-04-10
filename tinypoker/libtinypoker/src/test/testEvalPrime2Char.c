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

	assertEqual("Should have detected the proper value for prime.", '5', ipp_eval_prime2char(IPP_EVAL_5));
	assertEqual("Should have detected the proper value for prime.", 'A', ipp_eval_prime2char(IPP_EVAL_A));
	assertEqual("Should have detected the proper value for prime.", '3', ipp_eval_prime2char(IPP_EVAL_3));
	assertEqual("Should have detected the proper value for prime.", 'S', ipp_eval_prime2char(IPP_EVAL_S));

	assertNotEqual("Should not have detected the value it did.", 'A', ipp_eval_prime2char(IPP_EVAL_2));
	assertNotEqual("Should not have detected the value it did.", 'A', ipp_eval_prime2char(IPP_EVAL_K));

	ipp_exit();
	return PASS;
}
