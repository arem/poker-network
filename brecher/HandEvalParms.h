/* HandEvalParms.h      by Steve Brecher    version 30Nov97.0 */

#ifndef d_HandEvalParms_h
#define d_HandEvalParms_h
#pragma once

/* true for Asian stud: */
#ifndef d_asian
#define d_asian 0
#endif

/*
 *  If d_flop_game is zero then evaluation result does not include bits for
 *  non-playing kickers with quads or trips nor non-playing pair with full
 *  house.  This saves a little execution time.  I.e., set to zero for draw
 *  poker, stud poker, Chinese Poker; set to nonzero for hold 'em, Omaha,
 *  and other common-card games.
 */
#ifndef d_flop_game
#define d_flop_game 1
#endif

/* Function which allocates zero-filled bytes on the heap: */
/* optional for Macintosh: */
/* #define d_ram_alloc(quantity, size) NewPtrClear((quantity) * (size)) */
/* Portable ANSI C: */
#include <stdlib.h>
#define d_ram_alloc(quantity, size) calloc(quantity, size)


/* for C or C++, provide an empty definition: */
#define d_lang
/* For making a library to be used with Macintosh Pascal: */
/* #define d_lang pascal */

#endif
