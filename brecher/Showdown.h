/* Showdown.h */
/* Hold'Em Showdown by Steve Brecher <steve@brecher.reno.nv.us> */

#define kVersion "11Dec00.0"
/*
 *	11Dec00.0	Add additional user help.
 *  30Nov97.0   Work around CW Pro2 sscanf bug in Showdown.h:GetNextCard();
 *              Undo change of 07Oct97 and instead test value of d_flop_game
 *                  after #include of HandEval.h
 *  07Oct97.0   #define d_flop_game 1 before #include HandEval.h
 *  23Sep97.0   Fix typo in argument to SIOUXSetTitle.
 *  10Apr97.0   Fix runaway output when players > 4 -- bug introduced 11Dec96.0.
 *  11Dec96.0   Show 6 digits after decimal point.
 *              Output file creator R*ch (BBEdit).
 *  07Aug96.0   Change MaskT to ulong (from short) for PPC RAM access speed.
 *  04Aug96.0   AND only two suit masks to get quads bit in HandEval.
 *  14May96.0   Eliminate compiler warnings
 *  04May96.0   ANSI portability
 *              fix sense of resultsCreated test in Initialize()
 *  23May95.0   fix case of three pair and singleton to take kicker from higher
 *                  of 3rd-ranking pair or singleton instead of always from
 *                  3rd-ranking pair
 *  18Feb95.0   relax constraint on number of board cards specified from
 *                  0, 3, or 4 to 0 through 4.
 *  14Feb95.0   move poker logic routines nearer start of this file.
 *  10Feb95.1.0 change name of Enum2GuysNoBoard routine to Enum2GuysNoFlop;
 *              #include <time.h> iff kTimer.
 *  10Feb95.1   special case two players, no unknown, no flop.
 *  10Feb95.0   delete virginDeck -- superfluous;
 *              introduce Boolean nbrOfRanksGE5 array to help testing for flush;
 #              use bits 1 to 13 instead of 2 to 14 in masks;
 *              make lookup table elements longs instead of shorts;
 *              misc. minor optimizations, some due to Cliff Matthews et al;
 *              remove "unsigned" from MaskT -- big win in code gen;
 *              remove total pots from output and re-format output;
 *              InitTables:  remove knowledge of types from calls to calloc;
 *              fix typos in following line about 09Feb95.0.
 *  09Feb95.0   Enumerate:  fix logic bug in case 1 (1 unknown player).
 *  08Feb95.2   main: use WithCommas in too-many-enumerations error message,
 *                  thus fixing by removal a bad f format specifier.
 *  08Feb95.1   EnumBoardsNoUnknown: remove superfluous check in outer loop
 *                  for already-used deck card.
 *  08Feb95.0   mPotResults: improved (faster) logic w/r ties.
 *  07Feb95.0   first version without known bugs.
 */

#ifdef __MWERKS__
#ifdef macintosh
#define __Mac__ /* compiled on/for a Macintosh */
#if defined(powerc) || defined (__powerc)
#define __PPC__
#else
#define __68K__
#endif
#endif
#ifdef __INTEL__
#define __Windows__
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef THINK_C
#include <console.h>
#endif
#ifdef __MWERKS__
#ifdef __Mac__
#include <SIOUX.h>
#endif
#ifdef __Windows__
#include <WinSIOUX.h>
#endif
#endif
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#define k_hands_per_line 4

#define kTimer 1    /* output calculation time if non-zero */

#if kTimer
#ifdef __Mac__
#define __dest_os   __mac_os
#endif
#include <time.h>
#endif

#ifndef __Mac__
typedef unsigned char   Byte;
enum {false, true};
#define nil             NULL
#define kOutFileName    "SHOWDOWN.TXT"
#else
#define kOutFileName    "Showdown Results"
#define kFileCreator    'R*ch'
#endif

#ifndef __Mac__
#define IndicateWait()
#else
#define IndicateWait() SetCursor(*GetCursor(watchCursor))
#endif

#define kMaxPlayers 22 /* 5 board and 3 burn cards leaves 44 for hole cards */

#include "HandEval.h"
#if !d_flop_game
#error "d_flop_game is zero!"
#endif

#define kBotRankShift   (16)
#define kTopRankShift   (kBotRankShift+4)
#define kValueShift     (kTopRankShift+4)

#define BotRank(a)      (hiBotRank[a])
#define TopRank(a)      (hiTopRank[a])
#define Value(a)        ((long)a << kValueShift)


enum {kT = 10, kJ, kQ, kK, kA};

typedef struct {        /* struct for easy copying by assignment */
    HandT   hand;
} HandStrucT;

HandStrucT  board[5], holeHand[kMaxPlayers], zeroHand, deck[52],
            /* The following are each one card per hand so the cards can be
               output to results in the same order that user input them: */
            holeCard[2*kMaxPlayers], boardCard[4+1],
            deadCard[52-2-5+1]; /* +1s for excess input */
long        wins[kMaxPlayers], splits[kMaxPlayers];
int         nPlayers, nUnknown, nBoardCards, nCardsInDeck, nDeadCards;
double      partialPots[kMaxPlayers], nPots;
FILE        *out, *output[2];

void InitDeck(void);
void AddHandToHand(HandStrucT *hand1, HandStrucT *hand2);
void Enum2GuysNoFlop(void);
void EnumBoardsNoUnknown(void);
void EnumBoards(int nbrOfTimes);
void Enumerate(void);
void PrintCard(FILE *f, HandStrucT *h);
void General_Help(void);
int GetNextCard(char *string, int *offset, HandStrucT *h);
void ReadLine(char *s, int max);
Boolean GetResponse(char *prompt, char *s, int max, char *help_text);
Boolean GetHoleCards(int *nPlayers);
Boolean GetBoard(int *nBoardCards, HandStrucT *board);
Boolean GetDeadCards(int *nDeadCards);
Boolean GetNbrUnknown(int *nUnknown);
char *WithCommas(double n, char *s);
Boolean UserConfirm(double nDeals);
void Initialize(void);
void ResultsOut(void);
double Factorial(double n);
double Combos(int chooseFrom, int choose);
double NbrEnumerations(void);
