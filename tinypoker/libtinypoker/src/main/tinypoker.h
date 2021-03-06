/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <linuxgeek@gmail.com>
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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TINYPOKER_H
#define __TINYPOKER_H

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <io.h>
	typedef int socklen_t;

#else

#include <netinet/in.h>
	typedef int SOCKET;

#endif

#ifndef max
#define max(x,y) (x > y ? x : y)
#endif

#ifndef min
#define min(x,y) (x < y ? x : y)
#endif

/**
 * Major version number.
 */
#define LIBTINYPOKER_MAJOR_VERSION 0

/**
 * Minor version number.
 */
#define LIBTINYPOKER_MINOR_VERSION 2

/**
 * Patch version number.
 */
#define LIBTINYPOKER_PATCH_VERSION 0

#define TINYPOKER_PORT ((short) 9898)

/**
 * Version number as a string.
 */
#define LIBTINYPOKER_VERSION "0.2.0"

/* TODO Support for Stud and Draw */
#define HOLDEM_HOLE_CARDS 2
#define HOLDEM_PLAYERS_PER_TABLE 8
#define HOLDEM_BOARD_CARDS 5

/**
 * Stage of the Hold'em Hand.
 */
	enum holdem_stage {
		PREFLOP,
		FLOP,
		TURN,
		RIVER,
		SHOWDOWN
	};

/**
 * Enumerate the possible card suits.
 */
	enum card_suit {
		CLUBS = 'C',
		DIAMONDS = 'D',
		HEARTS = 'H',
		SPADES = 'S'
	};

	enum game_type {
		UNSPECIFIED = 0,
		HOLDEM = 1,
		DRAW = 2,
		STUD = 3
	};

/**
 * Enumerate the possible card ranks.
 */
	enum card_rank {
		TWO = '2',
		THREE = '3',
		FOUR = '4',
		FIVE = '5',
		SIX = '6',
		SEVEN = '7',
		EIGHT = '8',
		NINE = '9',
		TEN = 'T',
		JACK = 'J',
		QUEEN = 'Q',
		KING = 'K',
		ACE = 'A'
	};

/**
 * Structure used to hold network communications information.
 */
	typedef struct ipp_socket {
		SOCKET sd;
		struct sockaddr_in addr;
	} ipp_socket;

/**
 * Structure used to hold protocol messages.
 */
	typedef struct ipp_message {
		int type;
		char *payload;
		char **parsed;
	} ipp_message;

/**
 * Structure used to hold a playing card.
 */
	typedef struct ipp_card {
		enum card_rank rank;
		enum card_suit suit;
	} ipp_card;

/**
 * The size of the card deck (number of cards).
 */
#define DECKSIZE (52)

/**
 * Structure used to hold a deck of 52 cards.
 */
	typedef struct ipp_deck {
		guint8 deck_index;
		ipp_card *cards[DECKSIZE];
	} ipp_deck;

/**
 * Structure used to hold a poker player.
 * Some fields aren't used in some contexts.
 *   the server may know all players' hole cards.
 *   the client may only know his/her hole cards.
 */
	typedef struct ipp_player {
		char *name;	/* player name given at handshake */
		ipp_socket *sock;	/* communications socket */

		/* TODO Support for Stud and Draw */
		ipp_card *hole[HOLDEM_HOLE_CARDS];	/* two hole cards */
		guint32 bankroll;	/* amount of money this player has */
		guint32 amt_in;	/* amount into the pot from this player */
		gboolean still_in;	/* TRUE if still in the hand, FALSE
					 * if folded, disconnected, etc. */
		gboolean kill;	/* TRUE if the player is scheduled for removal from the table. This is needed because ipp_remove_player() cannot be called while the table is locked. */
	} ipp_player;

/**
 * Structure used to hold a poker table.
 */
	typedef struct ipp_table {
		enum game_type type;
		guint8 nplayers;
		guint32 amt_to_call;
		enum holdem_stage stage;
		/* TODO Support for Stud and Draw */
		ipp_player *players[HOLDEM_PLAYERS_PER_TABLE];
		ipp_card *board[HOLDEM_BOARD_CARDS];
		ipp_deck *deck;
		GMutex *lock;
	} ipp_table;

/**
 * Allocates an empty ipp_socket. Don't forget to call ipp_free_socket().
 * @return a malloc()'d ipp_socket structure.
 * @see ipp_free_socket()
 */
	ipp_socket *ipp_new_socket(void);

/**
 * Deallocate an ipp_socket.
 */
	void ipp_free_socket(ipp_socket * sock);

