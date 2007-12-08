/*
 * Copyright (C) 2005, 2006, 2007 Thomas Cort <code@member.fsf.org>
 *
 * This file is part of tinypokerd.
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

#include <errno.h>
#include <getopt.h>
#include <libdaemon/dlog.h>
#include <libdaemon/dpid.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "config.h"
#include "monitor.h"
#include "pokerserv.h"
#include "signal.h"

/**
 * Determines if gatewayavd should run in the background (daemonized) or
 * not. If daemonize is 1, then gatewayavd should run in the background.
 */
int daemonize;

/**
 * Determines if our process killed a running tinypokerd process successfully.
 */
int killed;

/**
 * Displays some version and copyright information upon request (-v or --version).
 */
void display_version()
{
	daemon_log(LOG_INFO, "tinypokerd v0.1");
	daemon_log(LOG_INFO, "Copyright (C) 2005, 2006, 2007 Thomas Cort");
	daemon_log(LOG_INFO, "This is free software; see the source for copying conditions.  There is NO");
	daemon_log(LOG_INFO, "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.");
}


/**
 * Displays some usage information, command line parameters and whatnot.
 * @param program the name of the program.
 */
void display_help(char *program)
{
	daemon_log(LOG_INFO, "Usage: %s [options]", program);
	daemon_log(LOG_INFO, "Options:");
	daemon_log(LOG_INFO, "    -h --help          Show this help message");
	daemon_log(LOG_INFO, "    -v --version       Show version information");
	daemon_log(LOG_INFO, "    -k --kill          Kill the running instance");
	daemon_log(LOG_INFO, "    -f --foreground    Run in the foreground");
}

/**
 * A command line parser using getopts.
 * @param argc The number of command line arguments coming in argv.
 * @param argv The command line arguments.
 * @return Returns 0 on success and non-zero when we want the program to terminate.
 */
int parse_args(int argc, char **argv)
{
	int option_index;
	int done;

	static struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"version", no_argument, 0, 'v'},
		{"kill", no_argument, 0, 'k'},
		{"foreground", no_argument, 0, 'f'},
		{0, 0, 0, 0}
	};

	option_index = 0;
	done = 0;

	while (!done) {
		int c;
		int ret;

		c = getopt_long(argc, argv, "hvkf", long_options, &option_index);
		if (c < 0) {
			break;
		}

		switch (c) {
		case 'h':
			display_help(argv[0]);
			done = 1;
			break;
		case 'v':
			display_version();
			done = 1;
			break;
		case 'k':
			ret = daemon_pid_file_kill_wait(SIGQUIT, 30);
			if (ret < 0) {
				daemon_log(LOG_ERR, "Daemon not killed: (%s)", strerror(errno));
			} else {
				killed = 1;
			}
			done = 1;
			break;
		case 'f':
			daemonize = 0;
			break;
		default:
			daemon_log(LOG_ERR, "Unsupported option");
			done = 1;
			break;
		}
	}

	return done;
}

int main(int argc, char *argv[], char *envp[])
{
	int fd;
	int ret;
	pid_t pid;

	/* Default Values for Global Variables */
	daemonize = 1;
	killed = 0;

	/* Sanity Checks */
	if (argc < 1 || !argv || !argv[0]) {
		daemon_log(LOG_ERR, "(%u:%s) Cannot determine program name from argv[0]\n");
		return 1;
	}

	daemon_pid_file_ident = daemon_log_ident = daemon_ident_from_argv0(argv[0]);

	if (geteuid() != 0) {
		daemon_log(LOG_ERR, "You need root privileges to run this application.");
		return 1;
	}

	/* Command Line Arguements */
	ret = parse_args(argc, argv);
	if (ret) {
		return (killed ? 0 : ret);
	}

	pid = daemon_pid_file_is_running();
	if (pid > 0) {
		daemon_log(LOG_ERR, "%s is already running (PID => %u)", argv[0], daemon_log_ident, pid);
		daemon_log(LOG_INFO, "Use `%s -k` to kill the running instance", daemon_log_ident);
		return 1;
	}

	/* Daemonize */
	if (daemonize) {
		/* Configure Logging */
		daemon_log_use = DAEMON_LOG_SYSLOG;

		umask(0);

		pid = fork();
		if (pid < 0) {
			return 1;
		} else if (pid > 0) {
			return 0;
		}

		setsid();

		pid = fork();
		if (pid < 0) {
			return 1;
		} else if (pid > 0) {
			return 0;
		}

		ret = chdir("/");
		if (ret < 0) {
			daemon_log(LOG_ERR, "Could not chdir() to '/': %s", strerror(errno));
			return 1;
		}

		/* close open file descriptors */
		for (fd = 0; fd < getdtablesize(); fd++) {
			ret = close(fd);
			if (ret == -1 && errno != EBADF) {
				daemon_log(LOG_ERR, "Could not close fd #%d: %s", fd, strerror(errno));
				return 1;
			}
		}

		/* re-open stdin, stdout, stderr */
		fd = open("/dev/null", O_RDONLY);
		fd = open("/dev/null", O_WRONLY);
		fd = open("/dev/null", O_WRONLY);
	}

	ret = daemon_pid_file_create();
	if (ret < 0) {
		daemon_log(LOG_ERR, "Could not create PID file: %s", strerror(errno));
		return 1;
	}

	/* this must run before any threads are created */
	monitor_init();

	/* Configure */
	config_parse();

	/* Install Signal Handlers */
	install_signal_handlers();

	/* Play some poker until we get a SIGINT, SIGQUIT, or SIGKILL */
	pokerserv();

	monitor_wait();		/* thread cleanup */
	config_free();
	daemon_pid_file_remove();

	daemon_log(LOG_INFO, "Exiting...");

	return 0;
}
