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

/* Some systems require that certain preprocessor symbols be defined in order
 * for certain pieces of the C library to be available.  If one of the
 * following is defined, then your system is one such.
 */

/* #undef _ALL_SOURCE */
/* #undef _POSIX_SOURCE */
/* #undef _MINIX */
/* #undef _POSIX_1_SOURCE */
/* #undef _HPUX_SOURCE */

/* The following symbols describe whether or not certain functions are declared
 * in header files on your system and, if so, where.  If, on your system, the
 * functions `memcmp', `memcpy', etc. are not declared in <string.h>, but are
 * instead in <memory.h>, then NEED_MEMORY_H will be defined.  Similarly, if
 * `malloc', `free', etc. are not declared in <stdlib.h>, but are instead in
 * <malloc.h>, then NEED_MALLOC_H will be defined.  If your system has the file
 * </sys/bsdtypes.h>, then NEED_BSDTYPES_H will be defined.
 *
 * The various NDECL_* symbols are defined if the named function or variable is
 * not declared in its standard place, which is given in a comment after each
 * symbol.  In several of the cases below, the absence of a declaration for the
 * named function or variable is taken in the server code to imply the similar
 * absence of declarations for several other related functions or variables.
 */

#define NEED_MEMORY_H 1
/* #undef NEED_MALLOC_H */
/* #undef NEED_BSDTYPES_H */
#define NEED_SELECT_H 1

/* #undef NDECL_TOLOWER */		/* <ctype.h>      */
/* #undef NDECL_FCNTL */		/* <fcntl.h>      */
/* #undef NDECL_HTONL */		/* <netinet/in.h> */
/* #undef NDECL_IOCTL */		/* <sys/ioctl.h>  */
/* #undef NDECL_POLL */		/* <poll.h>	  */
/* #undef NDECL_KILL */		/* <signal.h>	  */
/* #undef NDECL_SIGEMPTYSET */	/* <signal.h>	  */
/* #undef NDECL_SIGPROCMASK */	/* <signal.h>	  */
#define NDECL_SIGRELSE 1		/* <signal.h>	  */
/* #undef NDECL_ACCEPT */		/* <sys/socket.h> */
/* #undef NDECL_BIND */		/* <sys/socket.h> */
/* #undef NDECL_SHUTDOWN */		/* <sys/socket.h> */
/* #undef NDECL_FSTAT */		/* <sys/stat.h>	  */
/* #undef NDECL_FCLOSE */		/* <stdio.h>	  */
/* #undef NDECL_PERROR */		/* <stdio.h>	  */
/* #undef NDECL_REMOVE */		/* <stdio.h>	  */
/* #undef NDECL_VFPRINTF */		/* <stdio.h>	  */
/* #undef NDECL_RANDOM */		/* <stdlib.h>	  */
/* #undef NDECL_SRANDOM */		/* <stdlib.h>	  */
/* #undef NDECL_STRTOD */		/* <stdlib.h>	  */
/* #undef NDECL_STRTOL */		/* <stdlib.h>	  */
/* #undef NDECL_STRTOUL */		/* <stdlib.h>	  */
/* #undef NDECL_BZERO */		/* <string.h>	  */
/* #undef NDECL_MEMCPY */		/* <string.h>	  */
/* #undef NDECL_MEMSET */		/* <string.h>	  */
/* #undef NDECL_STRERROR */		/* <string.h>	  */
/* #undef NDECL_GETITIMER */		/* <sys/time.h>	  */
/* #undef NDECL_SETITIMER */		/* <sys/time.h>	  */
/* #undef NDECL_SELECT */		/* <sys/time.h>	  */
/* #undef NDECL_STRFTIME */		/* <time.h>	  */
/* #undef NDECL_TIME */		/* <time.h>	  */
/* #undef NDECL_TZNAME */		/* <time.h>	  */
#define NDECL_T_OPEN 1		/* <tiuser.h>	  */
#define NDECL_T_ERRLIST 1		/* <tiuser.h>	  */
/* #undef NDECL_FORK */		/* <unistd.h>	  */
/* #undef NDECL_WAITPID */		/* <sys/wait.h>	  */

/* Some systems declare bzero() in <stdlib.h> instead of in <string.h>. */

/* #undef BZERO_IN_STDLIB_H */

/* Some header files are only present on certain kinds of UNIXes (e.g., BSD,
 * System V, or POSIX).  The following symbols are defined only if the named
 * header file exists on your system.
 */

/* #undef HAVE_MACHINE_ENDIAN_H */
#define HAVE_STDLIB_H 1
#define HAVE_SYS_CDEFS_H 1
#define HAVE_UNISTD_H 1

/* Some POSIX-standard typedefs are not present in some systems.  The following
 * symbols are defined as aliases for their usual definitions if they are not
 * defined on your system.
 */