/**
 * Allocates an empty ipp_message. Don't forget to ipp_free_message().
 * @return a malloc()'d ipp_message structure.
 */
	ipp_message *ipp_new_message(void);

/**
 * Parses msg->payload into msg->parsed
 * @param msg an IPP message
 */
	void ipp_parse_msg(ipp_message * msg);

/**
 * Deallocate an ipp_message.
 */
	void ipp_free_message(ipp_message * msg);

/**
 * Allocates an empty ipp_card. Don't forget to ipp_free_card().
 * @return a malloc()'d ipp_card structure.
 */
	ipp_card *ipp_new_card(void);

/**
 * Deallocate an ipp_card.
 */
	void ipp_free_card(ipp_card * card);

/**
 * Gives a malloc'd, null terminated string representation of a card c (ex: "9d\0")
 * @param c the card to represent.
 * @return a NULL terminated string.
 */
	char *ipp_card2str(ipp_card * c);

/**
 * A deck iterator. Always returns the next card in the deck.
 * When the end of the deck is reached, it starts back at the
 * beginning.
 * @param deck the deck to operate on.
 * @return the card at (deck_index + 1) % DECKSIZE.
 */
	ipp_card *ipp_deck_next_card(ipp_deck * deck);

/**
 * Allocates an ipp_deck and cards. Sets deck index to 51.
 * The first call to ipp_deck_next_card() will return card at index 0.
 * Don't for get to ipp_free_deck().
 * @return a malloc()'d ipp_deck structure.
 */
	ipp_deck *ipp_new_deck(void);

/**
 * Randomize the order of the cards in the deck.
 * @param deck to shuffle.
 */
	void ipp_shuffle_deck(ipp_deck * deck);

/**
 * Deallocates an ipp_deck.
 */
	void ipp_free_deck(ipp_deck * deck);

/**
 * Allocates an empty ipp_player. Don't forget to ipp_free_player().
 * @return a malloc()'d ipp_player structure.
 */
	ipp_player *ipp_new_player(void);

/**
 * Deallocates an ipp_player.
 */
	void ipp_free_player(ipp_player * player);

/**
 * Allocates an empty ipp_table. Don't for get to ipp_free_table().
 * @return a malloc()'d ipp_table structure.
 */
	ipp_table *ipp_new_table(void);

/**
 * Add an ipp_player to an ipp_table in a thread safe way.
 * @param table the table to add to.
 * @param player the player to add.
 * @return the index in table->players[] or -1 if the table was full.
 */
	gint8 ipp_add_player(ipp_table * table, ipp_player * player);

/**
 * Deallocates an ipp_table.
 */
	void ipp_free_table(ipp_table * table);

/**
 * The name of this library for IPP strings, etc.
 */
#define TINYPOKER "TinyPoker"

/**
 * The service name used to determine the TCP port number.
 * This should be "tinypoker tcp/9899" in /etc/services
 */
#define IPP_SERVICE_NAME "tinypoker"

/**
 * The maximum number of server sockets.
 */
#define IPP_SERVER_MAX_SDS (8)

/**
 * The version of the protocol implemented.
 * Matches "2.0" and "2.0.0". The spec says 2.0, but
 * the reference implementation uses 2.0.0 :|
 */
#define REGEX_PROTOCOL_VERSION "(2\\.0(\\.0){0,1}){1}"

/**
 * The maximum size of a message, in characters.
 * No valid message should ever exceed this.
 */
#define MAX_MSG_SIZE (127)

/**
 * The minimum size of a message, in characters.
 * No valid message should ever be smaller than this this.
 */
#define MIN_MSG_SIZE (4)

/**
 * The number of seconds a client should wait for network input.
 * This should be much higher than SERVER_READ_TIMEOUT.
 * @see SERVER_READ_TIMEOUT
 */
#define CLIENT_READ_TIMEOUT ((guint8) 600)

/**
 * The number of seconds a server should wait for network input.
 * This should be much smaller than CLIENT_READ_TIMEOUT.
 * @see CLIENT_READ_TIMEOUT
 */
#define SERVER_READ_TIMEOUT ((guint8) 10)

/**
 * The number of seconds a client should wait for network output.
 * This should be much higher than SERVER_WRITE_TIMEOUT.
 * @see SERVER_WRITE_TIMEOUT
 */
#define CLIENT_WRITE_TIMEOUT ((guint8) 600)

/**
 * The number of seconds a server should wait for network output.
 * This should be much smaller than CLIENT_WRITE_TIMEOUT.
 * @see CLIENT_WRITE_TIMEOUT
 */
