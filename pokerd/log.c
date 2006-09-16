/*
 * Copyright (C) 2005, 2006 Thomas Cort
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
#include "time.h"

#include <stdio.h>
#include <string.h>
#include <strings.h>

int snprintf(char *str, size_t size, const char *format, ...);

/* use_syslog is defined in log.h */
/* if set then print to syslog    */
/* else print to setout           */

/**
 *  void logit(char * message)
 *
 *  General logging facility. If use_syslog set log with LOG_NOTICE else print
 */
void logit(char * message) {
	char logstr[256];
	bzero(logstr,256);

	snprintf(logstr,255,"[%d][%d] %s\n",tdate(),ttime(),message);

	if (use_syslog > 1) {
		syslog(LOG_NOTICE,logstr);
		printf(logstr);
	} else if (use_syslog) {
		syslog(LOG_NOTICE,logstr);
	} else {
		printf(logstr);
	}
	
}

/**
 *  void logemerg(char * message)
 *
 *  If use_syslog set log with LOG_EMERG else print
 */
void logemerg(char * message) {
	char logstr[256];
	bzero(logstr,256);

	snprintf(logstr,255,"[%d][%d] %s\n",tdate(),ttime(),message);

	if (use_syslog > 1) {
		syslog(LOG_EMERG,logstr);
		printf(logstr);
	} else if (use_syslog) {
		syslog(LOG_EMERG,logstr);
	} else {
		printf(logstr);
	}
	
}
