/* Showdown.c */

#include "Showdown.h"

void InitDeck(void)
{
    HandStrucT  *h;
    int         rank, suit;

    h = deck;
    for (suit = kClubs; suit <= kSpades; ++suit)
        for (rank = 2; rank <= kA; ++rank, ++h) {
            *h = zeroHand;
            h->hand[kRanks] = h->hand[suit] = (1 << (rank - 1)); }
}

#ifdef __68K__
#pragma parameter AddHandToHand(__A0,__A1)
#endif
void AddHandToHand(HandStrucT *hand1, HandStrucT *hand2)
#ifndef __68K__
{
    Mask_T  *h1 = hand1->hand, *h2 = hand2->hand;
    
    *h2++ |= *h1++; *h2++ |= *h1++; *h2++ |= *h1++; *h2++ |= *h1++; *h2 |= *h1;
}
#else
    /* 68000 code compiled inline in place of subroutine call: */
    = { 0x2018, //Move.L    (A0)+,D0
        0x8199, //Or.L      D0,(A1)+
        0x2018, //Move.L    (A0)+,D0
        0x8199, //Or.L      D0,(A1)+
        0x2018, //Move.L    (A0)+,D0
        0x8199, //Or.L      D0,(A1)+
        0x2018, //Move.L    (A0)+,D0
        0x8199, //Or.L      D0,(A1)+
        0x2018, //Move.L    (A0)+,D0
        0x8199  //Or.L      D0,(A1)+
    }; 
#endif

void Enum2GuysNoFlop(void)  /* special case for speed of EnumBoardsNoUnknown */
{
    HandStrucT  *deckP[5], *lastCardP[5], player;
    Eval_T      handValue0, handValue1;

    lastCardP[4] = &deck[nCardsInDeck - 1];
    lastCardP[3] = lastCardP[4] - 1;
    lastCardP[2] = lastCardP[3] - 1;
    lastCardP[1] = lastCardP[2] - 1;
    lastCardP[0] = lastCardP[1] - 1;
    for (deckP[0] = deck; deckP[0] <= lastCardP[0]; ++deckP[0]) {
        board[0] = *deckP[0];
        for (deckP[1] = deckP[0] + 1; deckP[1] <= lastCardP[1]; ++deckP[1]) {
            board[1] = board[0];
            AddHandToHand(deckP[1], &board[1]);
            for (deckP[2] = deckP[1] + 1; deckP[2] <= lastCardP[2]; ++deckP[2]) {
                board[2] = board[1];
                AddHandToHand(deckP[2], &board[2]);
                for (deckP[3] = deckP[2] + 1; deckP[3] <= lastCardP[3]; ++deckP[3]) {
                    board[3] = board[2];
                    AddHandToHand(deckP[3], &board[3]);
                    for (deckP[4] = deckP[3] + 1; deckP[4] <= lastCardP[4]; ++deckP[4]) {
                        board[4] = board[3];
                        AddHandToHand(deckP[4], &board[4]);
                        player = board[4];
                        AddHandToHand(&holeHand[0], &player);
                        handValue0 = Hand_7_Eval(player.hand);
                        player = board[4];
                        AddHandToHand(&holeHand[1], &player);
                        handValue1 = Hand_7_Eval(player.hand);
                        /* ResultsOut will fill in wins[1], splits[1], and calc partialPots */
                        if (handValue0 > handValue1)
                            ++wins[0];
                        else
                            if (handValue0 == handValue1)
                                ++splits[0]; } } } } }
}

#define mPotResults(nTimes)                             \
    {                                                   \
    Eval_T      eval, bestEval, handValue[kMaxPlayers]; \
    int         i, winningPlayer, waysSplit;            \
    HandStrucT  player;                                 \
    double      partialPot;                             \
                                                        \
    bestEval = 0;                                       \
    for (i = 0; i < nPlayers; ++i) {                    \
        player = board[4];                              \
        AddHandToHand(&holeHand[i], &player);           \
        handValue[i] = eval = Hand_7_Eval(player.hand); \
        if (eval > bestEval) {                          \
            bestEval = eval;                            \
            waysSplit = 0;                              \
            winningPlayer = i; }                        \
        else                                            \
            if (eval == bestEval)                       \
                ++waysSplit; }                          \
    if (!waysSplit)                                     \
        wins[winningPlayer] += nTimes;                  \
    else {                                              \
        partialPot = (double)nTimes/++waysSplit;        \
        for (i = 0; waysSplit; ++i)                     \
            if (handValue[i] == bestEval) {             \
                partialPots[i] += partialPot;           \
                splits[i] += nTimes;                    \
                --waysSplit; } } }

