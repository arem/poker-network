/*
 * BagelBot - Trivial client for pokerd
 * Copyright (C) 2005, 2006, 2007 Thomas Cort <code@member.fsf.org>
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

#ifndef __NET_H
#define __NET_H

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "byte.h"

/**
 * Socket descriptor. This is the socket we use to communicate with the 
 * server.
 * @see connect_to_server()
 * @see disconnect_from_server()
 */
int sd;

/**
 * Read a message and store it in a byte_array.
 * @param type the type of message read from the socket.
 * @return the message itself.
 */
struct byte_array *read_message(int *type);

/**
 * Write a message to the socket.
 * @param type the type of message we're sending.
 * @param ba the message itself.
 */
void write_message(int type, struct byte_array *);

/**
 * Connect to the server.
 * @param hostname server to connect to.
 * @param the TCP port to connect to.
 */
void connect_to_server(char *hostname, int port);

/**
 *  Shutdown and close the connection with the server .
 */
void disconnect_from_server();

#endif
