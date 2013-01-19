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
YACC = byacc

CFLAGS = -O
# If you're using GCC, you may prefer:
# CFLAGS = -O2 -finline-functions
#
# If you're not using GCC, and your compiler doesn't understand the
# inline keyword, use the following.  (This will be fixed in autoconf2
# upgrade).
# CFLAGS = -O2 -Dinline=

YFLAGS = -d
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c

CSRCS = ast.c code_gen.c db_file.c db_io.c db_objects.c db_properties.c \
	db_verbs.c decompile.c disassemble.c eval_env.c eval_vm.c \
	exceptions.c execute.c extensions.c functions.c keywords.c list.c \
	log.c match.c md5.c name_lookup.c network.c net_mplex.c \
	net_proto.c numbers.c objects.c parse_cmd.c pattern.c program.c \
	property.c regexpr.c server.c storage.c streams.c str_intern.c \
	sym_table.c tasks.c timers.c unparse.c utils.c verbs.c version.c

OPT_NET_SRCS = net_single.c net_multi.c \
	net_mp_selct.c net_mp_poll.c \
	net_bsd_tcp.c net_bsd_lcl.c

OPT_NET_OBJS = $(OPT_NET_SRCS:.c=.o)

OPT_CSRCS = $(OPT_NET_SRCS)

YSRCS = parser.y

HDRS =  ast.h bf_register.h code_gen.h db.h db_io.h db_private.h decompile.h \
	disassemble.h eval_env.h eval_vm.h exceptions.h execute.h functions.h \
	getpagesize.h keywords.h list.h log.h match.h md5.h name_lookup.h \
	network.h net_mplex.h net_multi.h net_proto.h numbers.h opcode.h \
	options.h parse_cmd.h parser.h pattern.h program.h \
	ref_count.h regexpr.h server.h storage.h streams.h structures.h  str_intern.h \
	sym_table.h tasks.h timers.h tokens.h unparse.h utils.h verbs.h \
	version.h y.tab.h

SYSHDRS =   my-in.h  my-ioctl.h  \
	my-poll.h     \
	    \
	  

CLIENT_SRCS = client_bsd.c

ALL_CSRCS = $(CSRCS) $(OPT_CSRCS) $(CLIENT_SRCS)

SRCS = $(ALL_CSRCS) keywords.gperf $(YSRCS) $(HDRS) $(SYSHDRS)

DISTFILES = $(SRCS) Makefile.in restart restart.sh \
	Minimal.db README.Minimal \
	ChangeLog.txt README configure.in configure config.h.in \
	MOOCodeSequences.txt AddingNewMOOTypes.txt pgperf aclocal.m4

COBJS = $(CSRCS:.c=.o)

YOBJS = $(YSRCS:.y=.o)

OBJS = $(COBJS) $(YOBJS)

moo:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBRARIES) -o $@

pure_moo: moo
	purify $(CC) $(CFLAGS) $(OBJS) $(LIBRARIES) -o $@

client_bsd: client_bsd.o
	$(CC) $(CFLAGS) client_bsd.o $(LIBRARIES) -o $@

# This rule gets around some "make"s' desire to `derive' it from `restart.sh'.
restart:
	touch restart

y.tab.h: parser.o
	touch y.tab.h

keywords.c: keywords.gperf
	pgperf -aCIptT -k1,3,$$ keywords.gperf \
		| sed -e 's/#include <ctype.h>/#include ""/' \
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
	rm -f $(OBJS) core y.tab.c y.output makedep eddep 

depend: ${ALL_CSRCS}
	rm -f eddep makedep
	gcc -MM ${CFLAGS} ${ALL_CSRCS} | sed -e '/:$$/d' > makedep
	echo '/^# DO NOT DELETE THIS LINE/+1,$$d' >eddep
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	cp Makefile.in Makefile.in.bak
	ed - Makefile.in < eddep
	rm -f eddep makedep

# Have to do this one manually, since 'make depend' can't hack yacc files.
parser.o:	    \
		ast.h code_gen.h config.h functions.h \
		keywords.h list.h log.h numbers.h opcode.h parser.h program.h \
		storage.h streams.h structures.h sym_table.h utils.h version.h

# Must do these specially, since they depend upon C preprocessor options.
network.o: 	net_single.o net_multi.o
net_proto.o:	net_bsd_tcp.o net_bsd_lcl.o
net_mplex.o:	net_mp_selct.o net_mp_poll.o

$(OPT_NET_OBJS):
	touch $@

# DO NOT DELETE THIS LINE -- 'make depend' replaces everything below it.
ast.o: ast.c  config.h ast.h parser.h program.h structures.h \
  version.h sym_table.h list.h log.h storage.h ref_count.h \
 utils.h execute.h db.h opcode.h options.h parse_cmd.h
code_gen.o: code_gen.c ast.h config.h parser.h program.h structures.h \
  version.h sym_table.h exceptions.h opcode.h options.h \
 storage.h ref_count.h str_intern.h utils.h execute.h db.h parse_cmd.h \
 
