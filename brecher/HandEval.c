/* HandEval.c           by Steve Brecher    version 97Oct04.0 */

#include "HandEval.h"

#define kBotRankShift   (16)
#define kTopRankShift   (kBotRankShift+4)
#define kValueShift     (kTopRankShift+4)

#define BotRank(a)      (hiBotRank[a])
#define TopRank(a)      (hiTopRank[a])
#define Value(a)        ((Mask_T)(a) << kValueShift)
#define kAce            (14)

#if d_flop_game
#define with(kickers) | (kickers)
#else
#define with(kickers)
#endif

/* Pointers to heap blocks for which subscript is bit mask of card ranks
   in hand: */
#define kArraySize (0x3F80 + 1)
static Boolean  *straight;      /* nonzero for all 5-, 6-, and 7-card hands
                                   containing straights */
static Mask_T   *theStraight;   /* Value(kStraight) | mask for the best
                                   5-card straight */
static char     *nbrOfRanks;    /* count of bits set */
static Boolean  *nbrOfRanksGE5; /* nonzero if number of bits set >= 5 */
static Mask_T   *hiTopRank;     /* Value(kTwoPair) | ((rank (2..kA) of the
                                   highest bit set) << kTopRankShift) */
static Mask_T   *hiBotRank;     /* (rank (2..kA) of the highest bit set)
                                   << kBotRankShift */
static Mask_T   *hiRankMask;    /* all bits except highest reset */
static Mask_T   *hi2RanksMask;  /* all bits except highest 2 reset */
static Mask_T   *hi3RanksMask;  /* all bits except highest 3 reset */
static Mask_T   *hi5RanksMask;  /* all bits except highest 5 reset */

#if d_asian
#define d_wheel 0x23C0 /* AT987 */
#else   /* { */
#define d_wheel 0x201E /* A5432 */

