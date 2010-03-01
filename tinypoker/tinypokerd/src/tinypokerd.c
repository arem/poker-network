/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010 Thomas Cort <linuxgeek@gmail.com>
 *
 * This file is part of TinyPoker.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE

#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>

#include <unistd.h>

#include "log.h"
#include "monitor.h"
#include "pokerserv.h"
#include "signal.h"
#include "tinypokerd.h"

/**
 * Determines if tinypokerd should run in the background (daemonized) or
 * not. If daemonize is 1, then tinypokerd should run in the background.
 */
int daemonize;

/**
 * Determines if our process killed a running tinypokerd process successfully.
 */
int killed;

#define DEFAULT_PID_LOCATION "tinypokerd.pid"

/**
 * Location of the PID file as specified on the command line.
 */
char *pid_filename;

/**
 * Displays some version and copyright information upon request (-v or --version).
 */
void display_version(void)
{
	fprintf(stdout, "%s/%s\n", TINYPOKERD_NAME, TINYPOKERD_VERSION);
	fprintf(stdout, "Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010 Thomas Cort <linuxgeek@gmail.com>\n");
	fprintf(stdout, "This is free software; see the source for copying conditions.  There is NO\n");
	fprintf(stdout, "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
}


/**
 * Displays some usage information, command line parameters and whatnot.
 * @param program the name of the program.
 */
void display_help(char *program)
{
	fprintf(stdout, "Usage: %s [options]\n", program);
	fprintf(stdout, "Options:\n");
	fprintf(stdout, "    -h --help          Show this help message\n");
	fprintf(stdout, "    -v --version       Show version information\n");
	fprintf(stdout, "    -k --kill          Kill the running instance\n");
	fprintf(stdout, "    -f --foreground    Run in the foreground\n");
}

const char *get_pid_filename(void)
{
	if (pid_filename) {
		return pid_filename;
	} else {
		return DEFAULT_PID_LOCATION;
	}
}

int pid_create(void)
{
	int fd, rc;
	char pid[20];

	memset(pid, '\0', sizeof(char) * 20);

	fd = open(get_pid_filename(), O_CREAT | O_WRONLY | O_EXCL, 0644);
	if (fd == -1) {
		return -1;
	}

	snprintf(pid, 18, "%lu", (unsigned long) getpid());
	rc = write(fd, pid, strlen(pid));
	if (rc == -1) {
		close(fd);
		unlink(get_pid_filename());
		return -1;
	}

	close(fd);
	return 0;
}

int pid_kill(void)
{
	gsize length;
	gchar *contents;
	gboolean frc;
	pid_t pid;
	int rc;
	int i;

	contents = NULL;

	frc = g_file_get_contents(get_pid_filename(), &contents, &length, NULL);
	if (frc == FALSE) {
		if (contents) {
			free(contents);
			contents = NULL;
		}
		return -1;
	}

	if (length > 20) {
		if (contents) {
			free(contents);
			contents = NULL;
		}
		return -1;
	}

	pid = strtol(contents, NULL, 10);

	rc = kill(pid, SIGINT);
	for (i = 0; i < 30; i++) {
		rc = kill(pid, 0);
		if (rc < 0 && errno != ESRCH) {
			return -1;
		} else if (rc) {
			return 0;
		}
#ifdef _WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
	}

	return 1;
}

void pid_unlink(void)
{
	unlink(get_pid_filename());
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
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{"kill", no_argument, NULL, 'k'},
		{"foreground", no_argument, NULL, 'f'},
		{"pid-file", required_argument, NULL, 'p'},
		{0, 0, 0, 0}
	};

	option_index = 0;
	done = 0;

	while (!done) {
		int c;
		int ret;

		c = getopt_long(argc, argv, "hvkfp:", long_options, &option_index);
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
			ret = pid_kill();
			if (ret < 0) {
				fprintf(stderr, "[MAIN] Daemon not killed: (%s)\n", strerror(errno));
			} else {
				killed = 1;
			}
			done = 1;
			break;
		case 'f':
			daemonize = 0;
			break;
		case 'p':
			pid_filename = g_strdup(optarg);
			break;
		default:
			fprintf(stderr, "[MAIN] Unsupported option\n");
			done = 1;
			break;
		}
	}

	return done;
}