void EnumBoardsNoUnknown(void)
/*  This is the same as EnumBoards (below) except there is no test for already-used deck
    cards.  This one is called when there are no players with unspecified hole cards. */
{
    HandStrucT  *deckP[5], *lastCardP[5];

    switch (nBoardCards) {
        case 0:
            if (nPlayers == 2) {
                Enum2GuysNoFlop();  /* special case */
                break; }
            lastCardP[4] = &deck[nCardsInDeck - 1];
            lastCardP[3] = lastCardP[4] - 1;
            lastCardP[2] = lastCardP[3] - 1;
            lastCardP[1] = lastCardP[2] - 1;
            lastCardP[0] = lastCardP[1] - 1;
            for (deckP[0] = deck; deckP[0] <= lastCardP[0]; ++deckP[0]) {
                board[0] = *deckP[0];
                for (deckP[1] = deckP[0] + 1; deckP[1] <= lastCardP[1]; ++deckP[1]) {
                    board[1] = board[0];
                    AddHandToHand(deckP[1], &board[1]);
                    for (deckP[2] = deckP[1] + 1; deckP[2] <= lastCardP[2]; ++deckP[2]) {
                        board[2] = board[1];
                        AddHandToHand(deckP[2], &board[2]);
                        for (deckP[3] = deckP[2] + 1; deckP[3] <= lastCardP[3]; ++deckP[3]) {
                            board[3] = board[2];
                            AddHandToHand(deckP[3], &board[3]);
                            for (deckP[4] = deckP[3] + 1; deckP[4] <= lastCardP[4]; ++deckP[4]) {
                                board[4] = board[3];
                                AddHandToHand(deckP[4], &board[4]);
                                mPotResults(1) } } } } }
            break;
        case 1:
            lastCardP[4] = &deck[nCardsInDeck - 1];
            lastCardP[3] = lastCardP[4] - 1;
            lastCardP[2] = lastCardP[3] - 1;
            lastCardP[1] = lastCardP[2] - 1;
            for (deckP[1] = deck; deckP[1] <= lastCardP[1]; ++deckP[1]) {
                board[1] = board[0];
                AddHandToHand(deckP[1], &board[1]);
                for (deckP[2] = deckP[1] + 1; deckP[2] <= lastCardP[2]; ++deckP[2]) {
                    board[2] = board[1];
                    AddHandToHand(deckP[2], &board[2]);
                    for (deckP[3] = deckP[2] + 1; deckP[3] <= lastCardP[3]; ++deckP[3]) {
                        board[3] = board[2];
                        AddHandToHand(deckP[3], &board[3]);
                        for (deckP[4] = deckP[3] + 1; deckP[4] <= lastCardP[4]; ++deckP[4]) {
                            board[4] = board[3];
                            AddHandToHand(deckP[4], &board[4]);
                            mPotResults(1) } } } }
            break;
        case 2:
            lastCardP[4] = &deck[nCardsInDeck - 1];
            lastCardP[3] = lastCardP[4] - 1;
            lastCardP[2] = lastCardP[3] - 1;
            for (deckP[2] = deck; deckP[2] <= lastCardP[2]; ++deckP[2]) {
                board[2] = board[0];
                AddHandToHand(deckP[2], &board[2]);
                for (deckP[3] = deckP[2] + 1; deckP[3] <= lastCardP[3]; ++deckP[3]) {
                    board[3] = board[2];
                    AddHandToHand(deckP[3], &board[3]);
                    for (deckP[4] = deckP[3] + 1; deckP[4] <= lastCardP[4]; ++deckP[4]) {
                        board[4] = board[3];
                        AddHandToHand(deckP[4], &board[4]);
                        mPotResults(1) } } }
            break;
        case 3:
            lastCardP[4] = &deck[nCardsInDeck - 1];
            lastCardP[3] = lastCardP[4] - 1;
            for (deckP[3] = deck; deckP[3] <= lastCardP[3]; ++deckP[3]) {
                board[3] = board[0];
                AddHandToHand(deckP[3], &board[3]);
                for (deckP[4] = deckP[3] + 1; deckP[4] <= lastCardP[4]; ++deckP[4]) {
                    board[4] = board[3];
                    AddHandToHand(deckP[4], &board[4]);
                    mPotResults(1) } }
            break;
        case 4:
            lastCardP[4] = &deck[nCardsInDeck - 1];
            for (deckP[4] = deck; deckP[4] <= lastCardP[4]; ++deckP[4]) {
                board[4] = board[0];
                AddHandToHand(deckP[4], &board[4]);
                mPotResults(1) }
    } /* end switch */
}

