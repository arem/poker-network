/* HandEval.h           by Steve Brecher    version 97Oct04.0 */

#ifndef d_HandEval_h    /* { */
#define d_HandEval_h
#pragma once

#include "HandEvalParms.h"

/*
 *  A hand is 0 or more cards represented by an array of five 32-bit masks.
 *  In each mask, bit 1 set (0x00000002) for a deuce, ..., bit 13 set
 *  (0x00002000) for an ace.  Bits 0 and 14..31 must be clear.  Little-
 *  endian bit numbering is used here.  The first mask is the bitwise OR of
 *  the following four, each of which denotes the ranks present in one of
 *  the suits.  The first mask thus denotes a summary of ranks present in
 *  the hand.  The subscripts of the array are:
 */
enum {kRanks, kClubs, kDiamonds, kHearts, kSpades, k_Nbr_of_masks};
/*
 *  Only the identifier kRanks is used in this module; strictly speaking
 *  the order of the suit elements of the array is immaterial.
 */
#define k_ace_mask      (0x00002000)
#define k_deuce_mask    (2)

enum {kNoPair, kPair, kTwoPair, kThreeOfAKind,
        kStraight, kFlush, kFullHouse, kFourOfAKind, kStraightFlush};

/*
 *  Evaluation result in 32 bits = 0VTBMMMM where:
 *
 *  V nybble = value code (kNoPair..kStraightFlush)
 *  T nybble = rank (2..14) of top pair for two pair, 0 otherwise
 *  B nybble = rank (2..14) of trips (including full house trips) or quads,
 *              or rank of bottom pair for two pair,
 *              or rank of pair for one pair,
 *              or 0 otherwise
 *  MMMM mask = 16-bit mask with...
 *              5 bits set for no pair, straight, flush, or straight flush
 *                  (for wheel straights/straight flushes, bit 0 rather than
 *                   13 is set for the ace),
 *              3 bits set for kickers with pair,
 */
#if d_flop_game
/*
 *              2 bits set for kickers with trips,
 *              1 bit set for pair with full house or kicker with quads or
 *                  two pair
 */
#else
/*
                or 0 otherwise
 */
#endif

typedef unsigned long   Mask_T;
typedef Mask_T          HandT[k_Nbr_of_masks];
typedef HandT           Hand_T;     /* use newer SB naming convention */

typedef unsigned long Eval_T;

#ifndef true
typedef unsigned char Boolean;
#define false 0
#define true 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if d_asian
d_lang Boolean Init_Asian_Eval(void);   /* returns false if heap allocation
                                            fails */
d_lang Eval_T Asian_5_Eval(HandT hand);
#else
d_lang Boolean Init_Hand_Eval(void);    /* returns false if heap allocation
                                            fails */
d_lang Eval_T Hand_7_Eval(HandT hand);
d_lang Eval_T Hand_5_Eval(HandT hand);
#endif

/* Utilities */
/* Init_..._Eval must have been previously called! */
/* mask argument must not exceed 0x3F80 */
Mask_T Hi_Card_Mask(short mask);    /* result is mask with all bits except
                                        highest reset */
int Number_Of_Ranks(short mask);    /* number of bits set in mask */
int Rank_Of_Hi_Card(short mask);    /* rank (0..kA) of highest bit set
                                        in mask */

#ifdef __cplusplus
}
#endif

#endif  /* } */
