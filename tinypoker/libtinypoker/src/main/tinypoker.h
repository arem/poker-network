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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TINYPOKER_H
#define __TINYPOKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gnutls/gnutls.h>
#include <netinet/in.h>

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

/**
 * Major version number. This is updated when major re-writes or
 * changes to the API occur requiring implementing programs to be
 * changed.
 */
#define LIBTINYPOKER_MAJOR_VERSION 0

/**
 * Minor version number. This is updated when new APIs are added
 * that don't change existing APIs.
 */
#define LIBTINYPOKER_MINOR_VERSION 0

/**
 * Patch version number. This is updated when bugs are fixed that don't
 * add any new APIs.
 */
#define LIBTINYPOKER_PATCH_VERSION 0

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
	RIVER
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
	int		sd;
	struct sockaddr_storage sockaddr;
	socklen_t sockaddrlen;

	gnutls_session_t session;
	gnutls_certificate_credentials_t x509_cred;
}		ipp_socket;

/**
 * Structure used to hold protocol messages.
 */
typedef struct ipp_message {
	int		type;
	char           *payload;
	char          **parsed;
}		ipp_message;

/**
 * Structure used to hold a playing card.
 */
typedef struct ipp_card {
	enum card_rank	rank;
	enum card_suit	suit;
}		ipp_card;

/**
 * Structure used to hold a poker player.
 * Some fields aren't used in some contexts.
 *   the server may know all players' hole cards.
 *   the client may only know his/her hole cards.
 */
typedef struct ipp_player {
	char           *name;	/* player name given at handshake */
	ipp_socket     *sock;	/* communications socket */

	/* TODO Support for Stud and Draw */
	ipp_card       *hole[HOLDEM_HOLE_CARDS];	/* two hole cards */
	int		bankroll;	/* amount of money this player has */
	int		amt_in;	/* amount into the pot from this player */
	int		still_in;	/* TRUE if still in the hand, FALSE
					 * if folded, disconnected, etc. */
}		ipp_player;

/**
 * Structure used to hold a poker table.
 */
typedef struct ipp_table {
	enum game_type	type;
	int		num_players;
	int		amt_to_call;
	enum holdem_stage stage;

	/* TODO Support for Stud and Draw */
	ipp_player     *players[HOLDEM_PLAYERS_PER_TABLE];
	ipp_card       *board[HOLDEM_BOARD_CARDS];
}		ipp_table;

/**
 * Allocates an empty ipp_socket. Don't forget to call ipp_free_socket().
 * @return a malloc()'d ipp_socket structure.
 * @see ipp_free_socket()
 */
ipp_socket     *ipp_new_socket();

/**
 * Deallocate an ipp_socket.
 */
void		ipp_free_socket(ipp_socket * sock);

/**
 * Allocates an empty ipp_message. Don't forget to ipp_free_message().
 * @return a malloc()'d ipp_message structure.
 */
ipp_message    *ipp_new_message();

/**
 * Parses msg->payload into msg->parsed
 * @param msg an IPP message
 */
void		ipp_parse_msg(ipp_message * msg);

/**
 * Deallocate an ipp_message.
 */
void		ipp_free_message(ipp_message * msg);

/**
 * Allocates an empty ipp_card. Don't forget to ipp_free_card().
 * @return a malloc()'d ipp_card structure.
 */
ipp_card       *ipp_new_card();

/**
 * Deallocate an ipp_card.
 */
void		ipp_free_card(ipp_card * card);

/**
 * Allocates an empty ipp_player. Don't forget to ipp_free_player().
 * @return a malloc()'d ipp_card structure.
 */
ipp_player     *ipp_new_player();

/**
 * Deallocates an ipp_player.
 */
void		ipp_free_player(ipp_player * player);

/**
 * Allocates an empty ipp_table. Don't for get to ipp_free_table().
 * @return a malloc()'d ipp_table structure.
 */
ipp_table      *ipp_new_table();

/**
 * Deallocates an ipp_table.
 */
void		ipp_free_table(ipp_table * table);

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
#define CLIENT_READ_TIMEOUT (600)

/**
 * The number of seconds a server should wait for network input.
 * This should be much smaller than CLIENT_READ_TIMEOUT.
 * @see CLIENT_READ_TIMEOUT
 */
#define SERVER_READ_TIMEOUT (10)