void EnumBoards(int nbrOfTimes)
{
    HandStrucT  *deckP[5], *lastCardP[5];

    switch (nBoardCards) {
        case 0:
            lastCardP[4] = &deck[nCardsInDeck - 1];
            lastCardP[3] = lastCardP[4] - 1;
            lastCardP[2] = lastCardP[3] - 1;
            lastCardP[1] = lastCardP[2] - 1;
            lastCardP[0] = lastCardP[1] - 1;
            for (deckP[0] = deck; deckP[0] <= lastCardP[0]; ++deckP[0]) {
                if (!deckP[0]->hand[kRanks])
                    continue;
                board[0] = *deckP[0];
                for (deckP[1] = deckP[0] + 1; deckP[1] <= lastCardP[1]; ++deckP[1]) {
                    if (!deckP[1]->hand[kRanks])
                        continue;
                    board[1] = board[0];
                    AddHandToHand(deckP[1], &board[1]);
                    for (deckP[2] = deckP[1] + 1; deckP[2] <= lastCardP[2]; ++deckP[2]) {
                        if (!deckP[2]->hand[kRanks])
                            continue;
                        board[2] = board[1];
                        AddHandToHand(deckP[2], &board[2]);
                        for (deckP[3] = deckP[2] + 1; deckP[3] <= lastCardP[3]; ++deckP[3]) {
                            if (!deckP[3]->hand[kRanks])
                                continue;
                            board[3] = board[2];
                            AddHandToHand(deckP[3], &board[3]);
                            for (deckP[4] = deckP[3] + 1; deckP[4] <= lastCardP[4]; ++deckP[4]) {
                                if (!deckP[4]->hand[kRanks])
                                    continue;
                                board[4] = board[3];
                                AddHandToHand(deckP[4], &board[4]);
                                mPotResults(nbrOfTimes) } } } } }
            break;
        case 1:
            lastCardP[4] = &deck[nCardsInDeck - 1];
            lastCardP[3] = lastCardP[4] - 1;
            lastCardP[2] = lastCardP[3] - 1;
            lastCardP[1] = lastCardP[2] - 1;
            for (deckP[1] = deck; deckP[1] <= lastCardP[1]; ++deckP[1]) {
                if (!deckP[1]->hand[kRanks])
                    continue;
                board[1] = board[0];
                AddHandToHand(deckP[1], &board[1]);
                for (deckP[2] = deckP[1] + 1; deckP[2] <= lastCardP[2]; ++deckP[2]) {
                    if (!deckP[2]->hand[kRanks])
                        continue;
                    board[2] = board[1];
                    AddHandToHand(deckP[2], &board[2]);
                    for (deckP[3] = deckP[2] + 1; deckP[3] <= lastCardP[3]; ++deckP[3]) {
                        if (!deckP[3]->hand[kRanks])
                            continue;
                        board[3] = board[2];
                        AddHandToHand(deckP[3], &board[3]);
                        for (deckP[4] = deckP[3] + 1; deckP[4] <= lastCardP[4]; ++deckP[4]) {
                            if (!deckP[4]->hand[kRanks])
                                continue;
                            board[4] = board[3];
                            AddHandToHand(deckP[4], &board[4]);
                            mPotResults(nbrOfTimes) } } } }
            break;
        case 2:
            lastCardP[4] = &deck[nCardsInDeck - 1];
            lastCardP[3] = lastCardP[4] - 1;
            lastCardP[2] = lastCardP[3] - 1;
            for (deckP[2] = deck; deckP[2] <= lastCardP[2]; ++deckP[2]) {
                if (!deckP[2]->hand[kRanks])
                    continue;
                board[2] = board[0];
                AddHandToHand(deckP[2], &board[2]);
                for (deckP[3] = deckP[2] + 1; deckP[3] <= lastCardP[3]; ++deckP[3]) {
                    if (!deckP[3]->hand[kRanks])
                        continue;
                    board[3] = board[2];
                    AddHandToHand(deckP[3], &board[3]);
                    for (deckP[4] = deckP[3] + 1; deckP[4] <= lastCardP[4]; ++deckP[4]) {
                        if (!deckP[4]->hand[kRanks])
                            continue;
                        board[4] = board[3];
                        AddHandToHand(deckP[4], &board[4]);
                        mPotResults(nbrOfTimes) } } }
            break;
        case 3:
            lastCardP[4] = &deck[nCardsInDeck - 1];
            lastCardP[3] = lastCardP[4] - 1;
            for (deckP[3] = deck; deckP[3] <= lastCardP[3]; ++deckP[3]) {
                if (!deckP[3]->hand[kRanks])
                    continue;
                board[3] = board[0];
                AddHandToHand(deckP[3], &board[3]);
                for (deckP[4] = deckP[3] + 1; deckP[4] <= lastCardP[4]; ++deckP[4]) {
                    if (!deckP[4]->hand[kRanks])
                        continue;
                    board[4] = board[3];
                    AddHandToHand(deckP[4], &board[4]);
                    mPotResults(nbrOfTimes) } }
            break;
        case 4:
            lastCardP[4] = &deck[nCardsInDeck - 1];
            for (deckP[4] = deck; deckP[4] <= lastCardP[4]; ++deckP[4]) {
                if (!deckP[4]->hand[kRanks])
                    continue;
                board[4] = board[0];
                AddHandToHand(deckP[4], &board[4]);
                mPotResults(nbrOfTimes) }
    } /* end switch */
}

