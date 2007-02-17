%option nounput
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
#include <string.h>
#include <stdlib.h>
#include "config.y.tab.h"
#include "conf.h"

void yyerror(char *);
extern int fileno(FILE *);
#define YY_INPUT(buf,result,max_size) \
	{ \
	int c = fgetc(yyin); \
	result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
	}
%}
digit	[0-9]
integer	{digit}+
%%

dbusername	{
		return DBUSERNAME;
	}

dbpassword	{ 
		return DBPASSWORD;
	}

dbhostname	{
		return DBHOSTNAME;
	}

dbdatabase	{
		return DBDATABASE;
	}

port		{
		return PORT;
}

debug		{
		return DEBUG;
}

log		{
		return LOG;
}

{integer}	{
		yylval.integer = atoi(yytext);
		return INTEGER;
	}

[a-zA-z]+	{
		yylval.string = (char *) malloc(strlen(yytext)+strlen("\0"));
		strcpy(yylval.string,yytext);
		return STRING;
	}

\n	{ 
		lineno++;
		return NEWLINE;
	}

"="	{
		break;
	}
" "	{
		break;
	}
%%
	int yywrap() { return 1; }
