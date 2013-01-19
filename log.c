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

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "bf_register.h"
#include "config.h"
#include "functions.h"
#include "log.h"
#include "options.h"
#include "storage.h"
#include "streams.h"
#include "utils.h"

static FILE *log_file = 0;

void
set_log_file(FILE * f)
{
    log_file = f;
}

static void
do_log(const char *fmt, va_list args, const char *prefix)
{
    FILE *f;

    if (log_file) {
	time_t now = time(0);
	char *nowstr = ctime(&now);

	nowstr[19] = '\0';	/* kill the year and newline at the end */
	f = log_file;
	fprintf(f, "%s: %s", nowstr + 4, prefix);	/* skip the day of week */
    } else
	f = stderr;

    vfprintf(f, fmt, args);
    fflush(f);
}

void
oklog(const char *fmt,...)
{
    va_list args;

    va_start(args, fmt);
    do_log(fmt, args, "");
    va_end(args);
}

void
errlog(const char *fmt,...)
{
    va_list args;

    va_start(args, fmt);
    do_log(fmt, args, "*** ");
    va_end(args);
}

void
log_perror(const char *what)
{
    errlog("%s: %s\n", what, strerror(errno));
}


#ifdef LOG_COMMANDS
static Stream *command_history = 0;
#endif

void
reset_command_history()
{
#ifdef LOG_COMMANDS
    if (command_history == 0)
	command_history = new_stream(1024);
    else
	reset_stream(command_history);
#endif
}

void
log_command_history()
{
#ifdef LOG_COMMANDS
    errlog("COMMAND HISTORY:\n%s", stream_contents(command_history));
#endif
}

void
add_command_to_history(Objid player, const char *command)
{
#ifdef LOG_COMMANDS
    time_t now = time(0);
    char *nowstr = ctime(&now);

    nowstr[19] = '\0';		/* kill the year and newline at the end */
    stream_printf(command_history, "%s: #%d: %s\n",
		  nowstr + 4,	/* skip day of week */
		  player, command);
#endif				/* LOG_COMMANDS */
}

/**** built in functions ****/

static package
bf_server_log(Var arglist, Byte next, void *vdata, Objid progr)
{
    if (!is_wizard(progr)) {
	free_var(arglist);
	return make_error_pack(E_PERM);
    } else {
	int is_error = (arglist.v.list[0].v.num == 2
			&& is_true(arglist.v.list[2]));

	if (is_error)
	    errlog("> %s\n", arglist.v.list[1].v.str);
	else
	    oklog("> %s\n", arglist.v.list[1].v.str);

	free_var(arglist);
	return no_var_pack();
    }
}

void
register_log(void)
{
    register_function("server_log", 1, 2, bf_server_log, TYPE_STR, TYPE_ANY);
}

char rcsid_log[] = "$Id: log.c,v 1.3 1998/12/14 13:17:59 nop Exp $";
