# Generated automatically from Makefile.in by configure.
# Copyright (c) 1992, 1995, 1996 Xerox Corporation.  All rights reserved.
# Portions of this code were written by Stephen White, aka ghond.
# Use and copying of this software and preparation of derivative works based
# upon this software are permitted.  Any distribution of this software or
# derivative works must comply with all applicable United States export
# control laws.  This software is made available AS IS, and Xerox Corporation
# makes no warranty about the software, its performance or its conformity to
# any specification.  Any person obtaining a copy of this software is requested
# to send their name and post office or electronic mail address to:
#   Pavel Curtis
#   Xerox PARC
#   3333 Coyote Hill Rd.
#   Palo Alto, CA 94304
#   Pavel@Xerox.Com

CC = gcc -g 
LIBRARIES =  -lm -lcrypt
YACC = bison -y

CFLAGS = -O -I. -Wall -Wextra

YFLAGS = -d
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c

CSRCS = ast.c code_gen.c db_file.c db_io.c db_objects.c db_properties.c \
	db_verbs.c decompile.c disassemble.c eval_env.c eval_vm.c \
	exceptions.c execute.c extensions.c functions.c keywords.c list.c \
	log.c match.c md5.c name_lookup.c network.c \
	numbers.c objects.c parse_cmd.c pattern.c program.c \
	property.c quota.c ref_count.c regexpr.c storage.c streams.c str_intern.c \
	sym_table.c tasks.c timers.c unparse.c utils.c verbs.c version.c \
	lmdb.c \
	net_single.c net_multi.c net_mp_poll.c net_bsd_tcp.c

OPT_NET_OBJS = $(OPT_NET_SRCS:.c=.o)

OPT_CSRCS = $(OPT_NET_SRCS)

YSRCS = parser.y

HDRS =  ast.h bf_register.h code_gen.h db.h db_io.h db_private.h decompile.h \
	disassemble.h eval_env.h eval_vm.h exceptions.h execute.h functions.h \
	getpagesize.h keywords.h list.h log.h match.h md5.h name_lookup.h \
	network.h net_mplex.h net_multi.h net_proto.h numbers.h opcode.h \
	options.h parse_cmd.h parser.h pattern.h program.h quota.h random.h \
	ref_count.h regexpr.h server.h storage.h streams.h structures.h  str_intern.h \
	sym_table.h tasks.h timers.h tokens.h unparse.h utils.h verbs.h \
	version.h

SYSHDRS = my-math.h \
	my-stdio.h \
	my-stdlib.h my-string.h

ALL_CSRCS = $(CSRCS) $(OPT_CSRCS)

SRCS = $(ALL_CSRCS) keywords.gperf $(YSRCS) $(HDRS) $(SYSHDRS)

DISTFILES = $(SRCS) Makefile.in restart restart.sh \
	Minimal.db README.Minimal \
	ChangeLog.txt README configure.in configure config.h.in \
	MOOCodeSequences.txt AddingNewMOOTypes.txt pgperf aclocal.m4

COBJS = $(CSRCS:.c=.o)

YOBJS = $(YSRCS:.y=.o)

OBJS = $(COBJS) $(YOBJS)

all: moo tools/lmdis

moo:	lambdamoo.o server.c
	$(CC) $(CFLAGS) $^ $(LIBRARIES) -o $@ -DSERVER_MAIN

lambdamoo.o : $(OBJS)
	$(LD) -r $(LDFLAGS) $(OBJS) -o $@

pure_moo: moo
	purify $(CC) $(CFLAGS) $(OBJS) $(LIBRARIES) -o $@

tools/lmdis: tools/lmdis.c lambdamoo.o server.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRARIES)

# This rule gets around some "make"s' desire to `derive' it from `restart.sh'.
restart:
	touch restart

y.tab.h: parser.o
	touch y.tab.h

keywords.c: keywords.gperf
	pgperf -aCIptT -k1,3,$$ keywords.gperf \
		> keywords.c

