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

#define _GNU_SOURCE

#include <confuse.h>
#include <libdaemon/dlog.h>
#include <tinypoker.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "config.h"

/**
 * Release any resources that hold configuration information.
 * This function effectively resets all configurable values.
 * It should be called at the end of the program.
 */
void config_free(void)
{
	if (setuid_name) {
		free(setuid_name);
		setuid_name = NULL;
	}

	if (setgid_name) {
		free(setgid_name);
		setgid_name = NULL;
	}

	if (x509_ca) {
		free(x509_ca);
		x509_ca = NULL;
	}

	if (x509_crl) {
		free(x509_crl);
		x509_crl = NULL;
	}

	if (x509_cert) {
		free(x509_cert);
		x509_cert = NULL;
	}

	if (x509_key) {
		free(x509_key);
		x509_key = NULL;
	}

	protocol_log_enabled = cfg_false;

	if (protocol_log_file) {
		free(protocol_log_file);
		protocol_log_file = NULL;
	}

	game_type = UNSPECIFIED;
}

/**
 * Examines each value of the current configuration.
 * If a value was not set, it is set to the default.
 */
static void config_with_defaults(void)
{
	if (protocol_log_enabled != cfg_false && protocol_log_enabled != cfg_true) {
		protocol_log_enabled = DEFAULT_PROTOCOL_LOG_ENABLED;
	}

	if (game_type == UNSPECIFIED) {
		game_type = DEFAULT_GAME_TYPE;
	}

	if (setuid_name == NULL) {
		setuid_name = strdup(DEFAULT_SETUID_NAME);
	}

	if (setgid_name == NULL) {
		setgid_name = strdup(DEFAULT_SETGID_NAME);
	}

	if (x509_ca == NULL) {
		x509_ca = strdup(DEFAULT_X509_CA);
	}

	if (x509_crl == NULL) {
		x509_crl = strdup(DEFAULT_X509_CRL);
	}

	if (x509_cert == NULL) {
		x509_cert = strdup(DEFAULT_X509_CERT);
	}

	if (x509_key == NULL) {
		x509_key = strdup(DEFAULT_X509_KEY);
	}

	if (protocol_log_file == NULL) {
		protocol_log_file = strdup(DEFAULT_PROTOCOL_LOG_FILE);
	}
}

/**
 * Parses an tinypokerd.conf configuration file.
 */
void config_parse(void)
{
	int rc;

	config_free();

	cfg_t *cfg;
	cfg_opt_t opts[] = {
		CFG_SIMPLE_STR("setuid", &setuid_name),
		CFG_SIMPLE_STR("setgid", &setgid_name),
		CFG_SIMPLE_STR("x509_ca", &x509_ca),
		CFG_SIMPLE_STR("x509_crl", &x509_crl),
		CFG_SIMPLE_STR("x509_cert", &x509_cert),
		CFG_SIMPLE_STR("x509_key", &x509_key),
		CFG_SIMPLE_BOOL("protocol_log_enabled", &protocol_log_enabled),
		CFG_SIMPLE_STR("protocol_log_file", &protocol_log_file),
		CFG_SIMPLE_INT("game_type", &game_type),
		CFG_END()
	};

	cfg = NULL;

	daemon_log(LOG_DEBUG, "[CONF] parsing config file");

	cfg = cfg_init(opts, 0);
	if (!cfg) {
		daemon_log(LOG_ERR, "[CONF] cfg_init failed! Loading defaults...");
		config_with_defaults();
		return;
	}
	rc = cfg_parse(cfg, configfile);
	if (rc == CFG_PARSE_ERROR) {
		daemon_log(LOG_ERR, "[CONF] parser error '%s'", DEFAULT_CONFIGFILE);
	}
	if (cfg) {
		cfg_free(cfg);
		cfg = NULL;
	}

	if (setuid_name == NULL) {
		daemon_log(LOG_ERR, "[CONF] setuid not present in config file, defaulting to '%s'", DEFAULT_SETUID_NAME);
	}

	if (setgid_name == NULL) {
		daemon_log(LOG_ERR, "[CONF] setgid not present in config file, defaulting to '%s'", DEFAULT_SETGID_NAME);
	}

	if (x509_ca == NULL) {
		daemon_log(LOG_ERR, "[CONF] x509_ca not present in config file, defaulting to '%s'", DEFAULT_X509_CA);
	}

	if (x509_crl == NULL) {
		daemon_log(LOG_ERR, "[CONF] x509_crl not present in config file, defaulting to '%s'", DEFAULT_X509_CRL);
	}

	if (x509_cert == NULL) {
		daemon_log(LOG_ERR, "[CONF] x509_cert not present in config file, defaulting to '%s'", DEFAULT_X509_CERT);
	}

	if (x509_key == NULL) {
		daemon_log(LOG_ERR, "[CONF] x509_key not present in config file, defaulting to '%s'", DEFAULT_X509_KEY);
	}

	if (protocol_log_enabled != cfg_false && protocol_log_enabled != cfg_true) {
		daemon_log(LOG_ERR, "[CONF] rotocol_log_enabled not present in config file, defaulting to 'false'");
	}

	if (protocol_log_file == NULL) {
		daemon_log(LOG_ERR, "[CONF] protocol_log_file not present in config file, defaulting to '%s'", DEFAULT_PROTOCOL_LOG_FILE);
	}

	if (game_type < 0 || game_type > 3) {
		daemon_log(LOG_ERR, "[CONF] game_type not present in config file or invalid, defaulting to holdem");
		game_type = HOLDEM;
	}

	config_with_defaults();
}