#define SERVER_WRITE_TIMEOUT ((guint8) 10)

/* Regular expressions used to match message parts */

/**
 * Whitespace characters used to separate message parts.
 */
#define REGEX_SPACE "[\\t ]+"

/**
 * Ace, 2, 3, 4, 5, 6, 7, 8, 9, Ten, Jack, Queen, King
 */
#define REGEX_RANK "[A23456789TJQK]{1}"

/**
 * Spades, Hearts, Diamonds, Clubs
 */
#define REGEX_SUIT "[SHDC]{1}"

/**
 * Concatenation of rank and suit
 */
#define REGEX_CARD REGEX_RANK REGEX_SUIT

/**
 * Hole Cards given for Texas Holdem
 */
#define REGEX_HOLDEM_HOLE_CARDS REGEX_CARD REGEX_SPACE REGEX_CARD

/**
 * Hole Cards given for 7 Card Stud
 */
#define REGEX_STUD_HOLE_CARDS REGEX_CARD

/**
 * Hole Cards given for 5 Card Draw
 */
#define REGEX_DRAW_HOLE_CARDS REGEX_CARD REGEX_SPACE REGEX_CARD

/**
 * Hole Cards for any game type.
 */
#define REGEX_HOLE_CARDS "(" REGEX_DRAW_HOLE_CARDS "|" REGEX_STUD_HOLE_CARDS "|" REGEX_HOLDEM_HOLE_CARDS ")"

/**
 * Name of player (no spaces).
 */
#define REGEX_NAME "[0-9a-zA-Z_]{1,32}"

/**
 * Password of player (no spaces).
 */
#define REGEX_PASS "[0-9a-zA-Z_]{1,32}"

/**
 * Password of player (no spaces).
 */
#define REGEX_LOGIN "(" REGEX_NAME "|" REGEX_NAME ":" REGEX_PASS ")"

/**
 * Amount of Money or other Non-Negative Amount (i.e. raises per round)
 * Smaller than max value of a signed 32-bit int.
 * XXX: 16-bit CPUs/OSes will fail on large amounts.
 */
#define REGEX_AMT "[0-9]{1,9}"

/**
 * Informational messages.
 */
#define REGEX_INFO "[0-9a-zA-Z/.\\t \\-]+"

/**
 * Small Blind.
 */
#define REGEX_SMALL_BLIND REGEX_AMT

/**
 * Big Blind.
 */
#define REGEX_LARGE_BLIND REGEX_AMT

/**
 * Raises per round
 */
#define REGEX_RAISES_PER_ROUND REGEX_AMT

/**
 * String used to specify the blinds for a game.
 */
#define REGEX_BLINDS REGEX_SMALL_BLIND REGEX_SPACE REGEX_LARGE_BLIND

/**
 * Type of stud game to play (5 card or 7 card)
 */
#define REGEX_STUD_TYPE_SPECIFIER "[57]{1}"

/**
 * String used to identify a stud game.
 */
#define REGEX_STUD_TYPE "STUD"

/**
 * String used to identify a draw game.
 */
#define REGEX_DRAW_TYPE "DRAW"

/**
 * String used to identify a holdem game.
 */
#define REGEX_HOLDEM_TYPE "HOLDEM"

/**
 * Game Types HOLDEM, DRAW, STUD.
 * STUD is followed by 5 or 7.
 */
#define REGEX_GAME_TYPE "(" REGEX_STUD_TYPE REGEX_SPACE REGEX_STUD_TYPE_SPECIFIER "|" REGEX_DRAW_TYPE "|" REGEX_HOLDEM_TYPE ")"

#define REGEX_DRAW_1 "1" REGEX_SPACE REGEX_CARD
#define REGEX_DRAW_2 "2" REGEX_SPACE REGEX_CARD REGEX_SPACE REGEX_CARD
#define REGEX_DRAW_3 "3" REGEX_SPACE REGEX_CARD REGEX_SPACE REGEX_CARD REGEX_SPACE REGEX_CARD
#define REGEX_DRAW_4 "4" REGEX_SPACE REGEX_CARD REGEX_SPACE REGEX_CARD REGEX_SPACE REGEX_CARD REGEX_SPACE REGEX_CARD
#define REGEX_DRAW_5 "5" REGEX_SPACE REGEX_CARD REGEX_SPACE REGEX_CARD REGEX_SPACE REGEX_CARD REGEX_SPACE REGEX_CARD REGEX_SPACE REGEX_CARD

