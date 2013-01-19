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

/* Multi-user networking protocol implementation for TCP/IP on BSD UNIX */

#include <arpa/inet.h>		/* inet_addr() */
#include <errno.h>		/* EMFILE, EADDRNOTAVAIL, ECONNREFUSED,
				   * ENETUNREACH, ETIMEOUT */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>		/* socket(), AF_INET, SOCK_STREAM,
				   * setsockopt(), SOL_SOCKET, SO_REUSEADDR,
				   * bind(), struct sockaddr, accept(),
				   * connect() */
#include <stdlib.h>		/* strtoul() */
#include <string.h>		/* memcpy() */
#include <unistd.h>		/* close() */

#include "config.h"
#include "list.h"
#include "log.h"
#include "name_lookup.h"
#include "net_proto.h"
#include "options.h"
#include "server.h"
#include "streams.h"
#include "timers.h"
#include "utils.h"

const char *
proto_name(void)
{
    return "BSD/TCP";
}

const char *
proto_usage_string(void)
{
    return "[port]";
}

int
proto_initialize(struct proto *proto, Var * desc, int argc, char **argv)
{
    int port = DEFAULT_PORT;
    char *p;

    initialize_name_lookup();

    proto->pocket_size = 1;
    proto->believe_eof = 1;
    proto->eol_out_string = "\r\n";

    if (argc > 1)
	return 0;
    else if (argc == 1) {
	port = strtoul(argv[0], &p, 10);
	if (*p != '\0')
	    return 0;
    }
    desc->type = TYPE_INT;
    desc->v.num = port;
    return 1;
}

enum error
proto_make_listener(Var desc, int *fd, Var * canon, const char **name)
{
    struct sockaddr_in address;
    int s, port, option = 1;
    static Stream *st = 0;

    if (!st)
	st = new_stream(20);

    if (desc.type != TYPE_INT)
	return E_TYPE;

    port = desc.v.num;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
	log_perror("Creating listening socket");
	return E_QUOTA;
    }
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
		   (char *) &option, sizeof(option)) < 0) {
	log_perror("Setting listening socket options");
	close(s);
	return E_QUOTA;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);
    if (bind(s, (struct sockaddr *) &address, sizeof(address)) < 0) {
	enum error e = E_QUOTA;

	log_perror("Binding listening socket");
	if (errno == EACCES)
	    e = E_PERM;
	close(s);
	return e;
    }
    if (port == 0) {
	int length = sizeof(address);

	if (getsockname(s, (struct sockaddr *) &address, &length) < 0) {
	    log_perror("Discovering local port number");
	    close(s);
	    return E_QUOTA;
	}
	canon->type = TYPE_INT;
	canon->v.num = ntohs(address.sin_port);
    } else
	*canon = var_ref(desc);

    stream_printf(st, "port %d", canon->v.num);
    *name = reset_stream(st);

    *fd = s;
    return E_NONE;
}

int
proto_listen(int fd)
{
    listen(fd, 5);
    return 1;
}

enum proto_accept_error
proto_accept_connection(int listener_fd, int *read_fd, int *write_fd,
			const char **name)
{
    int timeout = server_int_option("name_lookup_timeout", 5);
    int fd;
    struct sockaddr_in address;
    int addr_length = sizeof(address);
    static Stream *s = 0;

    if (!s)
	s = new_stream(100);

    fd = accept(listener_fd, (struct sockaddr *) &address, &addr_length);
    if (fd < 0) {
	if (errno == EMFILE)
	    return PA_FULL;
	else {
	    log_perror("Accepting new network connection");
	    return PA_OTHER;
	}
    }
    *read_fd = *write_fd = fd;
    stream_printf(s, "%s, port %d",
		  lookup_name_from_addr(&address, timeout),
		  (int) ntohs(address.sin_port));
    *name = reset_stream(s);
    return PA_OKAY;
}

void
proto_close_connection(int read_fd, int write_fd)
{
    /* read_fd and write_fd are the same, so we only need to deal with one. */
    close(read_fd);
}

void
proto_close_listener(int fd)
{
    close(fd);
}

#ifdef OUTBOUND_NETWORK

#include "exceptions.h"
#include "structures.h"

static Exception timeout_exception;

static void
timeout_proc(Timer_ID id, Timer_Data data)
{
    RAISE(timeout_exception, 0);
}

enum error
proto_open_connection(Var arglist, int *read_fd, int *write_fd,
		      const char **local_name, const char **remote_name)
{
    /* These are `static' rather than `volatile' because I can't cope with
     * getting all those nasty little parameter-passing rules right.  This
     * function isn't recursive anyway, so it doesn't matter.
     */
    static const char *host_name;
    static int port;
    static Timer_ID id;
    int s, result, length;
    int timeout = server_int_option("name_lookup_timeout", 5);
    static struct sockaddr_in addr;
    static Stream *st1 = 0, *st2 = 0;

    if (!st1) {
	st1 = new_stream(20);
	st2 = new_stream(50);
    }
    if (arglist.v.list[0].v.num != 2)
	return E_ARGS;
    else if (arglist.v.list[1].type != TYPE_STR ||
	     arglist.v.list[2].type != TYPE_INT)
	return E_TYPE;

    host_name = arglist.v.list[1].v.str;
    port = arglist.v.list[2].v.num;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = lookup_addr_from_name(host_name, timeout);
    if (addr.sin_addr.s_addr == 0)
	return E_INVARG;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
	if (errno != EMFILE)
	    log_perror("Making socket in proto_open_connection");
	return E_QUOTA;
    }
    TRY
	id = set_timer(server_int_option("outbound_connect_timeout", 5),
		       timeout_proc, 0);
    result = connect(s, (struct sockaddr *) &addr, sizeof(addr));
    cancel_timer(id);
    EXCEPT(timeout_exception)
	result = -1;
    errno = ETIMEDOUT;
    reenable_timers();
    ENDTRY

	if (result < 0) {
	close(s);
	if (errno == EADDRNOTAVAIL ||
	    errno == ECONNREFUSED ||
	    errno == ENETUNREACH ||
	    errno == ETIMEDOUT)
	    return E_INVARG;
	log_perror("Connecting in proto_open_connection");
	return E_QUOTA;
    }
    length = sizeof(addr);
    if (getsockname(s, (struct sockaddr *) &addr, &length) < 0) {
	close(s);
	log_perror("Getting local name in proto_open_connection");
	return E_QUOTA;
    }
    *read_fd = *write_fd = s;

    stream_printf(st1, "port %d", (int) ntohs(addr.sin_port));
    *local_name = reset_stream(st1);

    stream_printf(st2, "%s, port %d", host_name, port);
    *remote_name = reset_stream(st2);

    return E_NONE;
}
#endif				/* OUTBOUND_NETWORK */

char rcsid_net_bsd_tcp[] = "$Id: net_bsd_tcp.c,v 1.3 1998/12/14 13:18:27 nop Exp $";
