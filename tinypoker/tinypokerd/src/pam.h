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

#ifndef __PAM_H
#define __PAM_H

#include <security/pam_appl.h>

#define PAM_SERVICE_NAME "tinypoker"

/**
 * The PAM conversation callback.
 * @param num_msg number of conversation messages.
 * @param msg a conversation message.
 * @param resp a conversation response of size num_msg.
 * @param appdata_ptr data.
 * @return PAM_SUCCESS or PAM_CONV_ERR
 */
int ipp_pam_conv(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr);

/**
 * Authenticate a User with PAM
 * @param user username
 * @param pass password
 * @return TRUE for authenticated or FALSE for failed.
 */
int ipp_auth(char *user, char *pass);

#endif