/**
 * Draw for cards for 5 Card Draw
 */
#define REGEX_DRAW_N "(" REGEX_DRAW_1 "|" REGEX_DRAW_2 "|" REGEX_DRAW_3 "|" REGEX_DRAW_4 "|" REGEX_DRAW_5 ")"

#define CMD_STRAIGHTFLUSH "STRAIGHTFLUSH"
#define REGEX_HT_STRAIGHTFLUSH CMD_STRAIGHTFLUSH REGEX_SPACE REGEX_RANK
#define REGEX_MSG_STRAIGHTFLUSH ("^" REGEX_HT_STRAIGHTFLUSH "$")

#define CMD_FOUROFAKIND "FOUROFAKIND"
#define REGEX_HT_FOUROFAKIND CMD_FOUROFAKIND REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK
#define REGEX_MSG_FOUROFAKIND ("^" REGEX_HT_FOUROFAKIND "$")

#define CMD_FULLHOUSE "FULLHOUSE"
#define REGEX_HT_FULLHOUSE CMD_FULLHOUSE REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK
#define REGEX_MSG_FULLHOUSE ("^" REGEX_HT_FULLHOUSE "$")

#define CMD_FLUSH "FLUSH"
#define REGEX_HT_FLUSH CMD_FLUSH REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK
#define REGEX_MSG_FLUSH ("^" REGEX_HT_FLUSH "$")

#define CMD_STRAIGHT "STRAIGHT"
#define REGEX_HT_STRAIGHT CMD_STRAIGHT REGEX_SPACE REGEX_RANK
#define REGEX_MSG_STRAIGHT ("^" REGEX_HT_STRAIGHT "$")

#define CMD_THREEOFAKIND "THREEOFAKIND"
#define REGEX_HT_THREEOFAKIND CMD_THREEOFAKIND REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK
#define REGEX_MSG_THREEOFAKIND ("^" REGEX_HT_THREEOFAKIND "$")

#define CMD_TWOPAIR "TWOPAIR"
#define REGEX_HT_TWOPAIR CMD_TWOPAIR REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK
#define REGEX_MSG_TWOPAIR ("^" REGEX_HT_TWOPAIR "$")

#define CMD_ONEPAIR "ONEPAIR"
#define REGEX_HT_ONEPAIR CMD_ONEPAIR REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK
#define REGEX_MSG_ONEPAIR ("^" REGEX_HT_ONEPAIR "$")

#define CMD_HIGHCARD "HIGHCARD"
#define REGEX_HT_HIGHCARD CMD_HIGHCARD REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK
#define REGEX_MSG_HIGHCARD ("^" REGEX_HT_HIGHCARD "$")

/**
 * Declaration of a hand that can be made from a player's cards
 */
#define REGEX_HANDTYPE "(" REGEX_HT_STRAIGHTFLUSH "|" REGEX_HT_FOUROFAKIND "|" REGEX_HT_FULLHOUSE "|" REGEX_HT_FLUSH "|" REGEX_HT_STRAIGHT "|" REGEX_HT_THREEOFAKIND "|" REGEX_HT_TWOPAIR "|" REGEX_HT_ONEPAIR "|" REGEX_HT_HIGHCARD ")"

/**
 * The word blind.
 */
#define REGEX_BLIND "BLIND"

/**
 * The word straddle.
 */
#define REGEX_STRADDLE "STRADDLE"

/**
 * The word owing.
 */
#define REGEX_OWING "OWING"

/**
 * Betting Message
 */
#define REGEX_ACTION "(" REGEX_BLIND "|" REGEX_STRADDLE "|" REGEX_OWING ")"

/**
 * Matches a question mark.
 */
#define REGEX_QMARK "\\?"

/* protocol commands */

/**
 * Server welcome message sent after a client connects.
 */
#define CMD_IPP "IPP"

/**
 * Client response to IPP. This command asks the server to add the player to the game.
 */
#define CMD_BUYIN "BUYIN"

/**
 * Server welcome message sent after a client buys in.
 */
#define CMD_WELCOME "WELCOME"

/**
 * Sent by the server to start a new game.
 */
#define CMD_NEWGAME "NEWGAME"

/**
 * Sent by the server before play starts to identify a player and their
 * starting stake.
 */
#define CMD_PLAYER "PLAYER"

/**
 * Sent by the server at the start of each hand in a game to identify
 * the player on the button (first to bet).
 */
#define CMD_BUTTON "BUTTON"

/**
 * Sent by the server to remind a player that they have anted the
 * indicated amt.
 */