void Enumerate(void)
{
    int         i, firstUnknown = nPlayers - nUnknown;
    HandStrucT  *deckP[4], *lastCardP[4], card[4];

    for (i = 0; i < nPlayers; ++i) {
        wins[i] = splits[i] = 0;
        partialPots[i] = 0.0; }

    /* Compact deck array to remove elements for user-specified hole/board/dead cards */
    deckP[0] = deckP[1] = deck;
    for (i = 0; i < 52; ++i, ++deckP[1])
        if (deckP[1]->hand[kRanks])
            *deckP[0]++ = *deckP[1];

    switch (nUnknown) {
        case 0:
            EnumBoardsNoUnknown();  /* no check for missing cards in deck; faster */
            break;
        case 1:
            lastCardP[1] = &deck[nCardsInDeck - 1];
            lastCardP[0] = lastCardP[1] - 1;
            for (deckP[0] = deck; deckP[0] <= lastCardP[0]; ++deckP[0]) {
                holeHand[firstUnknown] = *deckP[0];
                card[0] = *deckP[0];
                deckP[0]->hand[kRanks] = 0;
                for (deckP[1] = deckP[0] + 1; deckP[1] <= lastCardP[1]; ++deckP[1]) {
                    holeHand[firstUnknown] = card[0];
                    AddHandToHand(deckP[1], &holeHand[firstUnknown]);
                    card[1].hand[kRanks] = deckP[1]->hand[kRanks];
                    deckP[1]->hand[kRanks] = 0;
                    EnumBoards(1);
                    deckP[1]->hand[kRanks] = card[1].hand[kRanks]; }
                deckP[0]->hand[kRanks] = card[0].hand[kRanks]; }
            break;
        case 2:
            lastCardP[3] = &deck[nCardsInDeck - 1];
            lastCardP[2] = lastCardP[3] - 1;
            lastCardP[1] = lastCardP[2] - 1;
            lastCardP[0] = lastCardP[1] - 1;
            for (deckP[0] = deck; deckP[0] <= lastCardP[0]; ++deckP[0]) {
                card[0] = *deckP[0];
                deckP[0]->hand[kRanks] = 0;
                for (deckP[1] = deckP[0] + 1; deckP[1] <= lastCardP[1]; ++deckP[1]) {
                    card[1] = *deckP[1];
                    deckP[1]->hand[kRanks] = 0;
                    for (deckP[2] = deckP[1] + 1; deckP[2] <= lastCardP[2]; ++deckP[2]) {
                        card[2] = *deckP[2];
                        deckP[2]->hand[kRanks] = 0;
                        for (deckP[3] = deckP[2] + 1; deckP[3] <= lastCardP[3]; ++deckP[3]) {
                            card[3] = *deckP[3];
                            deckP[3]->hand[kRanks] = 0;
                            holeHand[firstUnknown] = card[0];
                            AddHandToHand(&card[1], &holeHand[firstUnknown]);
                            holeHand[firstUnknown+1] = card[2];
                            AddHandToHand(&card[3], &holeHand[firstUnknown+1]);
                            /* The first "unknown" holding wx and the second holding yz is equivalent,
                               w/r the results of interest, to the first holding yz and the second wx.
                               So we just enumerate boards for one of the two cases and tell
                               the enumeration routine to tally the results twice. */
                            EnumBoards(2);
                            holeHand[firstUnknown] = card[0];
                            AddHandToHand(&card[2], &holeHand[firstUnknown]);
                            holeHand[firstUnknown+1] = card[1];
                            AddHandToHand(&card[3], &holeHand[firstUnknown+1]);
                            EnumBoards(2);
                            holeHand[firstUnknown] = card[0];
                            AddHandToHand(&card[3], &holeHand[firstUnknown]);
                            holeHand[firstUnknown+1] = card[1];
                            AddHandToHand(&card[2], &holeHand[firstUnknown+1]);
                            EnumBoards(2);
                            deckP[3]->hand[kRanks] = card[3].hand[kRanks]; }
                        deckP[2]->hand[kRanks] = card[2].hand[kRanks]; }
                    deckP[1]->hand[kRanks] = card[1].hand[kRanks]; }
                deckP[0]->hand[kRanks] = card[0].hand[kRanks]; }
        }
}

