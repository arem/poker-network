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
#include "daemon.h"
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern int getdtablesize(void);
int snprintf(char *str, size_t size, const char *format, ...);

/**
 *  void write_pid_file()
 *
 *  create a pid file in the current directory
 */
void write_pid_file() {
	int fd;
	char pid[16];

	bzero(pid,sizeof(pid));
	snprintf(pid,15,"%d\n",getpid());

	if ((fd = open("pid",O_WRONLY|O_TRUNC|O_CREAT,S_IRUSR|S_IWUSR)) < 0) {
		char buf[128];
		bzero(buf,128);
		snprintf(buf,127,"[FILE] %s",strerror(errno));
		logit(buf);
        }

	if (write(fd,pid,strlen(pid)) < 0) {
		char buf[128];
		bzero(buf,128);
		snprintf(buf,127,"[FILE] %s",strerror(errno));
		logit(buf);
	}

	if (close(fd) < 0) {
		char buf[128];
		bzero(buf,128);
		snprintf(buf,127,"[FILE] %s",strerror(errno));
		logit(buf);
	}
}

/**
 *  void cd_working_dir()
 *
 *  change to the working directory and create pid file
 */
void cd_working_dir() {
	int fd;
        struct stat s;
        memset(&s,0,sizeof(struct stat));

	/* create a working directory if one doesn't exist */
	if ((fd = stat("./working",&s)) == -1) {
		if (mkdir("./working",S_IRWXU) < 0) {
			char buf[128];
			bzero(buf,128);
			snprintf(buf,127,"[FILE] %s",strerror(errno));
			logit(buf);
		}
	} else if (fd < 0) {
		char buf[128];
		bzero(buf,128);
		snprintf(buf,127,"[FILE] %s",strerror(errno));
		logit(buf);
	}

	/* check for pid file */
	memset(&s,0,sizeof(struct stat));
	if (stat("./working/pid",&s) < -1) {
		char buf[128];
		bzero(buf,128);
		snprintf(buf,127,"[FILE] %s",strerror(errno));
		logit(buf);
	}

	/* if pid has some data, then exit */
	if (s.st_size > 0) {
                logit("server already running!\n");
                exit(1);
        }

	chdir("./working");
	write_pid_file();
}

/**
 *  void daemonize()
 *
 *  preforms the standard functions to become a daemon
 *  as discussed in class
 */
void daemonize() {

	int fd, child_pid = fork();
	char buf[129];
	buf[128]='\0';

	if (child_pid < 0) {
		char buf[128];
		bzero(buf,128);
		snprintf(buf,127,"[PROC] %s",strerror(errno));
		logit(buf);
		exit(1);
	} else if (child_pid > 0)
		exit(0);

	/* close open file descriptors */
	for(fd = 0; fd < getdtablesize(); fd++) {
		/* don't close stdout stderr */
		if (fd != 1 && fd != 2)
			close(fd);
	}

	/* re-open stdin */
	fd = open("/dev/null", O_RDWR);

	/* detach from terminal */
	fd = open("/dev/tty",O_RDWR);
	ioctl(fd,TIOCNOTTY,0);
	close(fd);

	/* change user:group to nobody:nobody */
	setuid(99);
	setgid(99);

	/* create new process group */
	setpgid(getpid(),0);
}