#define CMD_ANTE "ANTE"

/**
 * Sent by the server to tell a player their hole cards.
 */
#define CMD_DEAL "DEAL"

/**
 * Sent by the server to tell other players that the
 * player named name has sent the given msg.
 */
#define CMD_FROM "FROM"

/**
 * Sent by the server in Texas Holdem to announce the first three common cards.
 */
#define CMD_FLOP "FLOP"

/**
 * Sent by the server in Texas Holdem to announce the fourth common card.
 */
#define CMD_TURN "TURN"

/**
 * Sent by the server in Texas Holdem to announce the fifth and last common card.
 */
#define CMD_RIVER "RIVER"

/**
 * Sent by the server to ask a player how many cards they wish to draw.
 */
#define CMD_DRAWQ "DRAW" REGEX_QMARK

/**
 * Asks to draw num cards, and lists the cards being traded in.
 */
#define CMD_DRAW "DRAW"

/**
 * Sent by the server to indicate the new cards drawn by the player.
 */
#define CMD_DRAWN "DRAWN"

/**
 * Folds the player for this hand.
 */
#define CMD_FOLD "FOLD"

/**
 * Sent by the server to inform a player of a new up (public) card.
 */
#define CMD_UP "UP"

/**
 * Sent by the server to inform a player of a new down (private) card.
 */
#define CMD_DOWN "DOWN"

/**
 * Sent by the server to the player who is required to bet, blind or straddle.
 */
#define CMD_ACTIONQ "ACTION" REGEX_QMARK

/**
 * Makes the blind bet.
 */
#define CMD_BLIND REGEX_BLIND

/**
 * Taps out for the blind bet.
 */
#define CMD_TAPOUT "TAPOUT"

/**
 * Calls the current bet.
 */
#define CMD_CALL "CALL"

/**
 * Raises the current bet.
 */
#define CMD_RAISE "RAISE"

/**
 * Opens the betting by betting amt.
 */
#define CMD_OPEN "OPEN"

/**
 * Makes the straddle bet.
 */
#define CMD_STRADDLE REGEX_STRADDLE

/**
 * Checks the bet (passes without betting).
 */
#define CMD_CHECK "CHECK"

/**
 * Sent by the server to the player being called to show their hand.
 */
#define CMD_SHOWQ "SHOW" REGEX_QMARK

/**
 * Sent by the client in response to SHOW?
 */
#define CMD_SHOW "SHOW"

/**
 * Sent by the server to remaining players to ask if they can beat the given handtype.
 */
#define CMD_BEATQ "BEAT" REGEX_QMARK

/**
 * Indicates that the player cannot beat the announced hand and must fold.
 */
#define CMD_NO "NO"

/**
 * Indicates that the player can beat the announced hand and announces the better hand.
 */
#define CMD_YES "YES"

/**
 * Sent by the server to indicate the winner of the hand,
 * the amount they won, and the winning handtype.
 */
#define CMD_WINNER "WINNER"

/**
 * Sent by the server to indicate a player without enough
 * money left to stay in the game.
 */
#define CMD_BUSTED "BUSTED"

/**
 * Sent by the server to indicate the final winner of the game.
 * Also indicates the end of the game.
 */
#define CMD_GAMEOVER "GAMEOVER"

/**
 * Indicates that a player has unexpectedly quit the game.
 */
#define CMD_QUIT "QUIT"

/**
 * Sent by the server to the client with a messag eexplaining the nature of the error
 * Reasons for errors include trying to perform an action that is not permitted.
 */
#define CMD_ERROR "ERROR"

/**
 * Echo the player's action back to them.
 */
#define CMD_OK "OK"

/* Regular expressions used to match messages */

#define REGEX_MSG_IPP ("^" CMD_IPP REGEX_SPACE REGEX_PROTOCOL_VERSION REGEX_SPACE REGEX_INFO "$")

#define REGEX_MSG_BUYIN ("^" CMD_BUYIN REGEX_SPACE REGEX_LOGIN REGEX_SPACE REGEX_AMT "$")

#define REGEX_MSG_WELCOME ("^" CMD_WELCOME REGEX_SPACE REGEX_NAME "$")

#define REGEX_MSG_NEWGAME ("^" CMD_NEWGAME REGEX_SPACE REGEX_GAME_TYPE REGEX_SPACE REGEX_BLINDS REGEX_SPACE REGEX_RAISES_PER_ROUND "$")

