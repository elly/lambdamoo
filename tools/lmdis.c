/* lmdis.c - lambdamoo database disassembler */

#include <stdio.h>
#include <unistd.h>

#include "lmdb.h"

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
		pdef = lmdb_propdefbyid(obj, pid);
		while (pdef) {
			printf("\t#%d.%s\n", lmdb_objid(obj),
			       lmdb_propdefname(pdef));
			pid++;
			pdef = lmdb_propdefbyid(obj, pid);
		}
		oid++;
		obj = lmdb_objbyid(db, oid);
	}
}

static void dumpverb(struct lmdb *db, int oid, const char *name) {
	
}

static void dumpprop(struct lmdb *db, int oid, const char *name) {
	
}

static void dumpobj(struct lmdb *db, int oid) {
	struct Object *obj = lmdb_objbyid(db, oid);
	if (!obj) {
		fprintf(stderr, "Unknown object: %d\n", oid);
		return;
	}

	printf("Object #%d: %s\n", lmdb_objid(obj), lmdb_objname(obj));
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

	while ((opt = getopt(argc, argv, "ahx")) != -1) {
		switch (opt) {
			case 'a':
				dumpdb(db);
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
