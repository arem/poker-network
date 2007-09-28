# Copyright (C) 2005, 2006, 2007 Thomas Cort <code@member.fsf.org>
#
# This file is part of pokerd.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

CC     = gcc
CFLAGS = 
INCLUDE= -I/usr/include/poker-eval -I/usr/include/mysql
LIBDIR = -L/usr/lib -L/usr/lib32 -L/usr/lib64 -L/usr/lib/mysql -L/usr/lib32/mysql -L/usr/lib64/mysql
LIBS   = -lpthread -lmysqlclient -lz -lcrypt -lnsl -lm -lgsl -lgslcblas -lpoker-eval -lssl -ldaemon

LEX    = flex
YACC   = bison
LFLAGS =
YFLAGS = -d

ifeq ($(MAKECMDGOALS), warn)
CFLAGS = -Wall -pedantic -ansi
endif

ifeq ($(MAKECMDGOALS), optimize)
CFLAGS = -O3
endif

ifeq ($(MAKECMDGOALS), debug)
CFLAGS = -g3 -O0
endif

BINARY   = pokerd
LEXSPEC  = config.l
YACCSPEC = config.y

SOURCES  = $(LEXSPEC:.l=.lex.c) $(YACCSPEC:.y=.y.tab.c) byte.c conf.c \
	daemon.c db.c deck.c main.c md5.c monitor.c net.c poker.c \
	pokerserv.c rand.c sig.c eval.c

all optimize main debug warn: $(YACCSPEC:.y=.y.tab.c) $(LEXSPEC:.l=.lex.c) $(SOURCES:.c=.o)
	$(CC) $(LIBS) $(LIBDIR) $(INCLUDE) $(CFLAGS) -o $(BINARY) $(SOURCES:.c=.o)

clean:
	rm -rf *.o *.lex.c *.y.tab.? working $(BINARY)

help:
	@echo -e "Usage:"
	@echo -e "  make [all]\t\tBuild all"
	@echo -e "  make warn\t\tInclude Warnings"
	@echo -e "  make optimize\t\tEnable optimizations and nothing else"
	@echo -e "  make debug\t\tInclude Debugging Info and Warnings"
	@echo -e "  make help\t\tDisplay this help"

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -o $(<:.c=.o) -c $<

.SUFFIXES: .l .lex.c
.l.lex.c:
	$(LEX) $(LFLAGS) -o$(<:.l=.lex.c) $<

.SUFFIXES: .y .y.tab.c
.y.y.tab.c:
	$(YACC) $(YFLAGS) -b $< $<