#define REGEX_MSG_PLAYER ("^" CMD_PLAYER REGEX_SPACE REGEX_NAME REGEX_SPACE REGEX_AMT "$")

#define REGEX_MSG_BUTTON ("^" CMD_BUTTON REGEX_SPACE REGEX_NAME "$")

#define REGEX_MSG_ANTE ("^" CMD_ANTE REGEX_SPACE REGEX_AMT "$")

#define REGEX_MSG_DEAL ("^" CMD_DEAL REGEX_SPACE REGEX_HOLE_CARDS "$")

#define REGEX_MSG_FROM ("^" CMD_FROM REGEX_SPACE REGEX_NAME REGEX_SPACE REGEX_INFO "$")

#define REGEX_MSG_FLOP ("^" CMD_FLOP REGEX_SPACE REGEX_CARD REGEX_SPACE REGEX_CARD REGEX_SPACE REGEX_CARD "$")

#define REGEX_MSG_TURN ("^" CMD_TURN REGEX_SPACE REGEX_CARD "$")

#define REGEX_MSG_RIVER ("^" CMD_RIVER REGEX_SPACE REGEX_CARD "$")

#define REGEX_MSG_DRAWQ ("^" CMD_DRAWQ "$")

#define REGEX_MSG_DRAW ("^" CMD_DRAW REGEX_SPACE REGEX_DRAW_N "$")

#define REGEX_MSG_DRAWN ("^" CMD_DRAWN REGEX_SPACE REGEX_DRAW_N "$")

#define REGEX_MSG_FOLD ("^" CMD_FOLD "$")

#define REGEX_MSG_UP ("^" CMD_UP REGEX_SPACE REGEX_CARD "$")

#define REGEX_MSG_DOWN ("^" CMD_DOWN REGEX_SPACE REGEX_CARD "$")

#define REGEX_MSG_ACTION ("^" CMD_ACTIONQ REGEX_SPACE REGEX_ACTION REGEX_SPACE REGEX_AMT "$")

#define REGEX_MSG_BLIND ("^" CMD_BLIND REGEX_SPACE REGEX_AMT "$")

#define REGEX_MSG_TAPOUT ("^" CMD_TAPOUT REGEX_SPACE REGEX_AMT "$")

#define REGEX_MSG_STRADDLE ("^" CMD_STRADDLE REGEX_SPACE REGEX_AMT "$")

#define REGEX_MSG_CALL ("^" CMD_CALL REGEX_SPACE REGEX_AMT "$")

#define REGEX_MSG_RAISE ("^" CMD_RAISE REGEX_SPACE REGEX_AMT "$")

#define REGEX_MSG_OPEN ("^" CMD_OPEN REGEX_SPACE REGEX_AMT "$")

#define REGEX_MSG_CHECK ("^" CMD_CHECK "$")

#define REGEX_MSG_SHOWQ ("^" CMD_SHOWQ "$")

#define REGEX_MSG_SHOW ("^" CMD_SHOW REGEX_SPACE REGEX_HANDTYPE "$")

#define REGEX_MSG_BEATQ ("^" CMD_BEATQ REGEX_SPACE REGEX_HANDTYPE "$")

#define REGEX_MSG_NO ("^" CMD_NO "$")

#define REGEX_MSG_YES ("^" CMD_YES REGEX_SPACE REGEX_HANDTYPE "$")

#define REGEX_MSG_WINNER ("^" CMD_WINNER REGEX_SPACE REGEX_NAME REGEX_SPACE REGEX_AMT REGEX_SPACE REGEX_HANDTYPE "$")

#define REGEX_MSG_BUSTED ("^" CMD_BUSTED REGEX_SPACE REGEX_NAME "$")

#define REGEX_MSG_GAMEOVER ("^" CMD_GAMEOVER REGEX_SPACE REGEX_NAME REGEX_SPACE REGEX_AMT "$")

#define REGEX_MSG_QUIT ("^" CMD_QUIT REGEX_SPACE REGEX_NAME "$")

#define REGEX_MSG_ERROR ("^" CMD_ERROR REGEX_SPACE REGEX_INFO "$")

#define REGEX_MSG_OK ("^" CMD_OK REGEX_SPACE REGEX_INFO "$")