d_lang Eval_T Hand_7_Eval(HandT hand)
{
    Mask_T  i, j,  ranks, c, d, h, s;

    ranks = *hand++;
    c = *hand++;
    d = *hand++;
    h = *hand++;
    s = *hand;

    switch (nbrOfRanks[ranks]) {

        case 2: /* quads with trips kicker */
                i = c & d & h & s;  /* bit for quads */
                return Value(kFourOfAKind) | BotRank(i) with(i ^ ranks);

        case 3: /* trips and pair (full house) with non-playing pair,
                   or two trips (full house) with non-playing singleton,
                   or quads with pair and singleton */
                /* bits for singleton, if any, and trips, if any: */
                if (nbrOfRanks[i = c ^ d ^ h ^ s] == 3) {
                    /* two trips (full house) with non-playing singleton */
                    if (nbrOfRanks[i = c & d] != 2)
                        if (nbrOfRanks[i = c & h] != 2)
                            if (nbrOfRanks[i = c & s] != 2)
                                if (nbrOfRanks[i = d & h] != 2)
                                    if (nbrOfRanks[i = d & s] != 2)
                                        i = h & s;  /* bits for the trips */
                    return Value(kFullHouse) | BotRank(i)
                        with(i ^ hiRankMask[i]); }
                if ((j = c & d & h & s) != 0)   /* bit for quads */
                    /* quads with pair and singleton */
                    return Value(kFourOfAKind) | BotRank(j)
                        with(hiRankMask[ranks ^ j]);
                /* trips and pair (full house) with non-playing pair */
                return Value(kFullHouse) | BotRank(i)
                    with(hiRankMask[ranks ^ i]);

        case 4: /* three pair and singleton,
                   or trips and pair (full house) and two non-playing
                    singletons,
                   or quads with singleton kicker and two non-playing
                    singletons */
                i = c ^ d ^ h ^ s; /* the bit(s) of the trips, if any,
                                    and singleton(s) */
                if (nbrOfRanks[i] == 1) {   
                    /* three pair and singleton */
                    j = ranks ^ i;      /* the three bits for the pairs */
                    ranks = hiRankMask[j];  /* bit for the top pair */
                    j ^= ranks;         /* bits for the two bottom pairs */
                    return TopRank(ranks)   /* contains Value(kTwoPair) */
                        | BotRank(j) | hiRankMask[(hiRankMask[j] ^ j) | i]; }
                if (!(j = c & d & h & s)) {
                    /* trips and pair (full house) and two non-playing
                        singletons */
                    i ^= ranks;     /* bit for the pair */
                    if (!(j = (c & d) & (~i)))
                        j = (h & s) & (~i); /* bit for the trips */
                    return Value(kFullHouse) | BotRank(j) with(i); }
                /* quads with singleton kicker and two
                    non-playing singletons */
                return Value(kFourOfAKind) | BotRank(j) with(hiRankMask[i]);

#define mFlushOrStraightFlush(x)                                        \
                if (nbrOfRanksGE5[x])                                   \
                    if (!straight[x])                                   \
                        return Value(kFlush) | hi5RanksMask[x];         \
                    else                                                \
                        return Value(kStraightFlush                     \
                            | kStraight) ^ theStraight[x];
#define mStraightAndOrFlush                                             \
                mFlushOrStraightFlush(c)                                \
                mFlushOrStraightFlush(d)                                \
                mFlushOrStraightFlush(h)                                \
                mFlushOrStraightFlush(s)                                \
                if (straight[ranks])                                    \
                    return /* Value(kStraight) is in: */ theStraight[ranks];

        case 5: /* flush and/or straight,
                   or two pair and three singletons,
                   or trips and four singletons */
                mStraightAndOrFlush
                i = c ^ d ^ h ^ s; /* the bits of the trips, if any,
                                      and singletons */
                if (!nbrOfRanksGE5[i]) {
                    /* two pair and three singletons */
                    j = i ^ ranks;  /* the two bits for the pairs */
                    return TopRank(j)   /* contains Value(kTwoPair) */
                            | BotRank(hiRankMask[j] ^ j) | hiRankMask[i]; }
                /* trips and four singletons */
                if (!(j = c & d))
                    j = h & s;
                return Value(kThreeOfAKind) | BotRank(j)
                    with(hi2RanksMask[i ^ j]);

        case 6: /* flush and/or straight,
                   or one pair and three kickers and
                    two nonplaying singletons */
                mStraightAndOrFlush
                i = c ^ d ^ h ^ s; /* the bits of the five singletons */
                return Value(kPair) | BotRank(ranks ^ i) | hi3RanksMask[i];

        case 7: /* flush and/or straight or no pair */
                mStraightAndOrFlush
                return /* Value(kNoPair) | */ hi5RanksMask[ranks];

        } /* end switch */

    return 0; /* never reached, but avoids compiler warning */
}
#endif  /* } */

#if d_asian
d_lang Eval_T Asian_5_Eval(HandT hand)
#else
d_lang Eval_T Hand_5_Eval(HandT hand)
#endif
{
    Mask_T  i, j, ranks, c, d, h, s;

    ranks = *hand++;
    c = *hand++;
    d = *hand++;
    h = *hand++;
    s = *hand;

    switch (nbrOfRanks[ranks]) {

        case 2: /* quads or full house */
                i = c & d;
                if (!(i & h & s)) {     /* no bit common to all suits */
                    i = c ^ d ^ h ^ s;  /* trips bit */
                    return Value(kFullHouse) | BotRank(i) with(i ^ ranks); }
                else
                    /* the quads bit must be present in each suit mask,
                       but the kicker bit in no more than one; so we need
                       only AND any two suit masks: */
                    return Value(kFourOfAKind) | BotRank(i) with(i ^ ranks);

        case 3: /* trips and two kickers,
                   or two pair and kicker */
                if ((i = c ^ d ^ h ^ s) == ranks) {
                    /* trips and two kickers */
                    if ((i = c & d) != 0)
                        return Value(kThreeOfAKind)
                            | BotRank(i) with(i ^ ranks);
                    if ((i = c & h) != 0)
                        return Value(kThreeOfAKind) 
                            | BotRank(i) with(i ^ ranks);
                    i = d & h;
                    return Value(kThreeOfAKind) | BotRank(i)
                        with(i ^ ranks); }
                /* two pair and kicker; i has kicker bit */
                j = i ^ ranks;      /* j has pairs bits */
                return TopRank(j) | BotRank(j ^ hiRankMask[j]) | i;

        case 4: /* pair and three kickers */
                i = c ^ d ^ h ^ s; /* kicker bits */
                return Value(kPair) | BotRank(ranks ^ i) | i;

        case 5: /* flush and/or straight,
                   or no pair */
                if (ranks == c || ranks == d || ranks == h || ranks == s)
                    if (!straight[ranks])
                        return Value(kFlush) | ranks;
                    else
                        return Value(kStraightFlush
                            | kStraight) ^ theStraight[ranks];
                if (straight[ranks])
                    return /* Value(kStraight) is in: */ theStraight[ranks];
                return /* Value(kNoPair) | */ ranks; }

    return 0; /* never reached, but avoids compiler warning */
}