/**
 * The number of seconds a client should wait for network output.
 * This should be much higher than SERVER_WRITE_TIMEOUT.
 * @see SERVER_WRITE_TIMEOUT
 */
#define CLIENT_WRITE_TIMEOUT (600)

/**
 * The number of seconds a server should wait for network output.
 * This should be much smaller than CLIENT_WRITE_TIMEOUT.
 * @see CLIENT_WRITE_TIMEOUT
 */
#define SERVER_WRITE_TIMEOUT (10)

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

#define REGEX_STRAIGHTFLUSH "STRAIGHTFLUSH"
#define REGEX_STRAIGHTFLUSH_HANDTYPE REGEX_STRAIGHTFLUSH REGEX_SPACE REGEX_RANK

#define REGEX_FOUROFAKIND "FOUROFAKIND"
#define REGEX_FOUROFAKIND_HANDTYPE REGEX_FOUROFAKIND REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK

#define REGEX_FULLHOUSE "FULLHOUSE"
#define REGEX_FULLHOUSE_HANDTYPE REGEX_FULLHOUSE REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK

#define REGEX_FLUSH "FLUSH"
#define REGEX_FLUSH_HANDTYPE REGEX_FLUSH REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK

#define REGEX_STRAIGHT "STRAIGHT"
#define REGEX_STRAIGHT_HANDTYPE REGEX_STRAIGHT REGEX_SPACE REGEX_RANK

#define REGEX_THREEOFAKIND "THREEOFAKIND"
#define REGEX_THREEOFAKIND_HANDTYPE REGEX_THREEOFAKIND REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK

#define REGEX_TWOPAIR "TWOPAIR"
#define REGEX_TWOPAIR_HANDTYPE REGEX_TWOPAIR REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK

#define REGEX_ONEPAIR "ONEPAIR"
#define REGEX_ONEPAIR_HANDTYPE REGEX_ONEPAIR REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK

#define REGEX_HIGHCARD "HIGHCARD"
#define REGEX_HIGHCARD_HANDTYPE REGEX_HIGHCARD REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK REGEX_SPACE REGEX_RANK

/**
 * Declaration of a hand that can be made from a player's cards
 */
#define REGEX_HANDTYPE "(" REGEX_STRAIGHTFLUSH_HANDTYPE "|" REGEX_FOUROFAKIND_HANDTYPE "|" REGEX_FULLHOUSE_HANDTYPE "|" REGEX_FLUSH_HANDTYPE "|" REGEX_STRAIGHT_HANDTYPE "|" REGEX_THREEOFAKIND_HANDTYPE "|" REGEX_TWOPAIR_HANDTYPE "|" REGEX_ONEPAIR_HANDTYPE "|" REGEX_HIGHCARD_HANDTYPE ")"

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

#define REGEX_MSG_BUYIN ("^" CMD_BUYIN REGEX_SPACE REGEX_NAME REGEX_SPACE REGEX_AMT "$")

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

/* Regular Expressions for all supported message types */

static char    *ipp_regex_msg[] = {
	REGEX_MSG_IPP,
	REGEX_MSG_BUYIN,
	REGEX_MSG_WELCOME,
	REGEX_MSG_NEWGAME,
	REGEX_MSG_PLAYER,
	REGEX_MSG_BUTTON,
	REGEX_MSG_ANTE,
	REGEX_MSG_DEAL,
	REGEX_MSG_FROM,
	REGEX_MSG_FLOP,
	REGEX_MSG_TURN,
	REGEX_MSG_RIVER,
	REGEX_MSG_DRAWQ,
	REGEX_MSG_DRAW,
	REGEX_MSG_DRAWN,
	REGEX_MSG_FOLD,
	REGEX_MSG_UP,
	REGEX_MSG_DOWN,
	REGEX_MSG_ACTION,
	REGEX_MSG_BLIND,
	REGEX_MSG_TAPOUT,
	REGEX_MSG_STRADDLE,
	REGEX_MSG_CALL,
	REGEX_MSG_RAISE,
	REGEX_MSG_OPEN,
	REGEX_MSG_CHECK,
	REGEX_MSG_WINNER,
	REGEX_MSG_BUSTED,
	REGEX_MSG_GAMEOVER,
	REGEX_MSG_QUIT,
	REGEX_MSG_SHOWQ,
	REGEX_MSG_SHOW,
	REGEX_MSG_BEATQ,
	REGEX_MSG_NO,
	REGEX_MSG_YES,
	REGEX_MSG_ERROR,
	REGEX_MSG_OK,
	NULL
};

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

