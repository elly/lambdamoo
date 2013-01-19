/* config.h.  Generated automatically by configure.  */

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

/* If you're looking at this as `config.h.in', then this file is full of
 * `#undef' preprocessor directives.  The `configure' command, which generates
 * `config.h' from `config.h.in', replaces appropriate ones of those `#undef's
 * with `#define's, depending upon characteristics of the operating system,
 * and comments out all of the others.
 */

#ifndef Config_H
#define Config_H 1

/* To implement the LambdaMOO built-in function `ctime()', which converts a
 * number of seconds since the POSIX `epoch' into a human-readable time/date
 * string, the server code needs to be able to find out the name of the local
 * time zone.  This information is available in an obscure way from the ANSI
 * standard function `strftime()', but many systems still don't implement it.
 * The server has code to cope with this case, though, if your system maintains
 * the time-zone name in one of two common places.  Appropriate ones of the
 * following symbols will be defined to tell the server how to get time-zone
 * information on your system.
 */

#define HAVE_STRFTIME 1
#define HAVE_TM_ZONE 1
/* #undef HAVE_TZNAME */

/* Certain functions used by the server are `optional', in the sense that the
 * server can provide its own definition if necessary.  In some cases, there
 * are a number of common ways to do the same thing, differing by system type
 * (e.g., BSD, System V, or POSIX).  The following symbols are defined if your
 * system provides the named functions.
 */

#define HAVE_MATHERR 1
#define HAVE_SELECT 1
#define HAVE_POLL 1
#define HAVE_WAITPID 1
/* #undef HAVE_WAIT2 */
#define HAVE_WAIT3 1
#define HAVE_SIGEMPTYSET 1
#define HAVE_SIGSETMASK 1
#define HAVE_SIGRELSE 1

/* The NS_SYSV/NP_LOCAL networking configuration of the server makes use of an
 * operating system feature called `FIFO's for communication with clients of
 * the MOO.  In order to do this, the server needs some way to tell whether or
 * not there's something ready to be read on a given FIFO at any given time.
 * The BSD function `select()' usually works if it's available, but the System
 * V equivalent, `poll()', sometimes does and sometimes doesn't, depending on
 * the system.  The POSIX definers, of course, took the coward's way out and
 * didn't define any such function at all.  On many systems, however, you can
 * use the `fstat()' function to determine the number of bytes of data in a
 * FIFO; this allows the use of a somewhat crocky but workable substitute for
 * either `poll()' or `select()'.  The following symbols are defined if the
 * given function works to perform the necessary operation on FIFOs on your
 * system.
 */

/* #undef FSTAT_WORKS_ON_FIFOS */
#define POLL_WORKS_ON_FIFOS 1
/* #undef SELECT_WORKS_ON_FIFOS */

/* The following code figures out how to express a 32-bit integer type on your
 * machine.
 */

#include <limits.h>
#include <stdint.h>

typedef int32_t int32;
typedef uint32_t unsigned32;

#endif /* !Config_H */
