/*
 * Copyright (C) 2007 Thomas Cort <code@member.fsf.org>
 *
 * This file is part of libtinypoker.
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

#ifndef __TINYPOKER_H
#define __TINYPOKER_H

/**
 * The name of this library for IPP strings, etc.
 */
#define TINYPOKER "TinyPoker"

/**
 * The version of the protocol implemented.
 */
#define REGEX_PROTOCOL_VERSION "2.0"

/* Regular expressions used to match message parts */

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
#define REGEX_CARD (REGEX_RANK REGEX_SUIT)

/**
 * Name of player (no spaces).
 */
#define REGEX_NAME "[0-9a-zA-Z_]+"

/**
 * Amount of Money
 */
#define REGEX_AMT "[0-9]+"

/**
 * Whitespace characters used to separate message parts.
 */
#define REGEX_SPACE "[\t ]+"

/**
 * Whitespace characters used to separate message parts.
 */
#define REGEX_INFO "[0-9a-zA-Z/.\t ]+"

/* protocol commands */

/**
 * Server welcome message sent after a client connects.
 */
#define CMD_IPP "IPP"

/**
 * Client response to IPP. This command asks the server to add the player to the game.
 */
#define CMD_BUYIN "BUYIN"

/* Regular expressions used to match messages */

#define REGEX_MSG_IPP ("^" CMD_IPP REGEX_SPACE REGEX_PROTOCOL_VERSION REGEX_SPACE REGEX_INFO "$")

#define REGEX_MSG_BUYIN ("^" CMD_BUYIN REGEX_SPACE REGEX_NAME REGEX_SPACE REGEX_AMT "$")

/* function prototypes */

/**
 * Validates IPP Messages
 * @param regex one of the REGEX constants.
 * @param msg a message.
 * @return 1 if msg is valid, 0 if msg is not valid.
 */
int ipp_validate_msg(char *regex, char *msg);

/**
 * Convert the string to upper case and convert multiple spaces to 1 space.
 * Trim leading and trailing white space.
 * @param msg the message, a null terminated string, to transform.
 */
void ipp_normalize_msg(char *msg);

#endif
