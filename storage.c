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

#include <stdlib.h>

#include "config.h"
#include "exceptions.h"
#include "list.h"
#include "options.h"
#include "ref_count.h"
#include "storage.h"
#include "structures.h"
#include "utils.h"

static unsigned alloc_num[Sizeof_Memory_Type];

static inline int
refcount_overhead(Memory_Type type)
{
    /* These are the only allocation types that are addref()'d.
     * As long as we're living on the wild side, avoid getting the
     * refcount slot for allocations that won't need it.
     */
    switch (type) {
    case M_FLOAT:
	/* for systems with picky double alignment */
	return MAX(sizeof(int), sizeof(double));
    case M_STRING:
	return sizeof(int);
    case M_LIST:
	/* for systems with picky pointer alignment */
	return MAX(sizeof(int), sizeof(Var *));
    default:
	return 0;
    }
}

void *
mymalloc(unsigned size, Memory_Type type)
{
    char *memptr;
    char msg[100];
    int offs;

    if (size == 0)		/* For queasy systems */
	size = 1;

    offs = refcount_overhead(type);
    memptr = (char *) malloc(size + offs);
    if (!memptr) {
	sprintf(msg, "memory allocation (size %u) failed!", size);
	panic(msg);
    }
    alloc_num[type]++;

    if (offs) {
	memptr += offs;
	((int *) memptr)[-1] = 1;
    }
    return memptr;
}

const char *
str_ref(const char *s)
{
    addref(s);
    return s;
}

char *
str_dup(const char *s)
{
    char *r;

    if (s == 0 || *s == '\0') {
	static char *emptystring;

	if (!emptystring) {
	    emptystring = (char *) mymalloc(1, M_STRING);
	    *emptystring = '\0';
	}
	addref(emptystring);
	return emptystring;
    } else {
	r = (char *) mymalloc(strlen(s) + 1, M_STRING);
	strcpy(r, s);
    }
    return r;
}

void *
myrealloc(void *ptr, unsigned size, Memory_Type type)
{
    int offs = refcount_overhead(type);
    static char msg[100];

    ptr = realloc((char *) ptr - offs, size + offs);
    if (!ptr) {
	sprintf(msg, "memory re-allocation (size %u) failed!", size);
	panic(msg);
    }

    return (char *)ptr + offs;
}

void
myfree(void *ptr, Memory_Type type)
{
    alloc_num[type]--;
    free((char *) ptr - refcount_overhead(type));
}

Var
memory_usage(void)
{
    Var r;
    r = new_list(0);
    return r;
}

char rcsid_storage[] = "$Id: storage.c,v 1.5 1998/12/14 13:18:59 nop Exp $";