/* #undef pid_t */
/* #undef mode_t */
/* #undef size_t */

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
/* #undef TM_IN_SYS_TIME */
#define HAVE_TM_ZONE 1
/* #undef HAVE_TZNAME */

/* Some compilers `almost' conform to the ANSI C standard, in that they provide
 * the vast majority of the features and constructs used by the server code,
 * but one common failing with which the code can cope is the lack of support
 * for the `const' keyword.  If your compiler doesn't support `const', then
 * this symbol will be defined as a macro that expands into nothing, thus
 * eliding all uses of the keyword in the code.
 */

/* #undef const */

/* Some sites have installed GCC improperly or incompletely, thereby requiring
 * the server to be compiled with the `-traditional' switch.  That disables the
 * `const', `volatile' or `signed' keywords, which we need.  Thus, for GCC, we
 * do these little substitutions to always refer to its `hidden' names for
 * these keywords.
 */

#if defined(__GNUC__) && !HAVE_SYS_CDEFS_H
#  define const __const__
#  define volatile __volatile__
#  define signed __signed__
#endif

/* Certain functions used by the server are `optional', in the sense that the
 * server can provide its own definition if necessary.  In some cases, there
 * are a number of common ways to do the same thing, differing by system type
 * (e.g., BSD, System V, or POSIX).  The following symbols are defined if your
 * system provides the named functions.
 */

#define HAVE_CRYPT 1
#define HAVE_MATHERR 1
#define HAVE_MKFIFO 1
#define HAVE_REMOVE 1
#define HAVE_RENAME 1
#define HAVE_SELECT 1
#define HAVE_POLL 1
#define HAVE_STRERROR 1
#define HAVE_STRTOUL 1
#define HAVE_RANDOM 1
#define HAVE_LRAND48 1
#define HAVE_WAITPID 1
/* #undef HAVE_WAIT2 */
#define HAVE_WAIT3 1
#define HAVE_SIGEMPTYSET 1
#define HAVE_SIGPROCMASK 1
#define HAVE_SIGSETMASK 1
#define HAVE_SIGRELSE 1

/* It used to be very much the fashion in UNIX programming to make use of
 * certain standard header files depend on the programmer having #include'd
 * other standard header files first.  This is obviously losing and, amazingly,
 * is also losing favor, replaced by the notion that every header file should
 * #include whatever other headers it requires, and that all headers should
 * take steps to ensure that they are not #include'd more than once if that
 * would be bad.  The following symbols are defined if the named header files
 * fail to adhere to the new style; the server code makes up for them by doing
 * its own #include of the usual prerequisite.
 */

/* #undef ARPA_INET_H_NEEDS_HELP */
/* #undef SIGNAL_H_NEEDS_HELP */
/* #undef SYS_SOCKET_H_NEEDS_HELP */
/* #undef SYS_STAT_H_NEEDS_HELP */
/* #undef TIME_H_NEEDS_HELP */

/* Version 2 of the GNU C compiler includes `built-in' declarations for a
 * number of standard functions from the ANSI header file <string.h>.
 * Unfortunately, some systems declare these functions differently (and
 * non-standardly) in their native header files, generating a warning about the
 * conflict from GCC.  If this is the case on your system, the following symbol
 * will be defined, indicating that the server code should avoid your system's
 * <string.h> file, using its own version instead.
 */

/* #undef USE_OWN_STRING_H */

/* On some systems, notably AIX, both <sys/ioctl.h> and <stropts.h> define the
 * internal macro `_IO' in compatible but different ways.  This means that no
 * program can #include both files without getting an error.  If this is so on
 * your machine, then the following symbol will be defined and the server code
 * will carefully #undef _IO between inclusions of the two files.
 */

/* #undef UNDEF_IO_IN_STROPTS_H */

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

/* The POSIX standard specifies a nice, simple, reliable method for performing
 * non-blocking I/O, and a lot of systems actually implement it.  Some systems,
 * though (notably Ultrix 4.1), misleadingly define the preprocessor symbols
 * that might make you *think* that they've implemented it, but really they
 * don't.  This symbol is defined if POSIX-style non-blocking actually works on
 * your system.
 */

#define POSIX_NONBLOCKING_WORKS 1

/* The following code figures out how to express a 32-bit integer type on your
 * machine.
 */

#include <limits.h>

#if INT_MAX == 2147483647
   typedef int		int32;
   typedef unsigned	unsigned32;
#  define INT32_MAX	INT_MAX
#else
#  if LONG_MAX == 2147483647
     typedef long int		int32;
     typedef unsigned long	unsigned32;
#    define INT32_MAX		LONG_MAX
#  else
#    error I cannot figure out how to express a 32-bit integer on your machine.
#  endif
#endif

#endif /* !Config_H */