void PrintCard(FILE *f, HandStrucT *h)
{
    int     c, s = '?';
    Mask_T  m, r = h->hand[kRanks];

    for (m = 2, c = 2; c <= 14; m <<= 1, ++c)
        if (r & m)
            break;

    switch (c) {
        case kA: c = 'A'; break;
        case kK: c = 'K'; break;
        case kQ: c = 'Q'; break;
        case kJ: c = 'J'; break;
        case 10: c = 'T'; break;
        default: c += '0'; }
    if (h->hand[kClubs])
        s = 'c';
    if (h->hand[kDiamonds])
        s = 'd';
    if (h->hand[kHearts])
        s = 'h';
    if (h->hand[kSpades])
        s = 's';
    fprintf(f, "%c%c", c, s);
}

void General_Help(void)
{
	printf("This program displays a series of prompts for information.  After each prompt\n");
	printf("it waits for a response from you.  End each of your responses with the Return\n");
	printf("or Enter key.\n");
	printf("\n");
	printf("Some prompts indicate valid responses in (parentheses).  And some indicate the\n");
	printf("most typical situation in [square brackets]; to specify that condition, just\n");
	printf("press Return or Enter by itself.\n");
	printf("\n");
	printf("If you need to continue your response to a single prompt on another line (such\n");
	printf("as when you need to enter many hole cards) end the line to be continued with\n");
	printf("\"--\" (two hyphens).\n"); 
	printf("\n");
	printf("For help with a specific response, type \"?\" and then press Return or Enter.\n");
	printf("To quit the program, close its window.\n");
}

int GetNextCard(char *string, int *offset, HandStrucT *h)
/*  Increments *offset to skip over any initial spaces, tabs, or commas;
    returns 1 for valid card, 0 if string empty, -1 for bad input;
    if valid card, updates *offset to be just beyond last char of card */
{
    char    c = 1, s, *p;       /* initialization works around CW Pro 2 sscanf bug */
    Mask_T  rank, suit;
    int     i, consumed, digit_one = 0;

    p = string + *offset;
    while (*p == ' ' || *p == '\t' || *p == ',')
        ++p;
    *offset += p - (string + *offset);
    if (*p == '1') {
    	/* 10 rather than T for a ten is legal */
    	digit_one = 1;
    	++p;
    	++*offset; }
    if ((i = sscanf(p, " %c%c%n", &c, &s, &consumed)) != 2) {
        if (!digit_one && (i <= 0 || (i == 1 && !c)))   /* rhs of || works around CW Pro 2 sscanf bug */
            return 0;
        printf("Please specify each card as a rank (2..9/T/J/Q/K/A) and a suit (c/d/h/s).\n");
        return -1; }
    *offset += consumed;
    if (digit_one) {
    	if (c == '0')
    		rank = kT;
    	else {
    		printf("(Found \"1\" not followed by \"0\") Enter a ten as \"T\" or \"10\".\n");
    		return -1;
    		} }
    else
	    if (c >= '2' && c <= '9')
	        rank = c - '0';
	    else
	        switch (toupper(c)) {
	            case 'T':   rank = kT; break;
	            case 'J':   rank = kJ; break;
	            case 'Q':   rank = kQ; break;
	            case 'K':   rank = kK; break;
	            case 'A':   rank = kA; break;
	            default:    printf("Bad card rank \"%c\" (expecting digit 2..9 or one of T/J/Q/K/A).\n",
	                                (int)c);
	                        return -1; }
    switch (tolower(s)) {
        case 'c':   suit = kClubs;      break;
        case 'd':   suit = kDiamonds;   break;
        case 'h':   suit = kHearts;     break;
        case 's':   suit = kSpades;     break;
        default:    printf("Bad suit \"%c\" (expecting one of c/d/h/s).\n", (int)s); return -1; }
    *h = zeroHand;
    h->hand[kRanks] = h->hand[suit] = (1 << (rank - 1));
    if (!deck[(suit-kClubs)*13 + rank - 2].hand[kRanks]) {
        printf("There's only one ");
        PrintCard(stdout, h);
        printf(" in the deck!\n");
        return -1; }
    deck[(suit-kClubs)*13 + rank - 2].hand[kRanks] = 0;
    return 1;
}

void ReadLine(char *s, int max)
{
    int     count;

    for (count = 0; count < max; ++count)
        if ((*s++ = getchar()) == '\n') {
            *--s = 0;
            if (strlen(s) < 2)
            	return;
            if (*(s-1) == '-' && *(s-2) == '-') {
            	s -= 2;		/* line ended with "--" indicating line continuation */
            	count -= 2; }
            else
            	return; }
    *s = 0;
}

