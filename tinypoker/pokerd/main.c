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

#include "auth.h"
#include "conf.h"
#include "db.h"
#include "deck.h"
#include "daemon.h"
#include "sig.h"
#include "log.h"
#include "pokerserv.h"
#include "rand.h"
#include "monitor.h"

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <syslog.h>
#include <strings.h>

extern int snprintf(char *str, size_t size, const char *format, ...);

int main(int argc, char *argv[]) {
	int option, option_index = 0;
	char buf[128];
	struct stat s;

	static struct option long_options[] = {
		{"debug"        , 0, 0, 1},
		{"log"          , 0, 0, 2},
		{"port"         , 1, 0, 3},
		{"help"         , 0, 0, 4},
		{"config"       , 1, 0, 5},
		{"dbusername", 1, 0, 6},
		{"dbpassword", 1, 0, 7},
		{"dbhostname", 1, 0, 8},
		{"dbdatabase", 1, 0, 9},
		{0       , 0, 0, 0}
	};

	debug      = 0;
	use_syslog = 1;

	memset(&s,0,sizeof(struct stat));
	bzero(configfile,sizeof(configfile));

	default_config(); /* configure with default settings */

	/* try reading default config file ".pokerd" */
	if (stat(".pokerd",&s) != -1) {
		/* if no error, then .pokerd exists */
		strcpy(configfile,".pokerd");
		read_config();
	}

	bzero(configfile,sizeof(configfile));

	while(1) {
		option = getopt_long (argc, argv, "dlp:hc:",long_options, &option_index);

		if (option == -1)
			break;

		switch(option) {
			/* debug */
			case  1 :
			case 'd':
				debug      = 1;
				use_syslog = 0;
				break;

			/* log */
			case  2 :
			case 'l':
				use_syslog = 1;
				break;

			/* port */
			case  3 :
			case 'p':
				port = atoi(optarg);
				break;

			/* help */
			case  4 :
			case 'h':
				printf("--log                   || -l            = log using syslog\n");
                                printf("--debug                 || -d            = run in foreground\n");
                                printf("--port (port)           || -p (port)     = tcp port for file server\n");
				printf("--configfile (filename) || -c (filename) = configuration\n");
				printf("--help                  || -h            = display help message\n");
				printf("--dbusername (username)                  = username for db server\n");
				printf("--dbpassword (password)                  = password for db server\n");
				printf("--dbhostname (hostname)                  = hostname for db server\n");
				printf("--dbdatabase (database)                  = database for db server\n");
				exit(1);
                                break;

			/* config */
			case  5 :
			case 'c':
				strcpy(configfile,optarg);
				read_config();
				break;

			/* dbusername */
			case  6 :
				strcpy(dbusername,optarg);
				break;

			/* dbpassword */
			case  7 :
				strcpy(dbpassword,optarg);
				break;

			/* dbhostname */
			case  8 :
				strcpy(dbhostname,optarg);
				break;

			/* dbdatabase */
			case  9 :
				strcpy(dbdatabase,optarg);
				break;


			default:
				printf("Invalid Option\n");
				exit(1);
		}
	}

	install_signal_handlers();

	if (!debug) {
		daemonize();
		cd_working_dir();
	}

	if (use_syslog)
		openlog((argv[0] != NULL ? argv[0] : "pokerd"),LOG_PID,LOG_LOCAL6);

	/* Start up Message */

	bzero(buf,128);
	snprintf(buf,127,"[INFO] pokerd version %s Copyright (C) 2005, 2006 Thomas Cort",version);
	logit(buf);

	bzero(buf,128);
	snprintf(buf,127,"[INFO] pokerd comes with ABSOLUTELY NO WARRANTY; for details");
	logit(buf);

	bzero(buf,128);
	snprintf(buf,127,"[INFO] please read the GNU GPL (http://www.gnu.org/licenses/gpl.txt)");
	logit(buf);

	bzero(buf,128);
	snprintf(buf,127,"[INFO] pokerd is free software, and you are welcome to redistribute");
	logit(buf);

	bzero(buf,128);
	snprintf(buf,127,"[INFO] it under certain conditions; for details please read the");
	logit(buf);

	bzero(buf,128);
	snprintf(buf,127,"[INFO] GNU General Public License (http://www.gnu.org/licenses/gpl.txt)");
	logit(buf);


	db_players_kill(); /* set all Sock Descs to zero in the database */
	/* this is done in case the server crashed or was shutdown with players playing */

	monitor_init();
	rand_init();
	deck_init();

	pokermain(port);

	rand_destroy();

	if (use_syslog)
		closelog();

	monitor_wait();
	pthread_mutex_destroy(&auth_lock);
	pthread_mutex_destroy(&db_lock);
	return 0;
}
