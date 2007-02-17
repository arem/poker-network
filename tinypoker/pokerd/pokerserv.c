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

#include "db.h"
#include "auth.h"
#include "sig.h"
#include "net.h"
#include "byte.h"
#include "log.h"
#include "poker.h"
#include "monitor.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/socket.h>

extern int snprintf(char *str, size_t size, const char *format, ...);

/**
 * main function for the poker server
 *
 * continuously-running thread which serves poker clients
 */
int pokermain(int port) {
	int master, slave;
	int *slavep;

	pthread_t tt;
	pthread_attr_t ta;

	pthread_t tt2;
	pthread_attr_t ta2;

	struct pollfd p;
	struct sockaddr_in client_addr;
	unsigned int client_addr_len;

	pthread_attr_init(&ta);
	pthread_attr_setdetachstate(&ta,PTHREAD_CREATE_DETACHED);

	pthread_attr_init(&ta2);
	pthread_attr_setdetachstate(&ta2,PTHREAD_CREATE_DETACHED);

	pthread_mutex_init(&auth_lock,0);
	pthread_mutex_init(&db_lock,0);

	client_addr_len = sizeof(client_addr);

	if ((master = passivesocket(port)) < 0) {
		char buf[128];
		bzero(buf,128);
		snprintf(buf,127,"[SOCK] %s",strerror(errno));
		logit(buf);
		exit(1);
	}

	p.fd      = master;
	p.events  = POLLIN;
	p.revents = 0;


	/* create a thread to play the game */
	monitor_inc();
	if (pthread_create(&tt2,&ta2,(void* (*) (void*))play, (void*)NULL) != 0) {
		char buf[128];
		bzero(buf,128);
		snprintf(buf,127,"[THRD] %s",strerror(errno));
		logit(buf);
		monitor_dec();
		exit(1);
	}

	/* listen and accept new connections */
	while (!done) {

		/* Poll master so that we don't block on accept */
		/* this is done so that when we signal we re-evaluate if !done == true */

		poll(&p,1,30000);  /* 30 second timeout */
		if (p.revents != POLLIN) { /* no activity */
			if (done) break;
			else   continue;
		}

		if ((slave = accept(master, (struct sockaddr*)&client_addr, &client_addr_len)) < 0) {
			char buf[128];
			if (errno == EINTR)
				continue;

			bzero(buf,128);
			snprintf(buf,127,"[SOCK] %s",strerror(errno));
			logit(buf);
			exit(1);
		}

		/* we need to do this instead of just casting the int to void* in pthread_create
		 * because on Alpha sizeof(int) = 4 and sizeof(void*) = 8 */
		 slavep = (int*) malloc(sizeof(int));
		*slavep = slave;

		monitor_inc();
		if (pthread_create(&tt,&ta,(void* (*) (void*))authenticate, (void*)slavep) != 0) {
			char buf[128];
			bzero(buf,128);
			snprintf(buf,127,"[THRD] %s",strerror(errno));
			logit(buf);
			monitor_dec();
			exit(1);
		}

	}

	pthread_attr_destroy(&ta);
	pthread_attr_destroy(&ta2);

	shutdown(master,SHUT_RDWR);
	close(master);

	return 0;
}
