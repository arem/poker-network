/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This file is part of libtinypoker.
 *
 * libtinypoker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * libtinypoker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libtinypoker.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#ifndef __TINYPOKER_TEST_H
#define __TINYPOKER_TEST_H

#ifndef TRUE
/**
 * Boolean value for true.
 */
#define TRUE (1)
#endif

#ifndef FALSE
/**
 * Boolean value for false.
 */
#define FALSE (0)
#endif

#ifndef NULL
/**
 * 
 */
#define NULL (0)
#endif

#ifndef PASS
/**
 * Value returned by passing tests.
 */
#define PASS (0)
#endif

#ifndef FAIL
/**
 * Value returned by failing tests.
 */
#define FAIL (1)
#endif

/**
 * General assertion statement. Checks that parameter x is TRUE. If not,
 * exit() is called with FAIL.
 * @param w error message when the assertion fails
 * @param x expression to evaluate
 */
#define assert(w,x)                                                                             \
        do {                                                                                    \
                if ((x) == FALSE) {                                                             \
                        printf("(%s:%u) Assertion Failed: %s\n", __FILE__, __LINE__, w);        \
                        exit(FAIL);                                                             \
                }                                                                               \
        } while (0);

/**
 * Asserts that x is true.
 * @param w error message when the assertion fails
 * @param x expression to evaluate
 */
#define assertTrue(w,x)                 assert(w, (x == TRUE))

/**
 * Asserts that x is false.
 * @param w error message when the assertion fails
 * @param x expression to evaluate
 */
#define assertFalse(w,x)                assert(w, (x == FALSE))

/**
 * Asserts that x is equal to NULL.
 * @param w error message when the assertion fails
 * @param x expression to evaluate
 */
#define assertNull(w,x)                 assert(w, (x == NULL))

/**
 * Asserts that x is not equal to NULL.
 * @param w error message when the assertion fails
 * @param x expression to evaluate
 */
#define assertNotNull(w,x)              assert(w, (x != NULL))

/**
 * Asserts that x is equal to y.
 * @param w error message when the assertion fails
 * @param x value to compare to y.
 * @param y value to compare to x.
 */
#define assertEqual(w,x,y)              assert(w, (x == y))

/**
 * Asserts that x is not equal to y.
 * @param w error message when the assertion fails
 * @param x value to compare to y.
 * @param y value to compare to x.
 */
#define assertNotEqual(w,x,y)           assert(w, (x != y))

/**
 * Asserts that x is equal to y.
 * @param w error message when the assertion fails
 * @param x value to compare to y.
 * @param y value to compare to x.
 */
#define assertStringEqual(w,x,y)        assert(w, (!strcmp(x, y)))

/**
 * Asserts that x is not equal to y.
 * @param w error message when the assertion fails
 * @param x value to compare to y.
 * @param y value to compare to x.
 */
#define assertStringNotEqual(w,x,y)     assert(w, (strcmp(x, y)))

#endif