db_file.o: db_file.c  config.h   db.h \
 program.h structures.h version.h db_io.h db_private.h exceptions.h \
 list.h log.h options.h server.h network.h storage.h ref_count.h \
 streams.h str_intern.h tasks.h execute.h opcode.h parse_cmd.h timers.h \
 
db_io.o: db_io.c  config.h    \
 db_io.h program.h structures.h version.h db_private.h exceptions.h \
 list.h log.h numbers.h parser.h storage.h ref_count.h streams.h \
 str_intern.h unparse.h
db_objects.o: db_objects.c config.h db.h program.h structures.h \
  version.h db_private.h exceptions.h list.h storage.h \
 ref_count.h utils.h execute.h opcode.h options.h parse_cmd.h
db_properties.o: db_properties.c config.h db.h program.h structures.h \
  version.h db_private.h exceptions.h list.h storage.h \
 ref_count.h utils.h execute.h opcode.h options.h parse_cmd.h
db_verbs.o: db_verbs.c  config.h  db.h program.h \
 structures.h  version.h db_private.h exceptions.h db_tune.h \
 list.h log.h parse_cmd.h storage.h ref_count.h utils.h execute.h \
 opcode.h options.h
decompile.o: decompile.c ast.h config.h parser.h program.h structures.h \
  version.h sym_table.h decompile.h exceptions.h opcode.h \
 options.h storage.h ref_count.h utils.h execute.h db.h parse_cmd.h
disassemble.o: disassemble.c  config.h bf_register.h db.h \
 program.h structures.h version.h functions.h execute.h opcode.h \
 options.h parse_cmd.h list.h storage.h ref_count.h streams.h unparse.h \
 utils.h verbs.h
eval_env.o: eval_env.c config.h eval_env.h structures.h  \
 version.h storage.h ref_count.h sym_table.h utils.h execute.h db.h \
 program.h opcode.h options.h parse_cmd.h
eval_vm.o: eval_vm.c config.h db_io.h program.h structures.h  \
 version.h decompile.h ast.h parser.h sym_table.h eval_vm.h execute.h \
 db.h opcode.h options.h parse_cmd.h log.h storage.h ref_count.h tasks.h
exceptions.o: exceptions.c exceptions.h config.h
execute.o: execute.c  config.h db.h program.h structures.h \
  version.h db_io.h decompile.h ast.h parser.h sym_table.h \
 eval_env.h eval_vm.h execute.h opcode.h options.h parse_cmd.h \
 exceptions.h functions.h list.h log.h numbers.h server.h network.h \
 storage.h ref_count.h streams.h tasks.h timers.h  utils.h
extensions.o: extensions.c bf_register.h functions.h  config.h \
 execute.h db.h program.h structures.h version.h opcode.h options.h \
 parse_cmd.h db_tune.h utils.h
functions.o: functions.c  config.h bf_register.h db_io.h \
 program.h structures.h  version.h functions.h execute.h db.h \
 opcode.h options.h parse_cmd.h list.h log.h server.h network.h storage.h \
 ref_count.h streams.h unparse.h utils.h
keywords.o: keywords.c  config.h  keywords.h \
 structures.h  version.h tokens.h ast.h parser.h program.h \
 sym_table.h y.tab.h utils.h execute.h db.h opcode.h options.h \
 parse_cmd.h
list.o: list.c  config.h  bf_register.h exceptions.h \
 functions.h  execute.h db.h program.h structures.h version.h \
 opcode.h options.h parse_cmd.h list.h log.h md5.h pattern.h  \
 ref_count.h streams.h storage.h unparse.h utils.h
log.o: log.c  config.h    \
 bf_register.h functions.h execute.h db.h program.h structures.h \
 version.h opcode.h options.h parse_cmd.h log.h storage.h ref_count.h \
 streams.h utils.h
match.o: match.c  config.h  db.h program.h \
 structures.h  version.h exceptions.h match.h parse_cmd.h \
 storage.h ref_count.h unparse.h utils.h execute.h opcode.h options.h
md5.o: md5.c  config.h md5.h
name_lookup.o: name_lookup.c options.h config.h  \
      log.h  \
 structures.h server.h network.h storage.h ref_count.h timers.h 
network.o: network.c options.h config.h net_multi.c   \
      exceptions.h \
 list.h structures.h log.h net_mplex.h net_multi.h net_proto.h network.h \
 server.h streams.h storage.h ref_count.h timers.h  utils.h \
 execute.h db.h program.h version.h opcode.h parse_cmd.h
net_mplex.o: net_mplex.c options.h config.h net_mp_selct.c  \
   log.h  structures.h net_mplex.h
net_proto.o: net_proto.c options.h config.h net_bsd_tcp.c  \
     list.h structures.h \
  log.h name_lookup.h net_proto.h server.h network.h streams.h \
 timers.h  utils.h execute.h db.h program.h version.h opcode.h \
 parse_cmd.h
