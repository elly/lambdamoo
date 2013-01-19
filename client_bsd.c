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

/* BSD/LOCAL MUD client */

#include <errno.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>

#include "config.h"
#include "options.h"

void
main(int argc, char **argv)
{
    const char *connect_file = DEFAULT_CONNECT_FILE;
    int s;
    struct sockaddr_un address;

    if (argc == 2)
	connect_file = argv[1];
    else if (argc != 1) {
	fprintf(stderr, "Usage: %s [server-connect-file]\n", argv[0]);
	exit(1);
    }
    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
	perror("socket");
	exit(1);
    }
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, connect_file);

    if (connect(s, (struct sockaddr *) &address,
		sizeof(address.sun_family) + strlen(connect_file)) < 0) {
	perror("connect");
	exit(1);
    }
    while (1) {
	fd_set input;
	char buffer[1024];

	FD_ZERO(&input);
	FD_SET(0, &input);
	FD_SET(s, &input);

	if (select(s + 1, (void *) &input, 0, 0, 0) < 0) {
	    if (errno != EINTR) {
		perror("select");
		exit(1);
	    }
	} else {
	    if (FD_ISSET(0, &input))
		write(s, buffer, read(0, buffer, sizeof(buffer)));
	    if (FD_ISSET(s, &input)) {
		int count = read(s, buffer, sizeof(buffer));

		if (count == 0)
		    break;
		write(1, buffer, count);
	    }
	}
    }

    exit(0);
}

char rcsid_client_bsd[] = "$Id: client_bsd.c,v 1.2 1997/03/03 04:18:23 nop Exp $";
