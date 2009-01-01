/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <linuxgeek@gmail.com>
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

#ifndef __CONFIG_H
#define __CONFIG_H

#include <confuse.h>
#include <tinypoker.h>

char *setuid_name;

/**
 * The default username to run as.
 */
#define DEFAULT_SETUID_NAME "tinypokerd"

char *setgid_name;

/**
 * The default group to run as.
 */
#define DEFAULT_SETGID_NAME "tinypokerd"

char *x509_ca;

/**
 * The default CA file location.
 */
#define DEFAULT_X509_CA "/etc/tinypokerd/ca.pem"

char *x509_crl;

/**
 * The default CRL file location.
 */
#define DEFAULT_X509_CRL "/etc/tinypokerd/crl.pem"

char *x509_cert;

/**
 * The default certificate location.
 */
#define DEFAULT_X509_CERT "/etc/tinypokerd/cert.pem"

char *x509_key;

/**
 * The default private key location.
 */
#define DEFAULT_X509_KEY "/etc/tinypokerd/key.pem"

/**
 * Protocol logging flag. When true, protocol messages are written to protocol_log_file.
 */
cfg_bool_t protocol_log_enabled;

/**
 * The default setting for the protocol logging flag (false).
 */
#define DEFAULT_PROTOCOL_LOG_ENABLED cfg_false

/**
 * Protocol log file location
 */
char *protocol_log_file;

/**
 * The default protocol log file location.
 */
#define DEFAULT_PROTOCOL_LOG_FILE "/var/log/tinypokerd/protocol.log"

/**
 * The type of game we're playing (holdem, draw, stud)
 */
enum game_type game_type;

/**
 * The default value for game_type (Texas Holdem).
 */
#define DEFAULT_GAME_TYPE HOLDEM

/**
 * The path to the configuration file.
 */
char *configfile;

/**
 * The default configuration file location.
 */
#define DEFAULT_CONFIGFILE "/etc/tinypokerd/tinypokerd.conf"

/**
 * Parses an tinypokerd.conf configuration file.
 */
void config_parse(void);

/**
 * Release any resources that hold configuration information.
 * This function effectively resets all configurable values.
 * It should be called at the end of the program.
 */
void config_free(void);

#endif