#define MSG_IPP 0
#define MSG_BUYIN 1
#define MSG_WELCOME 2
#define MSG_NEWGAME 3
#define MSG_PLAYER 4
#define MSG_BUTTON 5
#define MSG_ANTE 6
#define MSG_DEAL 7
#define MSG_FROM 8
#define MSG_FLOP 9
#define MSG_TURN 10
#define MSG_RIVER 11
#define MSG_DRAWQ 12
#define MSG_DRAW 13
#define MSG_DRAWN 14
#define MSG_FOLD 15
#define MSG_UP 16
#define MSG_DOWN 17
#define MSG_ACTION 18
#define MSG_BLIND 19
#define MSG_TAPOUT 20
#define MSG_STRADDLE 21
#define MSG_CALL 22
#define MSG_RAISE 23
#define MSG_OPEN 24
#define MSG_CHECK 25
#define MSG_WINNER 26
#define MSG_BUSTED 27
#define MSG_GAMEOVER 28
#define MSG_QUIT 29
#define MSG_SHOWQ 30
#define MSG_SHOW 31
#define MSG_BEATQ 32
#define MSG_NO 33
#define MSG_YES 34
#define MSG_ERROR 35
#define MSG_OK 36
#define MSG_STRAIGHTFLUSH 37
#define MSG_FOUROFAKIND 38
#define MSG_FULLHOUSE 39
#define MSG_FLUSH 40
#define MSG_STRAIGHT 41
#define MSG_THREEOFAKIND 42
#define MSG_TWOPAIR 43
#define MSG_ONEPAIR 44
#define MSG_HIGHCARD 45

#define IPP_EVAL_C ( 2ll)
#define IPP_EVAL_H ( 3ll)
#define IPP_EVAL_D ( 5ll)
#define IPP_EVAL_S ( 7ll)
#define IPP_EVAL_2 (11ll)
#define IPP_EVAL_3 (13ll)
#define IPP_EVAL_4 (17ll)
#define IPP_EVAL_5 (19ll)
#define IPP_EVAL_6 (23ll)
#define IPP_EVAL_7 (29ll)
#define IPP_EVAL_8 (31ll)
#define IPP_EVAL_9 (37ll)
#define IPP_EVAL_T (41ll)
#define IPP_EVAL_J (43ll)
#define IPP_EVAL_Q (47ll)
#define IPP_EVAL_K (53ll)
#define IPP_EVAL_A (59ll)

#define IPP_EVAL_STRAIGHT_5 (  2725151ll)
#define IPP_EVAL_STRAIGHT_6 (  1062347ll)
#define IPP_EVAL_STRAIGHT_7 (  2800733ll)
#define IPP_EVAL_STRAIGHT_8 (  6678671ll)
#define IPP_EVAL_STRAIGHT_9 ( 14535931ll)
#define IPP_EVAL_STRAIGHT_T ( 31367009ll)
#define IPP_EVAL_STRAIGHT_J ( 58642669ll)
#define IPP_EVAL_STRAIGHT_Q ( 95041567ll)
#define IPP_EVAL_STRAIGHT_K (162490421ll)
#define IPP_EVAL_STRAIGHT_A (259106347ll)

#define IPP_EVAL_NPRIMES (17)

/* function prototypes */

/**
 * Initializes underlying libraries. This function *must* be called before performing any network operations!
 */
	void ipp_init(void);

/**
 * De-initializes underlying libraries. This function *must* be called last!
 */
	void ipp_exit(void);

/**
 * Validates IPP Messages
 * @param regex one of the REGEX constants.
 * @param msg a message.
 * @return TRUE if msg is valid, 0 if msg is not valid.
 */
	gboolean ipp_validate_msg(char *regex, char *msg);

/**
 * Validates an arbitrary IPP Messages.
 * @param msg a message.
 * @return constant for message type (example MSG_IPP), -1 if msg is not valid.
 */
	gint8 ipp_validate_unknown_msg(char *msg);

/**
 * Convert the string to upper case and convert multiple spaces to 1 space.
 * Trim leading and trailing white space.
 * @param msg the message, a null terminated string, to transform.
 */
	void ipp_normalize_msg(char *msg);

/**
 * Connect to a server.
 * @param hostname the hostname of the server to connect to (example: host.domain.tld).
 * @param port TCP/IP port for the connection.
 * @return a socket or NULL if an error happened.
 */
	ipp_socket *ipp_connect(char *hostname, short port);

/**
 * Disconnect from the server.
 * @param sock a socket to disconnect.
 */
	void ipp_disconnect(ipp_socket * sock);

/**
 * Read a message from the socket.
 * @param sock the socket to read from.
 * @param timeout_seconds number of seconds to wait for input.
 * @return a valid normalized message or NULL if message is invalid. All messages need to be deallocate by the user with g_free().
 */
	ipp_message *ipp_read_msg(ipp_socket * sock, guint8 timeout_seconds, void (*logger) (char *));

