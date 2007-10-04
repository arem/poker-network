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

#include "tinypoker.h"

#include <ctype.h>
#include <regex.h>
#include <string.h>

/**
 * Convert the string to upper case.
 * Convert multiple spaces to 1 space.
 * Trim leading and trailing white space.
 * Should be called before ipp_validate_msg()
 * @param msg the message, a null terminated string, to transform.
 */
void ipp_normalize_msg(char *msg) {
	int len, i, j;
	len = strlen(msg);

	j = 0; /* Trim leading white space */
	while ((msg[j] == ' ' || msg[j] == '\t') && (j < len)) {
		j++;
	}

	/* Trim whitespace as we go. Convert everything to upper case. */
	for (i = 0; j < len && i < len; i++, j++) {
		msg[i] = toupper(msg[j]);
		if ((msg[i] == ' ' || msg[i] == '\t') && (msg[j+1] == ' ' || msg[j+1] == '\t')) {
			i--;
		}
	}

	/* Trim that last whitespace character not caught by the 'for' loop */
	if (i > 0 && (msg[i-1] == ' ' || msg[i-1] == '\t')) {
		i--;
	}

	msg[i] = '\0';
}


/**
 * Validates IPP Messages
 * @param regex one of the REGEX constants.
 * @param msg a message.
 * @return 1 if msg is valid, 0 if msg is not valid.
 */
int ipp_validate_msg(char *regex, char *msg) {
	regex_t preg;
	int ret;

	ret = regcomp(&preg, regex, REG_EXTENDED);
	if (ret) { /* compile the pattern */
		return 0;
	}

	/* See if the message matches */
	ret = regexec(&preg, msg, 0, 0, 0);
	regfree(&preg); /* Clean up */

	return !ret;
}