Boolean GetResponse(char *prompt, char *s, int max, char *help_text)
{
	do {
		printf(prompt);
		ReadLine(s, max);
		if (*s == '?')
			printf(help_text);
	} while (*s == '?');
	if (tolower(*s) == 'h') {
		General_Help();
		return false; }
	return true;
}

Boolean GetHoleCards(int *nPlayers)
{
    char        s[81];
    int         offset, card = 0, i;

    if (!GetResponse("\nKnown hole cards; two per player: ", s, 80,
    "For each card enter the rank -- 2,3,4,5,6,7,8,9,(10 or T),J,Q,K,A --\n"
    "and suit -- c,d,h,s.  Enter two cards for each player.  Letters\n"
    "may be either upper- or lower-case.\n"))
    	return false;
    offset = 0;
    while ((i = GetNextCard(s, &offset, &holeCard[card])) > 0)
        if (++card == kMaxPlayers*2)
            break;
    if (i < 0)
        return false;
    if (card & 1 || card < 2) {
        printf("Number of hole cards must be at least two, and even.\n");
        return false; }
    *nPlayers = card / 2;
    for (i = 0; i < *nPlayers; ++i) {
        holeHand[i] = holeCard[2*i];
        AddHandToHand(&holeCard[2*i + 1], &holeHand[i]); }
    return true;
}

Boolean GetBoard(int *nBoardCards, HandStrucT *board)
{
    char        s[81];
    int         offset, i;

    *board = zeroHand;
    *nBoardCards = 0;
    if (!GetResponse("Known board cards [none]: ", s, 80,
    "If no board cards are known (there's no flop yet) just press Return or Enter.\n"
    "If only the flop has been dealt, enter three cards; if the turn card has been\n"
    "dealt, enter four cards.\n"
    "For each card enter the rank (2,3,4,5,6,7,8,9,10 or T,J,Q,K,A) and suit (c,d,h,s).\n"))
    	return false;
    offset = 0;
    while ((i = GetNextCard(s, &offset, &boardCard[*nBoardCards])) > 0) {
        AddHandToHand(&boardCard[*nBoardCards], board);
        if (++*nBoardCards > 4)
            break; }
    if (i < 0)
        return false;
    if (*nBoardCards > 4) {
        printf("Number of board cards cannot exceed 4 (flop and turn).\n");
        return false; }
    return true;
}

Boolean GetDeadCards(int *nDeadCards)
{
    char    s[81];
    int     offset, i, max = nCardsInDeck - (5 - nBoardCards);

    if (!GetResponse("Dead/exposed cards [none]: ", s, 80,
    "Enter any known cards which are unavailable to be dealt to the board, such as\n"
    "those which have been exposed, perhaps accidentally.  If there are no such cards\n"
    "then just press Return or Enter."))
    	return false;
    *nDeadCards = 0;
    offset = 0;
    while ((i = GetNextCard(s, &offset, &deadCard[*nDeadCards])) > 0)
        if (++*nDeadCards == max + 1)
            break;
    if (i < 0)
        return false;
    if (*nDeadCards > max) {
        printf("Not enough cards left for the board!\n");
        return false; }
    return true;
}

Boolean GetNbrUnknown(int *nUnknown)
{
    char    s[81], prompt[81];
    int     max, i;

    *nUnknown = 0;
    max = kMaxPlayers - nPlayers;
    if (!max)
        return true;
    if (max > 2)
        max = 2;
    sprintf(prompt, "Number of players with unknown hole cards (0 to %d) [0]: ", max);
    do {
        if (!GetResponse(prompt, s, 80,
        "Enter the number of players holding unknown (\"random\") cards.  Only\n"
        "a small number of such players can be accommodated by this program\n"
        "because otherwise the number of possible outcomes would be too large.\n"
        "If there are no such players you can just press Return or Enter.\n"))
        	return false;
        *nUnknown = 0;
        if (!s[0])
            return true;
        i = sscanf(s, "%d", nUnknown);
    } while (i == -1 || *nUnknown < 0 || *nUnknown > max);
    return true;
}

char *WithCommas(double n, char *s)
/* returns s; caller responsible for s being large enough */
{
    char    *in, *out;
    int     commas, len, leadDigits, i;

    sprintf(s, "%.0f", n);
    commas = (len = strlen(s)) / 3;
    if (!(leadDigits = len % 3)) {
        --commas;
        leadDigits = 3; }
    if (commas < 1)
        return s;
    in = s + len;
    out = in + commas;
    *out-- = *in--; /* copy the terminating null */
    for (i = 0; i < commas; ++i) {
        *out-- = *in--;
        *out-- = *in--;
        *out-- = *in--;
        *out-- = ','; }
    for (i = 0; i < leadDigits; ++i)
        *out-- = *in--;
    return s;
}