numbers.o: numbers.c   config.h   \
 functions.h  execute.h db.h program.h structures.h version.h \
 opcode.h options.h parse_cmd.h log.h  storage.h ref_count.h \
 utils.h
objects.o: objects.c db.h config.h program.h structures.h  \
 version.h db_io.h exceptions.h execute.h opcode.h options.h parse_cmd.h \
 functions.h list.h numbers.h server.h network.h storage.h \
 ref_count.h utils.h
parse_cmd.o: parse_cmd.c  config.h   \
   db.h program.h structures.h version.h list.h \
 match.h parse_cmd.h storage.h ref_count.h utils.h execute.h opcode.h \
 options.h
pattern.o: pattern.c  config.h   \
 pattern.h regexpr.h storage.h structures.h  ref_count.h \
 streams.h
program.o: program.c ast.h config.h parser.h program.h structures.h \
  version.h sym_table.h exceptions.h list.h storage.h \
 ref_count.h utils.h execute.h db.h opcode.h options.h parse_cmd.h
property.o: property.c db.h config.h program.h structures.h  \
 version.h functions.h execute.h opcode.h options.h parse_cmd.h list.h \
 storage.h ref_count.h utils.h
regexpr.o: regexpr.c  config.h regexpr.h  \
 
server.o: server.c  config.h    \
     db.h program.h \
 structures.h version.h db_io.h disassemble.h execute.h opcode.h \
 options.h parse_cmd.h functions.h list.h log.h network.h server.h \
 parser.h  storage.h ref_count.h streams.h tasks.h timers.h \
  unparse.h utils.h
storage.o: storage.c  config.h exceptions.h list.h \
 structures.h  options.h ref_count.h storage.h utils.h \
 execute.h db.h program.h version.h opcode.h parse_cmd.h
streams.o: streams.c  config.h   log.h \
 structures.h storage.h ref_count.h streams.h
str_intern.o: str_intern.c  config.h log.h  \
 structures.h storage.h ref_count.h str_intern.h utils.h execute.h db.h \
 program.h version.h opcode.h options.h parse_cmd.h
sym_table.o: sym_table.c  config.h ast.h parser.h program.h \
 structures.h version.h sym_table.h exceptions.h log.h storage.h \
 ref_count.h utils.h execute.h db.h opcode.h options.h parse_cmd.h
tasks.o: tasks.c  config.h  db.h program.h \
 structures.h  version.h db_io.h decompile.h ast.h parser.h \
 sym_table.h eval_env.h eval_vm.h execute.h opcode.h options.h \
 parse_cmd.h exceptions.h functions.h list.h log.h match.h  \
 server.h network.h storage.h ref_count.h streams.h tasks.h utils.h \
 verbs.h
timers.o: timers.c  config.h   \
 options.h    timers.h
unparse.o: unparse.c  config.h  ast.h parser.h \
 program.h structures.h version.h sym_table.h decompile.h exceptions.h \
 functions.h execute.h db.h opcode.h options.h parse_cmd.h keywords.h \
 list.h log.h unparse.h storage.h ref_count.h streams.h utils.h
utils.o: utils.c  config.h   db.h \
 program.h structures.h version.h db_io.h exceptions.h list.h log.h \
 match.h numbers.h ref_count.h server.h network.h options.h storage.h \
 streams.h utils.h execute.h opcode.h parse_cmd.h
verbs.o: verbs.c  config.h db.h program.h structures.h \
  version.h exceptions.h execute.h opcode.h options.h \
 parse_cmd.h functions.h list.h log.h match.h parser.h server.h network.h \
 storage.h ref_count.h unparse.h utils.h verbs.h
version.o: version.c config.h version.h
net_single.o: net_single.c  config.h   \
  log.h structures.h network.h options.h server.h streams.h \
 utils.h execute.h db.h program.h version.h opcode.h parse_cmd.h
net_multi.o: net_multi.c  config.h   \
     exceptions.h list.h \
 structures.h log.h net_mplex.h net_multi.h net_proto.h options.h \
 network.h server.h streams.h storage.h ref_count.h timers.h  \
 utils.h execute.h db.h program.h version.h opcode.h parse_cmd.h
net_mp_selct.o: net_mp_selct.c  config.h  \
 options.h  log.h  structures.h net_mplex.h
net_mp_poll.o: net_mp_poll.c log.h  config.h structures.h \
 net_mplex.h storage.h ref_count.h
net_bsd_tcp.o: net_bsd_tcp.c  config.h   \
   list.h structures.h  log.h \
 name_lookup.h net_proto.h options.h server.h network.h streams.h \
 timers.h  utils.h execute.h db.h program.h version.h opcode.h \
 parse_cmd.h
net_bsd_lcl.o: net_bsd_lcl.c  config.h   \
  log.h structures.h net_proto.h options.h storage.h \
 ref_count.h utils.h execute.h db.h program.h version.h opcode.h \
 parse_cmd.h
client_bsd.o: client_bsd.c  config.h   \
   options.h  
