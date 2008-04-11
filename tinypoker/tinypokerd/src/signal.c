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

#include <libdaemon/dlog.h>
#include <signal.h>
#include <tinypoker.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <execinfo.h>
#include <bfd.h>

#ifndef __USE_GNU
#define __USE_GNU
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <dlfcn.h>

#define MAX_STACK_FRAMES (128)

#include "signal.h"
#include "tinypokerd.h"

/**
 * Global variable used to determine if the program should halt. Will it ever halt? Only Alan Turing knows.
 * @see handle_signal()
 */
int exit_now;

/**
 * Signal handler for SIGKILL
 * @param sig signal to handle.
 */
void handle_sigkill(int sig)
{
	if (sig == SIGKILL) {
		raise(SIGUSR2);
		exit_now = 1;
		daemon_log(LOG_INFO, "[SIGN] SIGKILL Caught ; preparing to exit");
	}
}

/**
 * Signal handler for SIGQUIT
 * @param sig signal to handle.
 */
void handle_sigquit(int sig)
{
	if (sig == SIGQUIT) {
		raise(SIGUSR2);
		exit_now = 1;
		daemon_log(LOG_INFO, "[SIGN] SIGQUIT Caught ; preparing to exit");
	}
}

/**
 * Signal handler for SIGINT
 * @param sig signal to handle.
 */
void handle_sigint(int sig)
{
	if (sig == SIGINT) {
		raise(SIGUSR2);
		exit_now = 1;
		daemon_log(LOG_INFO, "[SIGN] SIGINT Caught ; preparing to exit");
	}
}

/**
 * Signal handler for SIGSEGV
 * @param sig signal to handle.
 */
void handle_sigsegv(int sig)
{
	Dl_info info;
	struct utsname sys_uname;
	void *frames[MAX_STACK_FRAMES];
	asymbol **symbol_table;
	char **bt_symbols;
	size_t nbt_symbols, i;
	int rc;
	int symbol_table_size;
	int number_of_symbols, j, found;

	if (sig != SIGSEGV) {
		return;
	}

	daemon_log(LOG_ERR, "=-=-=-=-=-=-=-=-=-=-=-=-=-= Segmentation fault =-=-=-=-=-=-=-=-=-=-=-=-=-=");
	daemon_log(LOG_ERR, " ");
	daemon_log(LOG_ERR, "A fatal error occured causing this application to crash. Please report the");
	daemon_log(LOG_ERR, "entire error (everything between the 'Segmentation fault' lines) to the");
	daemon_log(LOG_ERR, "following developer: Tom Cort <tom@tomcort.com>");
	daemon_log(LOG_ERR, " ");

	nbt_symbols = backtrace(frames, MAX_STACK_FRAMES);
	bt_symbols = backtrace_symbols(frames, nbt_symbols);

	for (i = 0; i < nbt_symbols; i++) {
		rc = dladdr(frames[i], &info);
		if (rc != 0 && info.dli_fname != NULL && info.dli_fname[0] != '\0') {
			bfd *fd;

			fd = bfd_openr(info.dli_fname, NULL);
			if (fd == NULL) {
				continue;
			}

			rc = bfd_check_format_matches(fd, bfd_object, NULL);
			if (rc == 0) {
				bfd_close(fd);
				continue;
			}

			if (!(fd->flags & HAS_SYMS)) {
				bfd_close(fd);
				continue;
			}

			symbol_table_size = bfd_get_symtab_upper_bound(fd);
			if (symbol_table_size <= 0) {
				bfd_close(fd);
				continue;
			}

			symbol_table = (asymbol **) malloc(symbol_table_size);
			if (symbol_table == NULL) {
				bfd_close(fd);
				continue;
			}

			number_of_symbols = bfd_canonicalize_symtab(fd, symbol_table);
			if (number_of_symbols <= 0) {
				free(symbol_table);
				symbol_table = NULL;
				bfd_close(fd);
				continue;
			}

			found = 0;
			for (j = 0; j < number_of_symbols; j++) {
				const char *file;
				const char *func;
				unsigned int lineno;

				bfd_size_type sec_size;
				sec_size = bfd_section_size(fd, symbol_table[j]->section);

				if ((int) (frames[i]) < (int) (symbol_table[j]->section->vma)) {
					continue;
				}

				if ((int) (frames[i]) >= (int) (symbol_table[j]->section->vma + sec_size)) {
					continue;
				}

				bfd_find_nearest_line(fd, symbol_table[j]->section, symbol_table, (int) ((char *) frames[i] - symbol_table[j]->section->vma), &file, &func, &lineno);
				if (file) {
					daemon_log(LOG_ERR, "%16s() (%s:%d) [%p] <%s>", func, file, lineno, frames[i], info.dli_fname);
				} else {
					daemon_log(LOG_ERR, "%16s() [%p] <%s>", func, frames[i], info.dli_fname);
				}
				found = 1;
				break;
			}

			bfd_close(fd);
		}
	}

	free(bt_symbols);

	daemon_log(LOG_ERR, " ");
	daemon_log(LOG_ERR, "%s version %s was compiled at %s on %s", TINYPOKERD_NAME, TINYPOKERD_VERSION, __TIME__, __DATE__);

	rc = uname(&sys_uname);
	if (rc != -1) {
		daemon_log(LOG_ERR, "%s version %s %s %s", sys_uname.sysname, sys_uname.version, sys_uname.release, sys_uname.machine);
	}

	daemon_log(LOG_ERR, " ");
	daemon_log(LOG_ERR, "=-=-=-=-=-=-=-=-=-=-=-=-=-= Segmentation fault =-=-=-=-=-=-=-=-=-=-=-=-=-=");
	exit(1);
}

/**
 *  installs signal handlers (mostly SIG_IGN)
 */
void install_signal_handlers(void)
{

	int i;

	for (i = 0; i < 32; i++) {
		/*
		 * Set ignore for all signals except the 3 we handle
		 * (SIGKILL, SIGQUIT, SIGINT) and the 1 libtinypoker handles
		 * (SIGUSR2)
		 */

		if (i != SIGCHLD && i != SIGQUIT && i != SIGINT && i != SIGKILL && i != SIGUSR2 && i != SIGSEGV) {
			signal(i, SIG_IGN);
		}
	}

	signal(SIGKILL, handle_sigkill);
	signal(SIGQUIT, handle_sigquit);
	signal(SIGINT, handle_sigint);
	signal(SIGSEGV, handle_sigsegv);
}
