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

#ifndef My_Stdlib_H
#define My_Stdlib_H 1

#include "config.h"

#include <stdlib.h>

#endif

/* 
 * $Log: my-stdlib.h,v $
 * Revision 1.3  1998/12/14 13:18:18  nop
 * Merge UNSAFE_OPTS (ref fixups); fix Log tag placement to fit CVS whims
 *
 * Revision 1.2  1997/03/03 04:18:56  nop
 * GNU Indent normalization
 *
 * Revision 1.1.1.1  1997/03/03 03:45:05  nop
 * LambdaMOO 1.8.0p5
 *
 * Revision 2.1  1996/02/08  06:01:42  pavel
 * Updated copyright notice for 1996.  Added strtod().  Release 1.8.0beta1.
 *
 * Revision 2.0  1995/11/30  04:58:54  pavel
 * New baseline version, corresponding to release 1.8.0alpha1.
 *
 * Revision 1.10  1993/08/04  01:16:15  pavel
 * Added default definition for strtoul(), as a synonym for strtol().
 *
 * Revision 1.9  1993/08/04  00:27:53  pavel
 * Added declaration for strtoul().
 *
 * Revision 1.8  1993/08/04  00:13:30  pavel
 * Added declarations of random() and srandom(), moved from random.h.
 *
 * Revision 1.7  1992/10/23  23:03:47  pavel
 * Added copyright notice.
 *
 * Revision 1.6  1992/10/23  19:31:29  pavel
 * Added protection against multiple inclusions, for systems that aren't
 * clever enough to do that for themselves.
 * Added declaration of realloc().
 * Added support for NEED_MALLOC_H.
 *
 * Revision 1.5  1992/10/21  03:02:35  pavel
 * Converted to use new automatic configuration system.
 *
 * Revision 1.4  1992/10/17  20:39:19  pavel
 * Removed apparently-useless declaration of exit()... (*sigh*)
 *
 * Revision 1.3  1992/09/27  19:25:12  pavel
 * Added declaration of exit() for SunOS.
 *
 * Revision 1.2  1992/07/30  00:33:28  pavel
 * Add support for compiling on RISC/os 4.52 and NonStop-UX A22.
 *
 * Revision 1.1  1992/07/20  23:23:12  pavel
 * Initial RCS-controlled version.
 */