static void SetStraight(int ts)
/* must call with ts from A..T to 5..A in that order */
{
    int es, i, j;

    for (i = 0x2000; i >= 0x0002; i >>= 1)
        for (j = 0x2000; j >= 0x0002; j >>= 1) {
            es = i | j | ts;
            straight[es] = true;
            if (!theStraight[es])
                if (ts == d_wheel)
                    /* Move the ace from bit 13 to bit 0 */
                    theStraight[es] = Value(kStraight)
                                        | (d_wheel & 0x1FFF) | 0x0001;
                else
                    theStraight[es] = Value(kStraight) | ts; }
}

#if d_asian
d_lang Boolean Init_Asian_Eval(void)
#else
d_lang Boolean Init_Hand_Eval(void)
#endif
{
    int     mask, bitCount;
    Mask_T  shiftReg, i;

#define mAllocate(type, array)                                  \
    array = (type *)(d_ram_alloc(kArraySize, sizeof(*array)));  \
    if (!array) return false

    mAllocate(Boolean, straight);
    mAllocate(Mask_T, theStraight);
    mAllocate(char, nbrOfRanks);
    mAllocate(Boolean, nbrOfRanksGE5);
    mAllocate(Mask_T, hiBotRank);
    mAllocate(Mask_T, hiTopRank);
    mAllocate(Mask_T, hiRankMask);
    mAllocate(Mask_T, hi2RanksMask);
    mAllocate(Mask_T, hi3RanksMask);
    mAllocate(Mask_T, hi5RanksMask);

    for (mask = 0x3E00/*A..T*/; mask >= 0x003E/*6..2*/; mask >>= 1)
        SetStraight(mask);
    SetStraight(d_wheel);       /* A,5..2 */

    for (mask = 0x0002/*a deuce*/; mask < kArraySize; ++mask) {
        bitCount = 0;
        shiftReg = mask;
        for (i = kAce; i >= 2; --i, shiftReg <<= 1)
            if (shiftReg & 0x2000)
                switch (++bitCount) {
                    case 1: hiBotRank[mask] = i << kBotRankShift;
                            hiTopRank[mask] = Value(kTwoPair) 
                                                | (i << kTopRankShift);
                            hiRankMask[mask] = 0x2000 >> (kAce - i);
                            break;
                    case 2: hi2RanksMask[mask] = (shiftReg & 0x07FFE000)
                                                    >> (kAce - i);
                            break;
                    case 3: hi3RanksMask[mask] = (shiftReg & 0x07FFE000)
                                                    >> (kAce - i);
                            break;
                    case 5: hi5RanksMask[mask] = (shiftReg & 0x07FFE000)
                                                    >> (kAce - i); }
        if (bitCount >= 5)
            nbrOfRanksGE5[mask] = true;
        nbrOfRanks[mask] = bitCount; }

    return true;
}

Mask_T Hi_Card_Mask(short mask)
{
    return hiRankMask[mask];
}

int Number_Of_Ranks(short mask)
{
    return nbrOfRanks[mask];
}

int Rank_Of_Hi_Card(short mask)
{
    return hiBotRank[mask] >> kBotRankShift;
}
