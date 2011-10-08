ast.o: ast.c my-string.h config.h ast.h parser.h program.h structures.h \
 my-stdio.h version.h sym_table.h list.h log.h storage.h ref_count.h \
 utils.h execute.h db.h opcode.h options.h parse_cmd.h
code_gen.o: code_gen.c ast.h config.h parser.h program.h structures.h \
 my-stdio.h version.h sym_table.h exceptions.h opcode.h options.h \
 storage.h ref_count.h str_intern.h utils.h execute.h db.h parse_cmd.h \
 my-stdlib.h
db_file.o: db_file.c my-stat.h config.h my-stdio.h my-stdlib.h db.h \
 program.h structures.h version.h db_io.h db_private.h exceptions.h \
 list.h log.h options.h server.h network.h storage.h ref_count.h \
 streams.h str_intern.h tasks.h execute.h opcode.h parse_cmd.h timers.h \
 my-time.h
db_io.o: db_io.c my-ctype.h config.h my-stdarg.h my-stdio.h my-stdlib.h \
 db_io.h program.h structures.h version.h db_private.h exceptions.h \
 list.h log.h numbers.h parser.h storage.h ref_count.h streams.h \
 str_intern.h unparse.h
db_objects.o: db_objects.c config.h db.h program.h structures.h \
 my-stdio.h version.h db_private.h exceptions.h list.h storage.h \
 ref_count.h utils.h execute.h opcode.h options.h parse_cmd.h
db_properties.o: db_properties.c config.h db.h program.h structures.h \
 my-stdio.h version.h db_private.h exceptions.h list.h storage.h \
 ref_count.h utils.h execute.h opcode.h options.h parse_cmd.h
db_verbs.o: db_verbs.c my-stdlib.h config.h my-string.h db.h program.h \
 structures.h my-stdio.h version.h db_private.h exceptions.h db_tune.h \
 list.h log.h parse_cmd.h storage.h ref_count.h utils.h execute.h \
 opcode.h options.h
decompile.o: decompile.c ast.h config.h parser.h program.h structures.h \
 my-stdio.h version.h sym_table.h decompile.h exceptions.h opcode.h \
 options.h storage.h ref_count.h utils.h execute.h db.h parse_cmd.h
disassemble.o: disassemble.c my-stdio.h config.h bf_register.h db.h \
 program.h structures.h version.h functions.h execute.h opcode.h \
 options.h parse_cmd.h list.h storage.h ref_count.h streams.h unparse.h \
 utils.h verbs.h
eval_env.o: eval_env.c config.h eval_env.h structures.h my-stdio.h \
 version.h storage.h ref_count.h sym_table.h utils.h execute.h db.h \
 program.h opcode.h options.h parse_cmd.h
eval_vm.o: eval_vm.c config.h db_io.h program.h structures.h my-stdio.h \
 version.h decompile.h ast.h parser.h sym_table.h eval_vm.h execute.h \
 db.h opcode.h options.h parse_cmd.h log.h storage.h ref_count.h tasks.h
exceptions.o: exceptions.c exceptions.h config.h
execute.o: execute.c my-string.h config.h db.h program.h structures.h \
 my-stdio.h version.h db_io.h decompile.h ast.h parser.h sym_table.h \
 eval_env.h eval_vm.h execute.h opcode.h options.h parse_cmd.h \
 exceptions.h functions.h list.h log.h numbers.h server.h network.h \
 storage.h ref_count.h streams.h tasks.h timers.h my-time.h utils.h
extensions.o: extensions.c bf_register.h functions.h my-stdio.h config.h \
 execute.h db.h program.h structures.h version.h opcode.h options.h \
 parse_cmd.h db_tune.h utils.h
functions.o: functions.c my-stdarg.h config.h bf_register.h db_io.h \
 program.h structures.h my-stdio.h version.h functions.h execute.h db.h \
 opcode.h options.h parse_cmd.h list.h log.h server.h network.h storage.h \
 ref_count.h streams.h unparse.h utils.h
keywords.o: keywords.c my-ctype.h config.h my-string.h keywords.h \
 structures.h my-stdio.h version.h tokens.h ast.h parser.h program.h \
 sym_table.h y.tab.h utils.h execute.h db.h opcode.h options.h \
 parse_cmd.h
list.o: list.c my-ctype.h config.h my-string.h bf_register.h exceptions.h \
 functions.h my-stdio.h execute.h db.h program.h structures.h version.h \
 opcode.h options.h parse_cmd.h list.h log.h md5.h pattern.h random.h \
 my-stdlib.h ref_count.h streams.h storage.h unparse.h utils.h
log.o: log.c my-stdarg.h config.h my-stdio.h my-string.h my-time.h \
 bf_register.h functions.h execute.h db.h program.h structures.h \
 version.h opcode.h options.h parse_cmd.h log.h storage.h ref_count.h \
 streams.h utils.h
match.o: match.c my-stdlib.h config.h my-string.h db.h program.h \
 structures.h my-stdio.h version.h exceptions.h match.h parse_cmd.h \
 storage.h ref_count.h unparse.h utils.h execute.h opcode.h options.h
md5.o: md5.c my-string.h config.h md5.h
name_lookup.o: name_lookup.c options.h config.h
network.o: network.c network.h config.h options.h structures.h my-stdio.h \
 server.h
numbers.o: numbers.c my-math.h my-stdlib.h config.h my-string.h my-time.h \
 functions.h my-stdio.h execute.h db.h program.h structures.h version.h \
 opcode.h options.h parse_cmd.h log.h random.h storage.h ref_count.h \
 utils.h
