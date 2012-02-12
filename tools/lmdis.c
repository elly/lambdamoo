/* lmdis.c - lambdamoo database disassembler */

#include <stdio.h>
#include <unistd.h>

#include "lmdb.h"
#include "program.h"
#include "unparse.h"
#include "util.h"

static int srcmode = 0;

static void usage(const char *progn) {
	printf("Usage: %s [-a] [-s] [objid|verbid|propid]...\n", progn);
	printf("  -a:        print overall info\n");
	printf("  -s:        dump entire db in source form\n");
	printf("  -x:        list entries\n");
}

static void dumpdb(struct lmdb *db) {
	struct lmdb_stats st;
	lmdb_stats(db, &st);
	printf("db: version %u, %d objects, %d programs, %d users\n",
	       st.dbver, st.nobjs, st.nprogs, st.nusers);
}

static void dumpents(struct lmdb *db) {
	int oid = 0, vid, pid;
	struct Object *obj = lmdb_objbyid(db, oid);
	struct Verbdef *vdef;
	struct Propdef *pdef;
	while (obj) {
		printf("#%d: %s\n", lmdb_objid(obj), lmdb_objname(obj));
		vid = 0;
		vdef = lmdb_verbdefbyid(obj, vid);
		while (vdef) {
			printf("\t#%d:%s: %d bytes\n", lmdb_objid(obj),
			       lmdb_verbdefname(vdef), lmdb_verbdefsize(vdef));
			vid++;
			vdef = lmdb_verbdefbyid(obj, vid);
		}
		pid = 0;
		pdef = lmdb_propdefbyid(db, obj, pid);
		while (pdef) {
			printf("\t#%d.%s\n", lmdb_objid(obj),
			       lmdb_propdefname(pdef));
			pid++;
			pdef = lmdb_propdefbyid(db, obj, pid);
		}
		oid++;
		obj = lmdb_objbyid(db, oid);
	}
}

static void _dumpverbrec(void *arg, const char *msg) {
	unused(arg);
	printf("%s%s\n", srcmode ? "" : "\t", msg);
}

static void dumpverb(struct lmdb *db, int oid, const char *name) {
	struct Object *obj = lmdb_objbyid(db, oid);
	struct Verbdef *vdef;

	if (!obj) {
		fprintf(stderr, "dumpverb: no object #%d\n", oid);
		return;
	}

	vdef = lmdb_verbdefbyname(obj, name);
	if (!vdef) {
		fprintf(stderr, "dumpverb: no vdef #%d:%s\n", oid, name);
		return;
	}

	printf("#%d:%s #%d %s %s\n", oid, name, lmdb_verbdefowner(vdef),
	       lmdb_verbdefperms(vdef), lmdb_verbdefprep(vdef));
	unparse_program(lmdb_verbdefprog(vdef), _dumpverbrec, NULL, 0, 1, MAIN_VECTOR);
	if (srcmode)
		printf(".\n");
}

static void dumpprop(struct lmdb *db, int oid, const char *name) {
	unused(db);
	unused(oid);
	unused(name);
}

static void dumpobj(struct lmdb *db, int oid) {
	struct Object *obj = lmdb_objbyid(db, oid);
	struct Verbdef *v;
	struct Propdef *p;
	struct Pval *a;
	int i;
	if (!obj) {
		fprintf(stderr, "Unknown object: %d\n", oid);
		return;
	}

	printf("Object #%d: %s\n", lmdb_objid(obj), lmdb_objname(obj));
	for (i = 0, v = lmdb_verbdefbyid(obj, i); v; i++, v = lmdb_verbdefbyid(obj, i))
		printf("\tVerb: %s\n", lmdb_verbdefname(v));
	for (i = 0, p = lmdb_propdefbyid(db, obj, i); p; i++, p = lmdb_propdefbyid(db, obj, i)) {
		printf("\tPropdef: %s%s\n", lmdb_propdefname(p), lmdb_propdefislocal(db, obj, i) ? "" : " [inherited]");
		a = lmdb_propbyname(db, obj, lmdb_propdefname(p));
		printf("\t  = %s\n", lmdb_proptostr(a));
	}
}

int main(int argc, char *argv[]) {
	struct lmdb *db = lmdb_new();
	int r = lmdb_read(db, stdin);
	int opt;
	int i;

	if (r) {
		fprintf(stderr, "%s\n", lmdb_lasterror(db));
		return 1;
	}

	while ((opt = getopt(argc, argv, "ahsx")) != -1) {
		switch (opt) {
			case 'a':
				dumpdb(db);
				break;
			case 's':
				srcmode = 1;
				break;
			case 'x':
				dumpents(db);
				break;
			case 'h':
			default:
				usage(argv[0]);
				return 1;
		}
	}

	argc -= optind;
	argv += optind;

	for (i = 0; i < argc; i++) {
		char name[1024];
		int idx;
		/* #x:y: verb */
		if (sscanf(argv[i], "#%d:%1024s", &idx, name) == 2)
			dumpverb(db, idx, name);
		/* #x.y: prop */
		else if (sscanf(argv[i], "#%d.%1024s", &idx, name) == 2)
			dumpprop(db, idx, name);
		/* #x: object */
		else if (sscanf(argv[i], "#%d", &idx) == 1)
			dumpobj(db, idx);
	}

	return 0;
}