/* function prototypes */

/**
 * Initializes underlying libraries. This function *must* be called before performing any network operations!
 */
void		ipp_init  ();

/**
 * De-initializes underlying libraries. This function *must* be called last!
 */
void		ipp_exit  ();

/**
 * Validates IPP Messages
 * @param regex one of the REGEX constants.
 * @param msg a message.
 * @return 1 if msg is valid, 0 if msg is not valid.
 */
int		ipp_validate_msg(char *regex, char *msg);

/**
 * Validates an arbitrary IPP Messages.
 * @param msg a message.
 * @return constant for message type (example MSG_IPP), -1 if msg is not valid.
 */
int		ipp_validate_unknown_msg(char *msg);

/**
 * Convert the string to upper case and convert multiple spaces to 1 space.
 * Trim leading and trailing white space.
 * @param msg the message, a null terminated string, to transform.
 */
void		ipp_normalize_msg(char *msg);

/**
 * Checks a certificate to make sure it is valid.
 * @param session GNU TLS Session.
 * @param hostname the hostname of the server connected to.
 */
int		__ipp_verify_cert(gnutls_session session, const char *hostname);

/**
 * Connect to a server.
 * @param hostname the hostname of the server to connect to (example: host.domain.tld).
 * @param ca_file Path to Certificate Authority file.
 * @return a socket or NULL if an error happened.
 */
ipp_socket     *ipp_connect(char *hostname, char *ca_file);

/**
 * Disconnect from the server.
 * @param sock a socket to disconnect.
 */
void		ipp_disconnect(ipp_socket * sock);

/**
 * Read a message from the socket.
 * @param sock the socket to read from.
 * @param timeout number of seconds to wait for input.
 * @return a valid normalized message or NULL if message is invalid. All messages need to be deallocate by the user with g_free().
 */
ipp_message    *ipp_read_msg(ipp_socket * sock, int timeout);

/**
 * Send a message to the socket. It will be normalized and validated by this function before sending.
 * @param sock the socket to read from.
 * @param msg the message to send.
 * @param timeout number of seconds to wait for output.
 * @return TRUE if msg was sent OK, else FALSE for error.
 */
int		ipp_send_msg(ipp_socket * sock, ipp_message * msg, int timeout);

/**
 * INTERNAL STRUCT. DO NOT USE OUTSIDE LIBTINYPOKER!!!
 * Parameters passed to the reader thread.
 */
typedef struct __ipp_readln_thread_params {
	ipp_socket     *sock;
	char          **buffer;
	unsigned int   *n;
}		__ipp_readln_thread_params;

/**
 * INTERNAL FUNCTION. DO NOT USE OUTSIDE LIBTINYPOKER!!!
 * @param void_params a __ipp_readln_thread_params structure.
 */
void		__ipp_readln_thread(void *void_params);

/**
 * INTERNAL STRUCT. DO NOT USE OUTSIDE LIBTINYPOKER!!!
 * Parameters passed to the writer thread.
 */
typedef struct __ipp_writeln_thread_params {
	ipp_socket     *sock;
	char           *buffer;
	unsigned int   *n;
}		__ipp_writeln_thread_params;

/**
 * INTERNAL FUNCTION. DO NOT USE OUTSIDE LIBTINYPOKER!!!
 * @param void_params a __ipp_readln_thread_params structure.
 */
void		__ipp_writeln_thread(void *void_params);

/**
 * Main server loop. This function sets up the networking and accepts
 * incoming connections. For every incoming client, a 'callback' is
 * called. The server blocks and waits for 'callback' to return, so
 * make 'callback' short and sweet.
 * @param callback function to call when a new client connects.
 * @param ca_file Certificate Authority
 * @param crl_file CRL
 * @param cert_file SSL/TLS Certificate File
 * @param key_file Private Key
 */
void		ipp_servloop(void (*callback) (ipp_socket *), char *ca_file, char *crl_file, char *cert_file, char *key_file);

#endif

#ifdef __cplusplus
}
#endif
