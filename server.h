/******************************************************************************
  Copyright (c) 1992, 1995, 1996 Xerox Corporation.  All rights reserved.
  Portions of this code were written by Stephen White, aka ghond.
  Use and copying of this software and preparation of derivative works based
  upon this software are permitted.  Any distribution of this software or
  derivative works must comply with all applicable United States export
  control laws.  This software is made available AS IS, and Xerox Corporation
  makes no warranty about the software, its performance or its conformity to
  any specification.  Any person obtaining a copy of this software is requested
  to send their name and post office or electronic mail address to:
    Pavel Curtis
    Xerox PARC
    3333 Coyote Hill Rd.
    Palo Alto, CA 94304
    Pavel@Xerox.Com
 *****************************************************************************/

/* server.h */

/*
 * This describes the complete set of procedures that a network implementation
 * is expected to use from the rest of the server.  See 'network.h' for the
 * complete set of procedures that a network implementation must provide.
 */

#ifndef Server_H
#define Server_H 1

#include <stdio.h>

#include "config.h"

typedef struct {		/* Server's handle on a connection */
    void *ptr;
} server_handle;

typedef struct {		/* Server's handle on a listening point */
    void *ptr;
} server_listener;

#include "network.h"		/* Include this *after* defining the types */

extern server_listener null_server_listener;

extern server_handle server_new_connection(server_listener l,
					 network_handle h, int outbound);
				/* Called by the network whenever a new player
				 * connection is created.  If `outbound' is
				 * true, then the connection is being made from
				 * the server to some external place, and L is
				 * NULL_SERVER_LISTENER; if `outbound' is
				 * false, the L is the listening point on which
				 * the new connection was received.  By this
				 * call, the network and server exchange tokens
				 * representing the connection for use in later
				 * calls on each other.
				 */

extern void server_refuse_connection(server_listener l,
				     network_handle h);
				/* Called by the network whenever it has
				 * temporarily accepted a connection just to
				 * explain to the user that the server is too
				 * full to accept their connection for real.
				 * The server may call network_send_line() and
				 * network_connection_name() to print the
				 * explanation, but H will no longer be valid
				 * after server_refuse_connection() returns.
				 */

extern void server_receive_line(server_handle h, const char *line);
				/* The given line has been received as input
				 * on the specified connection.  'line' does
				 * not end in a newline; any such bytes have
				 * been removed by the network.  The characters
				 * in 'line' are all either spaces or non-
				 * whitespace ASCII characters.
				 */

extern void server_close(server_handle h);
				/* The specified connection has been broken
				 * for some reason not in the server's control.
				 * Effective immediately, the network will no
				 * longer use the given server_handle and the
				 * server should not use the corresponding
				 * network_handle.
				 */

/*
 * The following procedures should not be called by a network implementation;
 * they are exported from the server module to other parts of the program.
 */

extern void server_suspend_input(Objid connection);
				/* As soon as possible, the server module
				 * should temporarily stop enqueuing input
				 * tasks for the given connection.
				 */
extern void server_resume_input(Objid connection);
				/* The server module may resume enqueuing input
				 * tasks for the given connection.
				 */

extern void set_server_cmdline(const char *line);
				/* If possible, the server's command line, as
				 * shown in the output of the `ps' command, is
				 * changed to the given string.  NOTE: This is
				 * not possible on all systems, so this
				 * operation is effectively a no-op in some
				 * cases.
				 */

#include "structures.h"

extern int server_flag_option(const char *name);
				/* Return true iff both $server_options and
				 * $server_options.NAME exist and the latter
				 * has a true MOO value.
				 */

extern int server_int_option(const char *name, int defallt);
				/* If both $server_options and
				 * $server_options.NAME exist and the latter
				 * has a numeric value, then return that value.
				 * Otherwise, return DEFALLT.
				 */

extern const char *server_string_option(const char *name,
					const char *defallt);
				/* If either $server_options or
				 * $server_options.NAME does not exist, then
				 * return DEFALLT.  Otherwise, if the latter
				 * has a string value, then return that value;
				 * else, return 0.  NOTE that the returned
				 * string has not had its reference count
				 * changed; the caller should str_ref() the
				 * result if the reference is to be persistent.
				 */

extern int get_server_option(Objid oid, const char *name, Var * r);
				/* If OID.server_options or $server_options
				 * exists, and the first of these that exists
				 * has as value a valid object OPT, and
				 * OPT.NAME exists, then set *R to the value of
				 * OPT.NAME and return 1; else return 0.
				 */

enum Fork_Result {
    FORK_PARENT, FORK_CHILD, FORK_ERROR
};
extern enum Fork_Result fork_server(const char *subtask_name);

extern void player_connected(Objid old_id, Objid new_id,
			     int is_newly_created);
extern void notify(Objid player, const char *message);
extern void boot_player(Objid player);

extern void write_active_connections(void);
extern int read_active_connections(void);

#endif				/* Server_H */
