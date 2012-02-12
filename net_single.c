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

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "config.h"
#include "log.h"
#include "network.h"
#include "server.h"
#include "streams.h"
#include "structures.h"
#include "utils.h"

#ifdef USE_NETPROTO_SINGLE

static enum {
    STATE_OPEN, STATE_CLOSED
} state = STATE_CLOSED;
static int listening = 0;
static server_listener slistener;
static int binary = 0;

static const char *
single_protocol_name(void)
{
    return "single-user";
}

static const char *
single_usage_string(void)
{
    return "";
}

static int
single_initialize(int argc, char **argv, Var * desc)
{
    *desc = zero;
    if (argc != 0)
	return 0;
    else
	return 1;
}

static enum error
single_make_listener(server_listener sl, Var desc, network_listener * nl,
		      Var * canon, const char **name)
{
    if (listening)
	return E_PERM;

    listening = 1;
    slistener = sl;
    nl->ptr = 0;
    *canon = zero;
    *name = "standard input";
    return E_NONE;
}

static int
single_listen(network_listener nl)
{
    return 1;
}

static int
single_send_line(network_handle nh, const char *line, int flush_ok)
{
    printf("%s\n", line);
    fflush(stdout);

    return 1;
}

static int
single_send_bytes(network_handle nh, const char *buffer, int buflen,
		   int flush_ok)
{
    /* Cast to (void *) to discard `const' on some systems */
    fwrite((void *) buffer, sizeof(char), buflen, stdout);
    fflush(stdout);

    return 1;
}

static int
single_buffered_output_length(network_handle nh)
{
    return 0;
}

static const char *
single_connection_name(network_handle nh)
{
    return "standard input";
}

static void
single_set_connection_binary(network_handle nh, int do_binary)
{
    binary = do_binary;
}

static Var
single_connection_options(network_handle nh, Var list)
{
    return list;
}

static int
single_connection_option(network_handle nh, const char *option, Var * value)
{
    return 0;
}

static int
single_set_connection_option(network_handle nh, const char *option, Var value)
{
    return 0;
}

static void
single_close(network_handle nh)
{
    state = STATE_CLOSED;
}

static void
single_close_listener(network_listener nl)
{
    listening = 0;
}

static void
single_shutdown(void)
{
    int flags;

    if ((flags = fcntl(0, F_GETFL)) < 0
	|| fcntl(0, F_SETFL, flags & ~O_NONBLOCK) < 0)
	log_perror("Setting standard input blocking again");
}

static int input_suspended = 0;

static void
single_suspend_input(network_handle nh)
{
    input_suspended = 1;
}

static void
single_resume_input(network_handle nh)
{
    input_suspended = 0;
}

static int
single_process_io(int timeout)
{
    network_handle nh;
    static server_handle sh;
    static Stream *s = 0;
    char buffer[1024];
    int count;
    char *ptr, *end;
    int got_some = 0;

    if (s == 0) {
	int flags;

	s = new_stream(1000);

	if ((flags = fcntl(0, F_GETFL)) < 0
	    || fcntl(0, F_SETFL, flags | O_NONBLOCK) < 0) {
	    log_perror("Setting standard input non-blocking");
	    return 0;
	}
    }
    switch (state) {
    case STATE_CLOSED:
	if (listening) {
	    sh = server_new_connection(slistener, nh, 0);
	    state = STATE_OPEN;
	    got_some = 1;
	} else if (timeout != 0)
	    sleep(timeout);
	break;

    case STATE_OPEN:
	for (;;) {
	    while (!input_suspended
		   && (count = read(0, buffer, sizeof(buffer))) > 0) {
		got_some = 1;
		if (binary) {
		    stream_add_string(s, raw_bytes_to_binary(buffer, count));
		    server_receive_line(sh, reset_stream(s));
		} else
		    for (ptr = buffer, end = buffer + count;
			 ptr < end;
			 ptr++) {
			unsigned char c = *ptr;

			if (isgraph(c) || c == ' ' || c == '\t')
			    stream_add_char(s, c);
			else if (c == '\n')
			    server_receive_line(sh, reset_stream(s));
		    }
	    }

	    if (got_some || timeout == 0)
		goto done;

	    sleep(1);
	    timeout--;
	}
    }

  done:
    return got_some;
}

struct netproto netproto_single = {
	.name = "single",
	.init = single_initialize,
	.protocol_name = single_protocol_name,
	.usage_string = single_usage_string,
	.make_listener = single_make_listener,
	.listen = single_listen,
	.send_line = single_send_line,
	.send_bytes = single_send_bytes,
	.buffered_output_length = single_buffered_output_length,
	.connection_name = single_connection_name,
	.set_connection_binary = single_set_connection_binary,
	.connection_options = single_connection_options,
	.connection_option = single_connection_option,
	.set_connection_option = single_set_connection_option,
	.close = single_close,
	.close_listener = single_close_listener,
	.shutdown = single_shutdown,
	.suspend_input = single_suspend_input,
	.resume_input = single_resume_input,
	.process_io = single_process_io,
};

char rcsid_net_single[] = "$Id: net_single.c,v 1.2 1997/03/03 04:19:07 nop Exp $";

#endif /* USE_NET_SINGLE */

/* $Log: net_single.c,v $
/* Revision 1.2  1997/03/03 04:19:07  nop
/* GNU Indent normalization
/*
 * Revision 1.1.1.1  1997/03/03 03:45:02  nop
 * LambdaMOO 1.8.0p5
 *
 * Revision 2.4  1996/03/10  01:23:12  pavel
 * Added support for `connection_option()'.  Added missing include of
 * "utils.h".  Release 1.8.0.
 *
 * Revision 2.3  1996/02/08  06:38:35  pavel
 * Added network_set_connection_binary() and network_connection_options().
 * Updated copyright notice for 1996.  Release 1.8.0beta1.
 *
 * Revision 2.2  1996/01/11  07:44:12  pavel
 * Added support for binary I/O.  Added network_buffered_output_length().
 * Removed one more `unsigned' from the server.  Release 1.8.0alpha5.
 *
 * Revision 2.1  1995/12/30  23:56:29  pavel
 * Added support for the new multiple-listening-points interface.
 * Release 1.8.0alpha4.
 *
 * Revision 2.0  1995/11/30  04:45:25  pavel
 * New baseline version, corresponding to release 1.8.0alpha1.
 *
 * Revision 1.8  1992/10/23  23:03:47  pavel
 * Added copyright notice.
 *
 * Revision 1.7  1992/10/21  03:02:35  pavel
 * Converted to use new automatic configuration system.
 *
 * Revision 1.6  1992/10/17  20:45:28  pavel
 * Changed to use generic support for POSIX-style non-blocking in "my-fcntl.h"
 * instead of a separate, different version.
 *
 * Revision 1.5  1992/10/06  18:20:11  pavel
 * Moved code to set stdin nonblocking out of the initialization routine and
 * into the first time that network_process_io() is called.
 * Also fixed up that routine to obviously return a value, circumventing a
 * warning from dumb compilers.
 *
 * Revision 1.4  1992/09/26  02:21:07  pavel
 * Added support for printing the network protocol name on server start-up.
 *
 * Revision 1.3  1992/09/11  21:23:52  pavel
 * Tracked change to network.h.
 *
 * Revision 1.2  1992/09/03  23:52:00  pavel
 * Added RCS Id and Log strings.
 */