dist.tar.Z:	$(DISTFILES)
	rm -f version_number
	grep "server_version = " version.c \
		| sed -e 's/^.* = "//' -e 's/";//' > version_number
	rm -fr MOO-`cat version_number`
	mkdir MOO-`cat version_number`
	chmod 755 MOO-`cat version_number`
	cp -pr $(DISTFILES) MOO-`cat version_number`
	chmod u+w MOO-`cat version_number`/*
	tar cvf - MOO-`cat version_number` | compress -c > dist.tar.Z.NEW
	mv dist.tar.Z.NEW dist.tar.Z
	rm -f LambdaMOO-`cat version_number`.tar.Z
	ln dist.tar.Z LambdaMOO-`cat version_number`.tar.Z
	rm -fr MOO-`cat version_number` version_number

tags:
	etags -t $(SRCS)

clean:
	rm -f $(OBJS) core y.tab.c y.tab.h y.output makedep eddep 

distclean:	clean
	rm -f config.h Makefile config.status

depend: ${ALL_CSRCS}
	rm -f eddep makedep
	gcc -MM ${CFLAGS} ${ALL_CSRCS} | sed -e '/:$$/d' > depend.mk

###############################################################################
# $Log: Makefile.in,v $
# Revision 1.7  2000/01/11 01:54:05  nop
# CFLAGS line for poor users without inline keyword.
#
# Revision 1.6  2000/01/11 01:39:14  nop
# Long overdue make depend
#
# Revision 1.5  1998/02/19 07:36:16  nop
# Initial string interning during db load.
#
# Revision 1.4  1997/03/03 06:36:23  nop
# clean target doesn't force reconfiguration; distclean now does.
#
# Revision 1.3  1997/03/03 05:55:14  nop
# Changed to ed for make depend, since ex is a cheap plastic imitation of ed
# on many Linux systems.  Added better optimization suggestion.
#
# Revision 1.2  1997/03/03 04:40:46  nop
# Avoid running autoconf automatically
#
# Revision 1.1.1.1  1997/03/03 03:45:05  nop
# LambdaMOO 1.8.0p5
#
# Revision 2.10  1996/04/19  01:28:40  pavel
# Ran a new `make depend'.  Release 1.8.0p4.
#
# Revision 2.9  1996/03/10  01:31:24  pavel
# Added pgperf directory and aclocal.m4 file to the distribution.
# Release 1.8.0.
#
# Revision 2.8  1996/02/18  23:18:02  pavel
# Added md5.[ch].  Release 1.8.0beta3.
#
# Revision 2.7  1996/02/08  07:26:19  pavel
# Updated copyright notice for 1996.  Release 1.8.0beta1 (again).
#
# Revision 2.6  1996/02/08  05:46:27  pavel
# Added numbers.h, AddingNewMOOTypes.txt.  Updated dependencies.  Made pure_moo
# depend on moo.  Release 1.8.0beta1.
#
# Revision 2.5  1996/01/16  07:35:55  pavel
# Fixed dependencies.  Release 1.8.0alpha6.
#
# Revision 2.4  1996/01/11  07:58:54  pavel
# Fixed dependencies for parser.o.  Release 1.8.0alpha5.
#
# Revision 2.3  1995/12/31  03:29:29  pavel
# Fixed dependencies for parser.y and the networking modules.
# Release 1.8.0alpha4.
#
# Revision 2.2  1995/12/28  00:32:32  pavel
# Added verbs.h.  Release 1.8.0alpha3.
#
# Revision 2.1  1995/12/11  08:07:47  pavel
# Added a few more derived files to `make clean'.  Release 1.8.0alpha2.
#
# Revision 2.0  1995/11/30  05:45:03  pavel
# New baseline version, corresponding to release 1.8.0alpha1.
#
# Revision 1.26  1992/10/23  23:10:29  pavel
# Updated dependency information for release 1.7.0 and added copyright notice.
#
# Revision 1.25  1992/10/23  19:12:08  pavel
# A few last portability changes for release 1.7.0.
#
# Revision 1.24  1992/10/21  03:02:35  pavel
# Converted to use new automatic configuration system.
#
# Revision 1.23  1992/10/17  20:14:17  pavel
# Numerous changes in response to beta-testing portability problems.
#
# Revision 1.22  1992/10/06  01:32:41  pavel
# Dyked out useless XNS network support.
# Added sources for clients and a few new my-*.h files.
# Fixed nasty bug in `make depend' code.
#
# Revision 1.21  1992/09/23  17:23:08  pavel
# Added support for the whole family of networking protocol and multiplexing
# wait implementations; it should now be easy to add new `optional' sources,
# only used if certain combinations of preprocessor flags are set.
#
# Revision 1.20  1992/09/14  18:42:47  pjames
# Updated.
#
# Revision 1.19  1992/09/14  17:46:39  pjames
# Updated.
#
# Revision 1.18  1992/09/08  22:49:59  pjames
# Took out bf_move.c
#
# Revision 1.17  1992/09/08  22:09:23  pjames
# Updated for new bf_ filenames
#
# Revision 1.16  1992/09/03  23:49:13  pavel
# Added support for multiple complete network implementations.
#
# Revision 1.15  1992/09/03  16:32:40  pjames
# Updated.
#
# Revision 1.14  1992/08/31  23:43:57  pavel
# Added math library to the LIBRARIES line for A/UX.
#
# Revision 1.13  1992/08/28  16:32:18  pjames
# Added ref_count modules.
#
# Revision 1.12  1992/08/21  01:09:56  pavel
# Minor fix.
#
# Revision 1.11  1992/08/21  00:56:39  pavel
# Shortened the names of several files to fit within the 14-character limit
# imposed on certain systems.
# Moved the specification of all configuration options other than those
# directly related to the compiler command line into config.h.  Also
# reorganized the ones that are left to make it easier for folks to see what's
# necessary.
#
# Revision 1.10  1992/08/14  00:35:21  pavel
# Fixed rule for dist.tar.Z to be idempotent.
#
# Revision 1.9  1992/08/13  23:35:42  pavel
# Cleaned up CFLAGS organization.
#
# Revision 1.8  1992/08/13  23:24:34  pavel
# Did a more recent `make depend'...
#
# Revision 1.7  1992/08/13  22:48:20  pjames
# Added my-math.h
#
# Revision 1.6  1992/08/13  21:29:58  pjames
# Added -DMOO to COMPILE.c
#
# Revision 1.5  1992/08/11  17:06:45  pjames
# Updated with new source files, removed old source files, fixed dependencies.
#
# Revision 1.3  1992/07/27  18:30:21  pjames
# Update what vector.o and vector.po depend on.
###############################################################################

include depend.mk
	
# Have to do this one manually, since 'make depend' can't hack yacc files.
parser.o:	my-math.h my-stdlib.h my-string.h \
		ast.h code_gen.h config.h functions.h \
		keywords.h list.h log.h numbers.h opcode.h parser.h program.h \
		storage.h streams.h structures.h sym_table.h utils.h version.h

# Must do these specially, since they depend upon C preprocessor options.
network.o: 	net_single.o net_multi.o

$(OPT_NET_OBJS):
	touch $@