Boolean UserConfirm(double nDeals)
{
    char    s[81], prompt[81], c;

	sprintf(prompt, "%s deals required.  Start dealing? (y/n) [y]: ", WithCommas(nDeals, s));
    while (true) {
        if (!GetResponse(prompt, s, 80,
        "The program is ready to tabulate each possible outcome (\"deal\").  Nothing\n"
        "will happen while it is working.  When it is finished, you will see the results.\n"
        "The time required depends on your computer's speed and how many deals are\n"
        "required.  As a very rough rule of thumb, figure 1 million deals per second per\n"
        "400 MHz of computer processor speed.\n"
        "To start the calculations, just press Return or Enter.  To start over, press\n"
        "\"n\" and then press Return or Enter.\n"))
        	return false;
        if (s[0]) {
            if ((sscanf(s, " %c", &c)) != 1)
                continue;
            if (tolower(c) == 'y')
                return true;
            if (tolower(c) == 'n')
                return false; }
        else
            return true; }
}

void Initialize(void)
{
    Boolean     resultsCreated;
#ifdef __Mac__
    HFileParam  PB;
#endif

    if (!(resultsCreated = ((out = fopen(kOutFileName, "r")) == 0)))
        fclose(out);
    if (!(out = fopen(kOutFileName, "a")))
        exit(errno);
    if (resultsCreated) {
        fprintf(out, "View this file with a fixed-width font such as Courier");
#ifdef __Mac__
		fprintf(out, " or Monaco");
#endif
		fprintf(out, "\n"); }
#ifdef __Mac__
    PB.ioNamePtr = "\p"kOutFileName;
    PB.ioVRefNum = -LMGetSFSaveDisk();
    PB.ioDirID = LMGetCurDirStore();
    PB.ioFDirIndex = 0;
    if (PBHGetFInfoSync((HParmBlkPtr)&PB) == noErr) {
            PB.ioFlFndrInfo.fdCreator = kFileCreator;
            PB.ioDirID = LMGetCurDirStore();
            PBHSetFInfoSync((HParmBlkPtr)&PB); }
#endif
#ifdef THINK_C
    console_options.title = "\pHold¼Em Showdown";
#endif
#ifdef __MWERKS__
	SIOUXSettings.asktosaveonclose = false;
    SIOUXSettings.rows = 25;
#ifdef __Mac__
    printf("\n");
    SIOUXSetTitle("\pHold'Em Showdown");
#endif
#endif
    if (!Init_Hand_Eval()) {
        printf("RAM allocation failure.\n");
        exit(0); }
#ifdef __Mac__
    printf("        Hold'Em Showdown version ");
    printf(kVersion);
    printf(" written by Steve Brecher\n");
    printf("Deals all possible boards to get exact win probability for each hand specified.\n");
    printf("Results written/appended to ""kOutFileName"" in Hold'Em Showdown's folder.\n");
#else
    printf("        HoldEm Showdown version ");
    printf(kVersion);
    printf(" written by Steve Brecher\n");
    printf("Deals all possible boards to get exact win probability for each hand specified.\n");
    printf("Results written/appended to \""kOutFileName"\".\n");
#endif
    printf("\nFor general help, type \"help\" or just \"h\" followed by Return or Enter.\n");
    printf("For help with a specific response, type \"?\" followed by Return or Enter.\n");
    output[0] = out;
    output[1] = stdout;
}

#define kFormat "%13.6f"