objects.o: objects.c db.h config.h program.h structures.h my-stdio.h \
 version.h db_io.h exceptions.h execute.h opcode.h options.h parse_cmd.h \
 functions.h list.h numbers.h quota.h server.h network.h storage.h \
 ref_count.h utils.h
parse_cmd.o: parse_cmd.c my-ctype.h config.h my-stdio.h my-stdlib.h \
 my-string.h my-time.h db.h program.h structures.h version.h list.h \
 match.h parse_cmd.h storage.h ref_count.h utils.h execute.h opcode.h \
 options.h
pattern.o: pattern.c my-ctype.h config.h my-stdlib.h my-string.h \
 pattern.h regexpr.h storage.h structures.h my-stdio.h ref_count.h \
 streams.h
program.o: program.c ast.h config.h parser.h program.h structures.h \
 my-stdio.h version.h sym_table.h exceptions.h list.h storage.h \
 ref_count.h utils.h execute.h db.h opcode.h options.h parse_cmd.h
property.o: property.c db.h config.h program.h structures.h my-stdio.h \
 version.h functions.h execute.h opcode.h options.h parse_cmd.h list.h \
 storage.h ref_count.h utils.h
quota.o: quota.c config.h db.h program.h structures.h my-stdio.h \
 version.h quota.h
ref_count.o: ref_count.c config.h exceptions.h ref_count.h storage.h \
 structures.h my-stdio.h
regexpr.o: regexpr.c my-stdio.h config.h regexpr.h my-stdlib.h \
 my-string.h
storage.o: storage.c my-stdlib.h config.h exceptions.h list.h \
 structures.h my-stdio.h options.h ref_count.h storage.h utils.h \
 execute.h db.h program.h version.h opcode.h parse_cmd.h
streams.o: streams.c my-stdarg.h config.h my-string.h my-stdio.h log.h \
 structures.h storage.h ref_count.h streams.h
str_intern.o: str_intern.c my-stdlib.h config.h log.h my-stdio.h \
 structures.h storage.h ref_count.h str_intern.h utils.h execute.h db.h \
 program.h version.h opcode.h options.h parse_cmd.h
sym_table.o: sym_table.c my-stdio.h config.h ast.h parser.h program.h \
 structures.h version.h sym_table.h exceptions.h log.h storage.h \
 ref_count.h utils.h execute.h db.h opcode.h options.h parse_cmd.h
tasks.o: tasks.c my-string.h config.h my-time.h db.h program.h \
 structures.h my-stdio.h version.h db_io.h decompile.h ast.h parser.h \
 sym_table.h eval_env.h eval_vm.h execute.h opcode.h options.h \
 parse_cmd.h exceptions.h functions.h list.h log.h match.h random.h \
 my-stdlib.h server.h network.h storage.h ref_count.h streams.h tasks.h \
 utils.h verbs.h
timers.o: timers.c my-signal.h config.h my-stdlib.h my-sys-time.h \
 options.h my-time.h my-unistd.h timers.h
unparse.o: unparse.c my-ctype.h config.h my-stdio.h ast.h parser.h \
 program.h structures.h version.h sym_table.h decompile.h exceptions.h \
 functions.h execute.h db.h opcode.h options.h parse_cmd.h keywords.h \
 list.h log.h unparse.h storage.h ref_count.h streams.h utils.h
utils.o: utils.c my-ctype.h config.h my-stdio.h my-string.h db.h \
 program.h structures.h version.h db_io.h exceptions.h list.h log.h \
 match.h numbers.h ref_count.h server.h network.h options.h storage.h \
 streams.h utils.h execute.h opcode.h parse_cmd.h
verbs.o: verbs.c my-string.h config.h db.h program.h structures.h \
 my-stdio.h version.h exceptions.h execute.h opcode.h options.h \
 parse_cmd.h functions.h list.h log.h match.h parser.h server.h network.h \
 storage.h ref_count.h unparse.h utils.h verbs.h
version.o: version.c config.h version.h
lmdb.o: lmdb.c db.h config.h program.h structures.h my-stdio.h version.h \
 db_private.h exceptions.h list.h lmdb.h parser.h storage.h ref_count.h
net_single.o: net_single.c my-ctype.h config.h my-fcntl.h my-stdio.h \
 my-unistd.h log.h structures.h network.h options.h server.h streams.h \
 utils.h execute.h db.h program.h version.h opcode.h parse_cmd.h
net_multi.o: net_multi.c my-ctype.h config.h my-fcntl.h my-ioctl.h \
 my-signal.h my-stdio.h my-stdlib.h my-string.h my-unistd.h exceptions.h \
 list.h structures.h log.h net_mplex.h net_multi.h net_proto.h options.h \
 network.h server.h streams.h storage.h ref_count.h timers.h my-time.h \
 utils.h execute.h db.h program.h version.h opcode.h parse_cmd.h
net_mp_poll.o: net_mp_poll.c my-poll.h config.h log.h my-stdio.h \
 structures.h net_mplex.h storage.h ref_count.h
net_bsd_tcp.o: net_bsd_tcp.c my-inet.h config.h my-in.h my-types.h \
 my-socket.h my-stdlib.h my-string.h my-unistd.h list.h structures.h \
 my-stdio.h log.h name_lookup.h net_proto.h options.h server.h network.h \
 streams.h timers.h my-time.h utils.h execute.h db.h program.h version.h \
 opcode.h parse_cmd.h
