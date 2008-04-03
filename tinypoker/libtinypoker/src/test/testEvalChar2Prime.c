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

	assertEqual("Should have detected the proper value for prime.", IPP_EVAL_S, ipp_eval_char2prime('S'));
	assertEqual("Should have detected the proper value for prime.", IPP_EVAL_A, ipp_eval_char2prime('A'));
	assertEqual("Should have detected the proper value for prime.", IPP_EVAL_3, ipp_eval_char2prime('3'));
	assertEqual("Should have detected the proper value for prime.", IPP_EVAL_T, ipp_eval_char2prime('T'));

	assertNotEqual("Should not have detected the value for prime.", IPP_EVAL_T, ipp_eval_char2prime('X'));
	assertNotEqual("Should not have detected the value for prime.", IPP_EVAL_2, ipp_eval_char2prime('3'));

	ipp_exit();
	return PASS;
}
