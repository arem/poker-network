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

	assertEqual("Should have detected straight.", '5', ipp_eval_straight2char(IPP_EVAL_A * IPP_EVAL_2 * IPP_EVAL_3 * IPP_EVAL_4 * IPP_EVAL_5));
	assertEqual("Should have detected straight.", '7', ipp_eval_straight2char(IPP_EVAL_3 * IPP_EVAL_4 * IPP_EVAL_5 * IPP_EVAL_6 * IPP_EVAL_7));
	assertEqual("Should have detected straight.", 'T', ipp_eval_straight2char(IPP_EVAL_6 * IPP_EVAL_7 * IPP_EVAL_8 * IPP_EVAL_9 * IPP_EVAL_T));
	assertEqual("Should have detected straight.", 'A', ipp_eval_straight2char(IPP_EVAL_T * IPP_EVAL_J * IPP_EVAL_Q * IPP_EVAL_K * IPP_EVAL_A));
	assertEqual("Should have detected straight.", 'A', ipp_eval_straight2char(IPP_EVAL_A * IPP_EVAL_J * IPP_EVAL_K * IPP_EVAL_Q * IPP_EVAL_T));
	assertNotEqual("Should have detected straight.", 'X', ipp_eval_straight2char(IPP_EVAL_A * IPP_EVAL_J * IPP_EVAL_K * IPP_EVAL_Q * IPP_EVAL_T));

	assertNotEqual("Should not have detected straight.", 'A', ipp_eval_straight2char(IPP_EVAL_3 * IPP_EVAL_J * IPP_EVAL_K * IPP_EVAL_Q * IPP_EVAL_T));
	assertNotEqual("Should not have detected straight.", 'K', ipp_eval_straight2char(IPP_EVAL_3 * IPP_EVAL_J * IPP_EVAL_K * IPP_EVAL_Q * IPP_EVAL_T));
	assertNotEqual("Should not have detected straight.", '3', ipp_eval_straight2char(IPP_EVAL_3 * IPP_EVAL_J * IPP_EVAL_K * IPP_EVAL_Q * IPP_EVAL_S));

	ipp_exit();
	return PASS;
}
