/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 * 
 * This file is part of tinypokerd.
 * 
 * tinypokerd is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * tinypokerd is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * tinypokerd.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE

#include <libdaemon/dlog.h>
#include <security/pam_appl.h>
#include <string.h>
#include <tinypoker.h>

#include "pam.h"

/**
 * The PAM conversation callback.
 * @param num_msg number of conversation messages.
 * @param msg a conversation message.
 * @param resp a conversation response of size num_msg.
 * @param appdata_ptr data.
 * @return PAM_SUCCESS or PAM_CONV_ERR
 */
int ipp_pam_conv(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr)
{
	int i;
	char *pass;
	struct pam_response *resp_ptr;

	pass = (char *) appdata_ptr;
	resp_ptr = NULL;

	if (num_msg < 0) {
		daemon_log(LOG_ERR, "[AUTH] conversation with a negative number of messages.");
		return PAM_CONV_ERR;
	}

	if (pass == NULL) {
		daemon_log(LOG_ERR, "[AUTH] appdata_ptr is NULL?!?!");
		return PAM_CONV_ERR;
	}

	resp_ptr = (struct pam_response *) malloc(sizeof(struct pam_response) * num_msg);
	if (resp_ptr == NULL) {
		daemon_log(LOG_ERR, "[AUTH] malloc failed");
		return PAM_CONV_ERR;
	}

	for (i = 0; i < num_msg; i++) {
		switch (msg[i]->msg_style) {
		case PAM_PROMPT_ECHO_ON:
		case PAM_PROMPT_ECHO_OFF:
			resp_ptr[i].resp_retcode = PAM_SUCCESS;

			resp_ptr[i].resp = strdup(pass);
			if (resp_ptr[i].resp == NULL) {
				daemon_log(LOG_ERR, "[AUTH] malloc failed");
				for (i = i - 1; i >= 0; i--) {
					if (resp_ptr[i].resp) {
						free(resp_ptr[i].resp);
						resp_ptr[i].resp = NULL;
					}
				}
				return PAM_CONV_ERR;
			}
			break;

		case PAM_TEXT_INFO:
			daemon_log(LOG_INFO, "[AUTH] libpam: '%s'", msg[i]->msg);
			resp_ptr[i].resp_retcode = PAM_SUCCESS;
			resp_ptr[i].resp = NULL;
			break;

		case PAM_ERROR_MSG:
			daemon_log(LOG_ERR, "[AUTH] libpam: '%s'", msg[i]->msg);
			resp_ptr[i].resp_retcode = PAM_SUCCESS;
			resp_ptr[i].resp = NULL;
			break;

		default:
			daemon_log(LOG_ERR, "[AUTH] unknown conversaion message");
			for (i = i - 1; i >= 0; i--) {
				if (resp_ptr[i].resp) {
					free(resp_ptr[i].resp);
					resp_ptr[i].resp = NULL;
				}
			}
			return PAM_CONV_ERR;
			break;
		}
	}

	*resp = resp_ptr;
	return PAM_SUCCESS;
}

/**
 * Authenticate a User with PAM
 * @param user username
 * @param pass password
 * @return TRUE for authenticated or FALSE for failed.
 */
int ipp_auth(char *user, char *pass)
{
	int rc;
	pam_handle_t *pamh;

	struct pam_conv conv = {
		ipp_pam_conv,
		pass
	};

	pamh = NULL;

	rc = pam_start(PAM_SERVICE_NAME, user, &conv, &pamh);
	if (rc != PAM_SUCCESS) {
		daemon_log(LOG_ERR, "[AUTH] could not allocate PAM handle.");
		return FALSE;
	}

	rc = pam_authenticate(pamh, 0);
	if (rc != PAM_SUCCESS) {
		daemon_log(LOG_ERR, "[AUTH] pam_authenticate() failed");
		return FALSE;
	}

	rc = pam_acct_mgmt(pamh, 0);
	if (rc != PAM_SUCCESS) {
		daemon_log(LOG_ERR, "[AUTH] pam_acct_mgmt() failed");
		return FALSE;
	}

	if (pam_end(pamh, rc) != PAM_SUCCESS) {
		daemon_log(LOG_ERR, "[AUTH] could not free PAM handle.");
		return FALSE;
	}

	return TRUE;
}
