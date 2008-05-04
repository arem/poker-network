/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This file is part of TinyPoker.
 *
 * TinyPoker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TinyPoker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TinyPoker.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <tinypoker/tinypoker.h>

/**
 * !!! INTERNAL FUNCTION. DO NOT USE. !!!
 * @param conn the connection object.
 * @param event the current event.
 * @param user_data an event loop.
 */
static void __tp_conn_callback(GConn *conn, GConnEvent *event, gpointer user_data) {
	GMainLoop *loop;

	g_assert(conn != NULL);
	g_assert(event != NULL);
	g_assert(user_data != NULL);

	loop = user_data;

	switch (event->type) {
		case GNET_CONN_CONNECT:
		case GNET_CONN_TIMEOUT:
		case GNET_CONN_ERROR:
		case GNET_CONN_CLOSE:
		case GNET_CONN_WRITE:
		case GNET_CONN_READ:
			gnet_conn_timeout(conn, 0);  /* clear timeout */
			g_main_loop_quit(loop);
			break;

		default:
			g_assert_not_reached();
	}

	return;
}

/**
 * Allocate a new connection object. The connection is not
 * established until you call tp_conn_connect().
 * To de-allocate the connection, use tp_conn_unref().
 * @param hostname hostname of the server you wish to connect to.
 * @param port TCP port of the server you wish to connect to.
 * @return a new unconnected TPConn. Free with tp_conn_unref().
 * @see tp_conn_unref()
 * @see tp_conn_connect()
 */
TPConn *tp_conn_new(const gchar *hostname, gint port) {
	TPConn *conn;
	GMainLoop *loop;

	g_assert(hostname != NULL);
	g_assert(1 <= port && port <= 65535);

	loop = g_main_loop_new(NULL, FALSE);
	conn = gnet_conn_new(hostname, port, __tp_conn_callback, loop);
	g_assert(conn != NULL);

	return conn;
}

/**
 * Establish a connection. If a connection is already established, this function has no effect.
 * @param conn the connection object to use -- which probably was created by tp_conn_new().
 * @see tp_conn_new()
 * @see tp_conn_is_connected()
 */
void tp_conn_connect(TPConn *conn) {
	GMainLoop *loop;
	gboolean is_connected;

	g_assert(conn != NULL);
	is_connected = tp_conn_is_connected(conn);
	if (is_connected) {
		return;
	}

	loop = conn->user_data;
	g_assert(loop != NULL);

	gnet_conn_timeout(conn, 30000);  /* 30 second timeout */
	gnet_conn_set_watch_error(conn, TRUE);
	gnet_conn_connect(conn);

	g_main_run(loop);
	g_main_loop_unref(loop);
	loop = NULL;

	return;
}

/**
 * Determines if the connection is established.
 * @param conn the connection to test.
 * @return TRUE if connected, FALSE if not connected.
 * @see tp_conn_connect()
 * @see tp_conn_disconnect()
 */
gboolean tp_conn_is_connected(const TPConn *conn) {
	g_assert(conn != NULL);
	return gnet_conn_is_connected(conn);
}

/**
 * Disconnect an established connection.
 * If a connection is already disconnected, this function has no effect.
 * @param conn the connection to disconnect.
 * @see tp_conn_is_connected()
 * @see tp_conn_disconnect()
 */
void tp_conn_disconnect(TPConn *conn) {
	gboolean is_connected;
	g_assert(conn != NULL);

	is_connected = tp_conn_is_connected(conn);
	if (is_connected) {
		gnet_conn_disconnect(conn);
	}
}

/**
 * Decrements the internal reference counter.
 * Object is deleted when the reference count reaches 0.
 * @see tp_conn_new()
 */
void tp_conn_unref(TPConn *conn) {
	g_assert(conn != NULL);
	gnet_conn_unref(conn);
}