/**
 * Send a message to the socket. It will be normalized and validated by this function before sending.
 * @param sock the socket to read from.
 * @param msg the message to send.
 * @param timeout_seconds number of seconds to wait for output.
 * @return TRUE if msg was sent OK, else FALSE for error.
 */
	gboolean ipp_send_msg(ipp_socket * sock, ipp_message * msg, guint8 timeout_seconds, void (*logger) (char *));

/**
 * Main server loop. This function sets up the networking and accepts
 * incoming connections. For every incoming client, a 'callback' is
 * called. The server blocks and waits for 'callback' to return, so
 * make 'callback' short and sweet.
 * @param port TCP/IP port to listen on.
 * @param callback function to call when a new client connects.
 */
	void ipp_servloop(int port, void (*callback) (ipp_socket *));

/**
 * Causes the servloop to terminate gracefully.
 */
	void ipp_servloop_shutdown(void);

/**
 * Maps a product of the prime representation of ranks of cards in the
 * hands to the character representing the highest card in the straight.
 * @param str a product of primes that represent a straight.
 * @return a character representing the highest card in the straight.
 */
	char ipp_eval_straight2char(long long str);

/**
 * Maps the prime representation of a rank _or_ suit to a character.
 * @param p the prime number.
 * @return a chacter representing the rank or suit ('C' = clubs, 'T' = 10, '2' = 2, etc).
 */
	char ipp_eval_prime2char(long long p);

/**
 * Maps a character representation of a rank _or_ suit to a prime number.
 * @param c the character ('C' = clubs, 'T' = 10, '2' = 2, etc).
 * @return a prime number used for hand evaluation.
 */
	long long ipp_eval_char2prime(char c);

/**
 * Maps a card to a prime number based representation of the card.
 * @param card the card to map.
 * @return the prime number based representation of the card or 0ll if card is NULL.
 */
	long long ipp_eval_card2prime(ipp_card * card);

/**
 * Evaluate a 5 card hand. 
 * @param cards a hand to evaluate.
 * @return an IPP message containing a formated 'handtype' string as the payload and the type as the type.
 */
	ipp_message *ipp_eval(ipp_card * cards[5]);

/**
 * This method evaluates all of possible hands that can be made out of a player's hole cards and the board cards.
 * @param table a table where stage is set to SHOWDOWN, the board has all its cards, and the player has all his or her cards.
 * @param playerid the index of the player in the table->players array.
 * @return a message representing the best hand. NULL is returned when this function is called with bad parameters.
 */
	ipp_message *ipp_best_combination(ipp_table * table, int playerid);

/**
 * Ranks players based on their best hand.
 * @param table a table in the SHOWDOWN stage.
 * @return a list of playerid's sorted by 1st place, 2nd place, etc. Don't forget to free the result.
 */
	guint8 *ipp_rank_players(ipp_table * table);

/**
 * Comparitor for qsort and other similar sorting functions.
 * @param ipp_message_a an ipp_message pointer.
 * @param ipp_message_b an ipp_message pointer.
 * @return 1, 0 or -1 if ipp_message_a is greater than, equal to or less than ipp_message_b.
 */
	int ipp_hand_compar(const void *ipp_message_a, const void *ipp_message_b);

/**
 * Handshake Helper Function. This should be called by the server from the client connect callback.
 * @param sock the connected socket.
 * @param server_tag server name and version (example "tinypokerd/0.0.0").
 * @param auth a callback function to authenticate the user.
 * @param logger a callback function to log the protocol messages (optional). If no logger, use NULL.
 */
	ipp_player *ipp_server_handshake(ipp_socket * sock, char *server_tag, int (*auth) (char *), void (*logger) (char *));

/**
 * Handshake Helper Function. This should be called by the client.
 * @param hostname the name of the server.
 * @param port TCP/IP port. 
 * @param user the username (in upper case).
 * @param buyin_amt the inital money amount for this player.
 * @param logger a callback function to log the protocol messages (optional). If no logger, use NULL.
 */
	ipp_socket *ipp_client_handshake(char *hostname, short port, char *user, char *buyin_amt, void (*logger) (char *));

/**
 * Deal cards to the players at the table.
 * @param table to deal at.
 * @param timeout number of seconds to wait for output.
 * @param logger a callback function to log the protocol messages (optional). If no logger, use NULL.
 */
	void ipp_deal(ipp_table * table, guint8 timeout, void (*logger) (char *));

#endif

#ifdef __cplusplus
}
#endif
