%{

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include "conf.h"
#include "log.h"

extern int yylex (void);
extern int snprintf(char *str, size_t size, const char *format, ...);

void yyerror (char* str) {
	char logstr[256];
	bzero(logstr,256);
	snprintf(logstr,255,"configfile: %s on line %d\n",str,lineno);
	logit(logstr);
	exit(1);
}

%}

%union {
	int integer;
	char *string;
};
%token <integer> INTEGER
%token <string> STRING
%token NEWLINE DBUSERNAME DBPASSWORD PORT DEBUG LOG
%token DBHOSTNAME DBDATABASE
%type <integer> configarg_i
%type <string> configarg_s
%%

config: empty;
	| configlines;

configlines: configlines configline
	| configline;

configline: dbusername
	| dbpassword
	| dbhostname
	| dbdatabase
	| port
	| debug
	| log;


dbusername: DBUSERNAME configarg_s NEWLINE {
		strncpy(dbusername,$2,127);
		free($2);
	};

dbpassword: DBPASSWORD configarg_s NEWLINE {
		strncpy(dbpassword,$2,127);
		free($2);
	};

dbdatabase: DBDATABASE configarg_s NEWLINE {
		strncpy(dbdatabase,$2,127);
		free($2);
	};

dbhostname: DBHOSTNAME configarg_s NEWLINE {
		strncpy(dbhostname,$2,127);
		free($2);
	};

port: PORT configarg_i NEWLINE {
		port = $2;
	};

debug: DEBUG configarg_i NEWLINE {
		debug = $2;
	};

log: LOG configarg_i NEWLINE {
		use_syslog = $2;
	};

configarg_i: INTEGER {
		$$ = $1;
	};

configarg_s: STRING {
		$$ = $1;
	};

empty: /* Nothin' */;
%%

/* nothing */
