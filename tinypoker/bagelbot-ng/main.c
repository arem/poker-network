/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 * 
 * This file is part of bagelbot-ng.
 * 
 * bagelbot-ng is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * bagelbot-ng is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * bagelbot-ng.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <strings.h>
#include <getopt.h>
#include <libdaemon/dlog.h>
#include <tinypoker.h>

#include "conf.h"
#include "play.h"
#include "main.h"

/**
 * Handshake with the server.
 * @return a connected socket or NULL.
 */
ipp_socket *handshake(void)
{
	int rc;
	ipp_socket *sock;
	ipp_message *msg;

	daemon_log(LOG_INFO, "[HAND] [CONN] %s", host);

	sock = ipp_connect(host, "ca.pem");
	if (sock == NULL) {
		daemon_log(LOG_ERR, "[HAND] ipp_connect() failed");
		return NULL;
	}
	msg = ipp_read_msg(sock, CLIENT_READ_TIMEOUT);
	if (!msg || msg->type != MSG_IPP) {
		ipp_free_message(msg);
		msg = NULL;
		ipp_free_socket(sock);
		sock = NULL;
		return NULL;
	}
	daemon_log(LOG_INFO, "[HAND] [RECV] %s", msg->payload);
	ipp_free_message(msg);
	msg = NULL;

	msg = ipp_new_message();
	if (!msg) {
		daemon_log(LOG_ERR, "[HAND] malloc failed");
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
		return NULL;
	}
	msg->type = MSG_BUYIN;
	msg->payload = (char *) malloc(sizeof(char) * (strlen("BUYIN ") + strlen(user) + strlen(" 100") + 2));
	if (!(msg->payload)) {
		daemon_log(LOG_ERR, "[HAND] malloc failed");
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
		return NULL;
	}
	memset(msg->payload, '\0', (sizeof(char) * (strlen("BUYIN ") + strlen(user) + strlen(" 100") + 2)));
	snprintf(msg->payload, (sizeof(char) * (strlen("BUYIN ") + strlen(user) + strlen(" 100") + 1)), "%s%s%s", "BUYIN ", user, " 100");

	rc = ipp_send_msg(sock, msg, CLIENT_WRITE_TIMEOUT);
	if (!rc) {
		daemon_log(LOG_ERR, "[HAND] send failed");
		ipp_free_message(msg);
		msg = NULL;
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
		return NULL;
	}
	daemon_log(LOG_INFO, "[HAND] [SEND] %s", msg->payload);
	ipp_free_message(msg);
	msg = NULL;

	msg = ipp_read_msg(sock, CLIENT_READ_TIMEOUT);
	if (!msg || msg->type != MSG_WELCOME) {
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
		ipp_free_message(msg);
		msg = NULL;
		return NULL;
	}
	daemon_log(LOG_INFO, "[HAND] [RECV] %s", msg->payload);
	ipp_free_message(msg);
	msg = NULL;

	return sock;
}


/**
 * Displays some usage information, command line parameters and whatnot.
 * @param program the name of the program.
 */
void display_help(char *program)
{
	daemon_log(LOG_INFO, "Usage: %s [options]", program);
	daemon_log(LOG_INFO, "Options:");
	daemon_log(LOG_INFO, "    -h --help        Show this help message");
	daemon_log(LOG_INFO, "    -v --version     Show version information");
}

/**
 * Displays some version and copyright information upon request (-v or --version).
 */
void display_version(void)
{
	daemon_log(LOG_INFO, "%s v%1.1f", PROGRAM, VERSION);
	daemon_log(LOG_INFO, "Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>");
	daemon_log(LOG_INFO, "This is free software; see the source for copying conditions.  There is NO");
	daemon_log(LOG_INFO, "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.");
}


/**
 * A command line parser using getopts.
 * @param argc The number of command line arguments coming in argv.
 * @param argv The command line arguments.
 * @return Returns 0 on success and non-zero when we want the program to terminate.
 */
int parse_args(int argc, char **argv)
{
	int option_index = 0, done = 0, c;

	static struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"version", no_argument, 0, 'v'},
		{0, 0, 0, 0}
	};

	while (!done) {
		c = getopt_long(argc, argv, "hv", long_options, &option_index);
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
		default:
			display_help(argv[0]);
			done = 1;
			break;
		}
	}

	return done;
}

/**
 * A command line poker bot.
 * @param argc The number of command line arguments coming in argv.
 * @param argv The command line arguments.
 * @return Returns 0 on success and non-zero when we want the program to terminate.
 */
int main(int argc, char **argv)
{
	ipp_socket *sock;
	int rc;

	if (argc < 1 || !argv || !argv[0]) {
		daemon_log(LOG_ERR, "(%u:%s) Cannot determine program name from argv[0]", __LINE__, __FILE__);
		return 1;
	}
	/* Set the default config file path */
	configfile = (char *) malloc((strlen("bagelbot-ng.conf") + 2) * sizeof(char));
	if (!configfile) {
		daemon_log(LOG_ERR, "malloc() failed!");
		return 255;
	}
	bzero(configfile, (strlen("bagelbot-ng.conf") + 2) * sizeof(char));
	snprintf(configfile, strlen("bagelbot-ng.conf") + 1, "bagelbot-ng.conf");

	configure();

	rc = parse_args(argc, argv);
	if (rc) {
		return rc;
	}
	if (!host || !user) {
		daemon_log(LOG_ERR, "[MAIN] Could not determine one or more configuration setting from '%s'", configfile);
		free_config();
		return 255;
	} else {
		daemon_log(LOG_INFO, "[MAIN] Connecting to %s as %s", host, user);
	}

	ipp_init();

	if ((sock = handshake())) {
		daemon_log(LOG_INFO, "[MAIN] Handshake OK");
		play(sock);
		ipp_disconnect(sock);
		ipp_free_socket(sock);
		sock = NULL;
	} else {
		daemon_log(LOG_ERR, "[MAIN] Handshake Failed!");
	}

	ipp_exit();

	free_config();
	return 0;
}