void ResultsOut(void)
{
    int     i, j, n, f, nbrToPrint;
    char    s[81];

    if (nPlayers == 2 && !nUnknown) { /* flesh out results for special case; see Enum2GuysNoFlop */
        wins[1] = (long)nPots - wins[0] - splits[0];
        splits[1] = splits[0];
        partialPots[0] = partialPots[1] = splits[0]/2.0; }

    nbrToPrint = nPlayers;
    if (nUnknown == 2) {    /* show the total of the two as one entry */
        --nbrToPrint;
        wins[nPlayers-2] += wins[nPlayers-1];
        splits[nPlayers-2] += splits[nPlayers-1];
        partialPots[nPlayers-2] += partialPots[nPlayers-1]; }

    fprintf(out, "\n%s pots with board cards:", WithCommas(nPots, s));
    if (!nBoardCards)
        fprintf(out, " (unspecified)");
    for (i = 0; i < nBoardCards; ++i) {
        fprintf(out, " ");
        PrintCard(out, &boardCard[i]); }
    if (nDeadCards) {
        fprintf(out, "\nDead/exposed cards:");
        for (i = 0; i < nDeadCards; ++i) {
            fprintf(out, " ");
            PrintCard(out, &deadCard[i]); } }
    fprintf(out, "\n");

    for (f = 0; f < 2; ++f) {
        n = nbrToPrint;
        j = 0;
        while (n > 0) {
            fprintf(output[f], "\n                     ");
            for (i = j; i < j+k_hands_per_line && i < nbrToPrint; ++i)
                if (i >= nPlayers - nUnknown)
                    if (nUnknown == 1)
                        fprintf(output[f], "  Unknown");
                    else
                        fprintf(output[f], " Unknowns");
                else {
                    fprintf(output[f], "         ");
                    PrintCard(output[f], &holeCard[i*2]);
                    PrintCard(output[f], &holeCard[i*2+1]); }
            fprintf(output[f], "\n%% chance of outright win ");
            for (i = j; i < j+k_hands_per_line && i < nbrToPrint; ++i)
                fprintf(output[f], kFormat, wins[i]*100.0/nPots);
            fprintf(output[f], "\n%% chance of win or split ");   
            for (i = j; i < j+k_hands_per_line && i < nbrToPrint; ++i)
                fprintf(output[f], kFormat, (wins[i]+splits[i])*100.0/nPots);
            fprintf(output[f], "\nexpected return, %% of pot");
            for (i = j; i < j+k_hands_per_line && i < nbrToPrint; ++i)
                fprintf(output[f], kFormat, (wins[i]+partialPots[i])*100.0/nPots);
            fprintf(output[f], "\nfair pot odds:1          ");
            for (i = j; i < j+k_hands_per_line && i < nbrToPrint; ++i) {
                if (wins[i] || partialPots[i] > 0.1E-9)
                    fprintf(output[f], kFormat,
                        (nPots-(wins[i]+partialPots[i]))/(wins[i]+partialPots[i]));
                else
                    fprintf(output[f], " infinite"); }
            fprintf(output[f], "\npots won:                ");
            for (i = j; i < j+k_hands_per_line && i < nbrToPrint; ++i) {
                fprintf(output[f], "%13.2f", wins[i] + partialPots[i]);
                --n; }
            fprintf(output[f], "\n");
            j += k_hands_per_line; }
        fflush(output[f]); }
}

double Factorial(double n)
{
    if (n > 1.0)
        return n * Factorial(n - 1.0);
    return 1.0;
}

double Combos(int chooseFrom, int choose)
{
    return Factorial((double)chooseFrom) /
            (Factorial((double)choose) * Factorial((double)(chooseFrom - choose)));
}

double NbrEnumerations(void)
{
    double  enums;

    enums = Combos(nCardsInDeck - 2*nUnknown, 5 - nBoardCards); /* number of boards */
    if (nUnknown)
        /*    *= the number of ORDERED sets of hole cards for the "unknown" players */
        enums *= Combos(nCardsInDeck, 2*nUnknown)
                    * Factorial(2*nUnknown)/((1 << nUnknown)/*2^^nUnknown*/);
    return enums;
}

void main(void)
{
    Boolean restart;
    char    s[81];
#if kTimer
    clock_t timer;
#endif

    Initialize();

    while (true) {
        InitDeck();
        restart = true;
        if (GetHoleCards(&nPlayers)) {
            if (GetNbrUnknown(&nUnknown)) {
            	nPlayers += nUnknown;
	            if (nPlayers < 2)
	                printf("At least two players are required.\n");
	            else
	                if (GetBoard(&nBoardCards, &board[0])) {
	                    nCardsInDeck = 52 - (nPlayers - nUnknown)*2 - nBoardCards;
	                    if (GetDeadCards(&nDeadCards)) {
	                        nCardsInDeck -= nDeadCards;
	                        nPots = NbrEnumerations();
	                        if (nPots > LONG_MAX)
	                            printf("%s pots required, which is higher than this program can count.\n",
	                                    WithCommas(nPots, s));
	                        else
	                            /* Due to an optimization, deals == pots/2 when there are two
	                               players with unknown hole cards... */
	                            if (UserConfirm((nUnknown == 2) ? nPots/2.0 : nPots))
	                                restart = false; } } } }
        if (restart) {
            printf("\nRestarting...\n");
            continue; }
        IndicateWait();
#if kTimer
        timer = clock();
#endif
        Enumerate();
#if kTimer
        timer = clock() - timer;
        printf("seconds = %.1f\n", (float)timer/CLOCKS_PER_SEC);
#endif
        ResultsOut(); }
}
