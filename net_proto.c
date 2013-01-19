/******************************************************************************
  Copyright (c) 1994, 1995, 1996 Xerox Corporation.  All rights reserved.
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

#include "options.h"

#  if NETWORK_PROTOCOL == NP_TCP
#    if NETWORK_STYLE == NS_BSD
#      include "net_bsd_tcp.c"
#    endif
#  endif

#  if NETWORK_PROTOCOL == NP_LOCAL
#    if NETWORK_STYLE == NS_BSD
#      include "net_bsd_lcl.c"
#    endif
#  endif

char rcsid_net_proto[] = "$Id: net_proto.c,v 1.2 1998/12/14 13:18:33 nop Exp $";
