/*
 * Copyright (C) 2005, 2006, 2007 Thomas Cort <code@member.fsf.org>
 *
 * This file is part of pokerd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "log.h"
#include "poker.h"
#include "eval.h"
#include "deck.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <poker-eval/poker_defs.h>
#include <poker-eval/inlines/eval.h>

extern int snprintf(char *str, size_t size, const char *format, ...);


/* this code is based on an example program that came with the poker-eval package */


int gNCommon, gNDead, gNPlayers;
CardMask gDeadCards, gCommonCards, gPlayerCards[MAX_PLAYERS];

static void eval_init() {
	int i, j = 0, cardCount = 0, c;

	/* load the players hole cards into the evaluator */
	for (i = 0; i < game_info.num_players; ++i) {
		char *card[2];

		if (game_info.players[i].folded == FALSE) {

			card[0] = card2str(game_info.players[i].hole[0]);

			if (StdDeck_stringToCard(card[0], &c) == 0) {
				char buf[128];
				bzero(buf,128);
				snprintf(buf,127,"[EVAL] %s",strerror(errno));
				logit(buf);
				exit(1);
			}

			StdDeck_CardMask_SET(gPlayerCards[j], c);
			gNPlayers = 0+1;
			++cardCount;

			card[1] = card2str(game_info.players[i].hole[1]);

			if (StdDeck_stringToCard(card[1], &c) == 0) {
				char buf[128];
				bzero(buf,128);
				snprintf(buf,127,"[EVAL] %s",strerror(errno));
				logit(buf);
				exit(1);
			}

			StdDeck_CardMask_SET(gPlayerCards[j], c);
			gNPlayers = 1+1;
			++cardCount;

			free(card[0]);
			free(card[1]);

			j++;
		}
	}

	for (i = 0; i < 5; i++) {
		char *x;
		x = card2str(game_info.board[i]);

		if (StdDeck_stringToCard(x, &c) == 0) {
			char buf[128];
			bzero(buf,128);
			snprintf(buf,127,"[EVAL] %s",strerror(errno));
			logit(buf);
			exit(1);
		}

		StdDeck_CardMask_SET(gCommonCards, c);
		++gNCommon;
		free(x);
	}
}

int evalmain() {
	CardMask deadCards, cards, playerCards[MAX_PLAYERS], pCards;
	HandVal handval[MAX_PLAYERS], maxHand;
	int i, j;
	unsigned long winCount[MAX_PLAYERS], loseCount[MAX_PLAYERS], tieCount[MAX_PLAYERS], handCount=0, nWinners;
	float ev[MAX_PLAYERS];

	CardMask_RESET(gDeadCards);
	CardMask_RESET(gCommonCards);

	gNCommon = 0, gNDead = 0, gNPlayers = 0;

	for (i=0; i<MAX_PLAYERS; i++) {
		CardMask_RESET(gPlayerCards[i]);
		winCount[i] = 0;
		tieCount[i] = 0;
		loseCount[i] = 0;
		ev[i] = 0;
	};

	eval_init();

	deadCards = gDeadCards;

	for (i=0; i<gNPlayers; i++) {
		CardMask_OR(playerCards[i], gPlayerCards[i], gCommonCards);
		CardMask_OR(deadCards, deadCards, playerCards[i]);
	};

  ENUMERATE_N_CARDS_D(cards, 5-gNCommon, deadCards,
                      {
                        ++handCount;
                        nWinners = 0;
                        maxHand = HandVal_NOTHING;
                        for (i=0; i<gNPlayers; i++) {
                          CardMask_OR(pCards, playerCards[i], cards);
                          handval[i] = Hand_EVAL_N(pCards, 7);
                          if (handval[i] > maxHand) {
                            nWinners = 1;
                            maxHand = handval[i];
                          }
                          else if (handval[i] == maxHand)
                            ++nWinners;
                        };

                        for (i=0; i<gNPlayers; i++) {
                          if (handval[i] == maxHand) {
                            if (nWinners == 1) {
                              ++winCount[i];
                              ev[i] += 1.0;
                            }
                            else {
                              ++tieCount[i];
                              ev[i] += (1.0 / nWinners);
                            };
                          }
                          else
                            ++loseCount[i];
                        }
                      }
                      );


	j = 0;

	/* get the results */
	for (i=0; i<gNPlayers; i++) {
		while (j < MAX_PLAYERS && game_info.players[j].folded != FALSE) 
			j++;

		if (winCount[i]) {
			game_info.players[j].outcome = WIN;
			game_info.winners++;
		} else if (tieCount[i]) {
			game_info.players[j].outcome = TIE;
			game_info.winners++;
		} else if (loseCount[i]) {
			game_info.players[j].outcome = LOSE;
 		}

		j++;
	}
	return 0;
}