int main(int argc, char *argv[], char *envp[])
{
	int fd, ret;
	struct rlimit rlim;
	pid_t pid;

	/* Default Values for Global Variables */
	daemonize = 1;
	pid_filename = NULL;
	killed = 0;

	g_debug("[MAIN] Starting up tinypokerd...");

	if (argc < 1 || !argv || !argv[0]) {
		fprintf(stderr, "[MAIN] Cannot determine program name from argv[0]\n");
		return 1;
	}

	g_debug("[MAIN] program name is '%s'", argv[0]);

	/* Command Line Arguements */
	ret = parse_args(argc, argv);
	if (ret) {
		g_debug("[MAIN] parse_args() returned a non-zero value %d", ret);
		return (killed ? 0 : ret);
	}

	/* Daemonize */
	if (daemonize) {
		umask(0);

		g_debug("[MAIN] daemonize");

		pid = fork();
		if (pid < 0) {
			return 1;
		} else if (pid > 0) {
			return 0;
		}
		setsid();

		g_debug("[MAIN] fork() once");

		pid = fork();
		if (pid < 0) {
			return 1;
		} else if (pid > 0) {
			return 0;
		}

		g_debug("[MAIN] fork() twice");

		ret = chdir("/");
		if (ret < 0) {
			fprintf(stderr, "[MAIN] Could not chdir() to '/': %s\n", strerror(errno));
			return 1;
		}

		g_debug("[MAIN] fork() chdir");

		/* close open file descriptors */
		for (fd = 0; fd < getdtablesize(); fd++) {
			ret = close(fd);
			if (ret == -1 && errno != EBADF) {
				return 1;
			}
		}

		/* re-open stdin, stdout, stderr */
		fd = open("/dev/null", O_RDONLY);
		fd = open("/dev/null", O_WRONLY);
		fd = open("/dev/null", O_WRONLY);
	}

	/* disable core dumps so usernames and passwords aren't dumped. */
	rlim.rlim_cur = 0;
	rlim.rlim_max = 0;
	ret = setrlimit(RLIMIT_CORE, &rlim);
	if (ret < 0) {
		fprintf(stderr, "[MAIN] setrlimit() failed\n");
		return 1;
	}

	g_debug("[MAIN] disabled core dumps");

	/* set max file size to 8MB (8388608 bytes) */
	rlim.rlim_cur = 8388608;
	rlim.rlim_max = 8388608;
	ret = setrlimit(RLIMIT_FSIZE, &rlim);
	if (ret < 0) {
		fprintf(stderr, "[MAIN] setrlimit() failed\n");
		return 1;
	}

	g_debug("[MAIN] set max file size to 8MB");

	/* set max address space to 32MB (33554432 bytes) */
	rlim.rlim_cur = 33554432;
	rlim.rlim_max = 33554432;
	ret = setrlimit(RLIMIT_AS, &rlim);
	if (ret < 0) {
		fprintf(stderr, "[MAIN] setrlimit() failed\n");
		return 1;
	}

	g_debug("[MAIN] set max address space to 32 MB");

	/* set max file descriptor number */
	rlim.rlim_cur = 64;
	rlim.rlim_max = 64;
	ret = setrlimit(RLIMIT_NOFILE, &rlim);
	if (ret < 0) {
		fprintf(stderr, "[MAIN] setrlimit() failed\n");
		return 1;
	}

	g_debug("[MAIN] set max FDs to 64");

	/* set max threads to 64 */
/*
	rlim.rlim_cur = 64;
	rlim.rlim_max = 64;
	ret = setrlimit(RLIMIT_NPROC, &rlim);
	if (ret < 0) {
		fprintf(stderr, "[MAIN] setrlimit() failed\n");
		return 1;
	}
*/
	/* don't be a CPU hog */
	if (getpriority(PRIO_PROCESS, getpid()) < 0) {
		ret = setpriority(PRIO_PROCESS, getpid(), 0);
		if (ret == -1) {
			fprintf(stderr, "[MAIN] setpriority() failed\n");
			return 1;
		}
	}

	g_debug("[MAIN] set priority");

/*TODO: reimplement
	pid = daemon_pid_file_is_running();
	if (pid > 0) {
		fprintf(stderr, "[MAIN] %s is already running (PID => %u)\n", argv[0], pid);
		fprintf(stdout, "[MAIN] Use `%s -k` to kill the running instance\n", argv[0]);
		return 1;
	}
*/

	ret = pid_create();
	if (ret < 0) {
		fprintf(stderr, "[MAIN] Could not create PID file: %s\n", strerror(errno));
		return 1;
	}

	g_debug("[MAIN] PID file created");

	/* setup libtinypoker */
	ipp_init();

	g_debug("[MAIN] IPP Initialized");

	/* Install Signal Handlers */
	install_signal_handlers();

	g_debug("[MAIN] signal handlers installed");

	/* this must run before any threads are created */
	monitor_init();

	g_debug("[MAIN] monitor intialized");

	/* configure protocol logger - call this even when protocol logging is disabled */
	protocol_logger_init();

	g_debug("[MAIN] logger initialized");

	/* Play some poker until we get a SIGINT, SIGQUIT, or SIGKILL */
	pokerserv();

	g_debug("[MAIN] pokerserv() returned");

	monitor_wait();		/* thread cleanup */

	g_debug("[MAIN] all threads gone");

	/* clean up protocol logger - call this even when protocol logging is disabled */
	protocol_logger_exit();

	g_debug("[MAIN] protocol log closed");

	ipp_exit();

	g_debug("[MAIN] ipp de-initialized");

	pid_unlink();

	g_debug("[MAIN] PID file removed");

	if (pid_filename) {
		free(pid_filename);
		pid_filename = NULL;
	}

	g_debug("[MAIN] Bye bye");

	return 0;
}
