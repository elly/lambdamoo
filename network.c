/******************************************************************************
  Copyright (c) 1992 Xerox Corporation.  All rights reserved.
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

#include <stddef.h>

#include "network.h"

#ifdef USE_NETPROTO_SINGLE
extern struct netproto netproto_single;
#endif
#ifdef USE_NETPROTO_TCP
extern struct netproto netproto_tcp;
#endif

static struct netproto *netprotos[] = {
#ifdef USE_NETPROTO_SINGLE
	&netproto_single,
#endif
#ifdef USE_NETPROTO_TCP
	&netproto_tcp,
#endif
	NULL,
};

static struct netproto *netproto;

void network_useproto(const char *name) {
	int i;
	for (i = 0; netprotos[i]; i++)
		if (!strcmp(netprotos[i]->name, name))
			netproto = netprotos[i];
}

int network_initialize(int argc, char **argv, Var *desc) {
	return netproto->init(argc, argv, desc);
}

const char *network_protocol_name(void) {
	return netproto->protocol_name();
}

const char *network_usage_string(void) {
	return netproto->usage_string();
}

enum error network_make_listener(server_listener sl, Var desc,
                                 network_listener *nl, Var *canon,
                                 const char **name) {
	return netproto->make_listener(sl, desc, nl, canon, name);
}

int network_listen(network_listener nl) {
	return netproto->listen(nl);
}

int network_send_line(network_handle nh, const char *line, int flush_ok) {
	return netproto->send_line(nh, line, flush_ok);
}

int network_send_bytes(network_handle nh, const char *buffer, int buflen,
                       int flush_ok) {
	return netproto->send_bytes(nh, buffer, buflen, flush_ok);
}

int network_buffered_output_length(network_handle nh) {
	return netproto->buffered_output_length(nh);
}

void network_suspend_input(network_handle nh) {
	return netproto->suspend_input(nh);
}

void network_resume_input(network_handle nh) {
	return netproto->resume_input(nh);
}

void network_set_connection_binary(network_handle nh, int binary) {
	return netproto->set_connection_binary(nh, binary);
}

int network_process_io(int timeout) {
	return netproto->process_io(timeout);
}

const char *network_connection_name(network_handle nh) {
	return netproto->connection_name(nh);
}

Var network_connection_options(network_handle nh, Var list) {
	return netproto->connection_options(nh, list);
} 

int network_connection_option(network_handle nh, const char *option, Var *value) {
	return netproto->connection_option(nh, option, value);
}

int network_set_connection_option(network_handle nh, const char *option,
                                  Var value) {
	return netproto->set_connection_option(nh, option, value);
}

void network_close(network_handle nh) {
	return netproto->close(nh);
}

void network_close_listener(network_listener nl) {
	return netproto->close_listener(nl);
}

void network_shutdown(void) {
	return netproto->shutdown();
}

/* 
 * $Log: network.c,v $
 * Revision 1.2  1998/12/14 13:18:35  nop
 * Merge UNSAFE_OPTS (ref fixups); fix Log tag placement to fit CVS whims
 *
 * Revision 1.1.1.1  1997/03/03 03:45:00  nop
 * LambdaMOO 1.8.0p5
 *
 * Revision 2.0  1995/11/30  05:11:37  pavel
 * New baseline version, corresponding to release 1.8.0alpha1.
 *
 * Revision 1.5  1992/10/23  23:03:47  pavel
 * Added copyright notice.
 *
 * Revision 1.4  1992/10/21  03:02:35  pavel
 * Converted to use new automatic configuration system.
 *
 * Revision 1.3  1992/09/26  02:29:24  pavel
 * Dyked out useless support plans for XNS protocols.
 *
 * Revision 1.2  1992/09/23  17:18:44  pavel
 * Now supports the new networking architecture, switching among protocol and
 * multiplexing-wait implementations based on settings in config.h.
 *
 * Revision 1.1  1992/09/03  21:09:51  pavel
 * Initial RCS-controlled version.
 */
