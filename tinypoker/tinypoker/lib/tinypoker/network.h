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

/*
 * This file contains the networking operation interfaces.
 * Most of these operate on gnet data structures.
 * It is very important that you only use these interfaces
 * and types and never mix them with gnet interfaces or types.
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TINYPOKER_NETWORK_H

/**
 * Used interally to prevent double inclusion of this header file.
 */
#define __TINYPOKER_NETWORK_H

#include <glib.h>
#include <gnet.h>

/**
 * Make GConn a TPConn.
 * This hides gnet and provides a way to add extra attributes in the future.
 * You should never manipulate the members of a TPConn directly, use one the the tp_* functions.
 */
typedef GConn TPConn;

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
TPConn *tp_conn_new(const gchar *hostname, gint port);

/**
 * Establish a connection. If a connection is already established, this function has no effect.
 * @param conn the connection object to use -- which probably was created by tp_conn_new().
 * @see tp_conn_new()
 * @see tp_conn_is_connected()
 */
void tp_conn_connect(TPConn *conn);

/**
 * Determines if the connection is established.
 * @param conn the connection to test.
 * @return TRUE if connected, FALSE if not connected.
 * @see tp_conn_connect()
 * @see tp_conn_disconnect()
 */
gboolean tp_conn_is_connected(const TPConn *conn);

/**
 * Disconnect an established connection.
 * If a connection is already disconnected, this function has no effect.
 * @param conn the connection to disconnect.
 * @see tp_conn_is_connected()
 * @see tp_conn_disconnect()
 */
void tp_conn_disconnect(TPConn *conn);

/**
 * Decrements the internal reference counter.
 * Object is deleted when the reference count reaches 0.
 * @see tp_conn_new()
 */
void tp_conn_unref(TPConn *conn);

#endif /* __TINYPOKER_NETWORK_H */

#ifdef __cplusplus
}
#endif

