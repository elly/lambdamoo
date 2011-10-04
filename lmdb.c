/* lmdb.c - lambdamoo database */

#include <string.h>

#include "db_private.h"
#include "list.h"
#include "lmdb.h"
#include "parser.h"
#include "storage.h"

struct lmdb {
	unsigned int dbver;
	int nobjs;
	int nprogs;
	int nusers;
	Objid lastobjid;
	Objid *users;
	Verbdef **verbs;
	Propdef **props;
	Object **objs;
};

struct lmdb *lmdb_new(void) {
	struct lmdb *db = mymalloc(sizeof(*db), M_INTERNAL);
	return db;
}

void lmdb_free(struct lmdb *lmdb) {
	myfree(lmdb, M_INTERNAL);
}

static int rdint(FILE *f, int *rv) {
	return fscanf(f, "%d\n", rv) != 1;
}

static int rdobjid(FILE *f, int *rv) {
	return rdint(f, rv);
}

static int rdstr(FILE *f, const char **str) {
	char buf[1024];
	if (!fgets(buf, sizeof(buf), f))
		return 1;
	if (strchr(buf, '\n'))
		*strchr(buf, '\n') = '\0';
	if (str)
		*str = str_dup(buf);
	return 0;
}

static int rdvar(FILE *f, Var *v) {
	int type;
	int i, n;
	Var list;
	if (rdint(f, &type))
		return 1;
	v->type = type;
	switch (type) {
		case TYPE_CLEAR:
		case TYPE_NONE:
			break;
		case _TYPE_STR:
			if (rdstr(f, &v->v.str))
				return 1;
			v->type |= TYPE_COMPLEX_FLAG;
			break;
		case TYPE_OBJ:
		case TYPE_ERR:
		case TYPE_INT:
		case TYPE_CATCH:
		case TYPE_FINALLY:
			if (rdint(f, &v->v.num))
				return 1;
			break;
		case _TYPE_FLOAT:
			/* XXX: not supported or used */
			break;
		case _TYPE_LIST:
			if (rdint(f, &n))
				return 1;
			list = new_list(n);
			v->v.list = list.v.list;
			for (i = 0; i < n; i++)
				if (rdvar(f, &v->v.list[i + 1]))
					return 1;
			break;
		default:
			return 1;
	}
	return 0;
}

int rdverbdef(FILE *f, Verbdef *v) {
	int dummy;
	if (rdstr(f, &v->name))
		return 1;
	if (rdobjid(f, &v->owner))
		return 1;
	if (rdint(f, &dummy))
		return 1;
	v->perms = dummy;
	if (rdint(f, &dummy))
		return 1;
	v->prep = dummy;
	return 0;
}

static int rdpropdef(FILE *f, Propdef *d) {
	return rdstr(f, &d->name);
}

static int rdpropval(FILE *f, Pval *v) {
	int dummy;
	if (rdvar(f, &v->var))
		return 1;
	if (rdobjid(f, &v->owner))
		return 1;
	if (rdint(f, &dummy))
		return 1;
	/* v->perms is short, wtf */
	v->perms = dummy;
	return 0;
}

int rdobj(struct lmdb *lmdb, FILE *f, Object **obj) {
	Objid oid;
	Object *o;
	int nverbs, npropdefs, nprops;
	Verbdef **pv;
	int i;
	char buf[32];
	if (fscanf(f, "#%d", &oid) != 1)
		return 1;
	if (!fgets(buf, sizeof(buf), f))
		return 1;
	if (!strcmp(buf, " recycled\n")) {
		*obj = NULL;
		printf("skip recycled: %d\n", oid);
		return 0;
	} else if (strcmp(buf, "\n"))
		return 1;
	o = mymalloc(sizeof(*o), M_INTERNAL);
	*obj = o;

	o->id = oid;
	if (rdstr(f, &o->name))
		return 1;
	rdstr(f, NULL);
	if (rdint(f, &o->flags))
		return 1;
	if (rdobjid(f, &o->owner))
		return 1;
	if (rdobjid(f, &o->location))
		return 1;
	if (rdobjid(f, &o->contents))
		return 1;
	if (rdobjid(f, &o->next))
		return 1;
	if (rdobjid(f, &o->parent))
		return 1;
	if (rdobjid(f, &o->child))
		return 1;
	if (rdobjid(f, &o->sibling))
		return 1;
	if (rdint(f, &nverbs))
		return 1;
	pv = &o->verbdefs;
	for (i = 0; i < nverbs; i++) {
		Verbdef *v = mymalloc(sizeof(*v), M_INTERNAL);
		if (rdverbdef(f, v))
			return 1;
		*pv = v;
		pv = &v->next;
	}

	if (rdint(f, &npropdefs))
		return 1;
	o->propdefs.cur_length = npropdefs;
	o->propdefs.max_length = npropdefs;
	o->propdefs.l = NULL;
	if (npropdefs != 0) {
		o->propdefs.l = mymalloc(npropdefs * sizeof(*o->propdefs.l), M_INTERNAL);
		for (i = 0; i < npropdefs; i++)
			if (rdpropdef(f, &o->propdefs.l[i]))
				return 1;
	}

	if (rdint(f, &nprops))
		return 1;
	if (nprops)
		o->propval = mymalloc(nprops * sizeof(*o->propval), M_INTERNAL);
	else
		o->propval = NULL;

	for (i = 0; i < nprops; i++)
		if (rdpropval(f, &o->propval[i]))
			return 1;

	lmdb->lastobjid++;
	return 0;
}

struct state {
	FILE *fh;
	int oid;
	const char *name;
	char prev;
};

static void _error(void *data, const char *msg) {
	struct state *st = data;
	printf("error: #%d:%s: %s\n", st->oid, st->name, msg);
}

static void _warning(void *data, const char *msg) {
	struct state *st = data;
	printf("warning: #%d:%s: %s\n", st->oid, st->name, msg);
}

static int _getc(void *data) {
	struct state *st = data;
	int c;

	c = fgetc(st->fh);
	if (c == '.' && st->prev == '\n') {
		c = fgetc(st->fh);
		return EOF;
	}
	if (c == EOF)
		_error(data, "Unexpected EOF");
	st->prev = c;
	return c;
}

int rdverb(struct lmdb *lmdb, FILE *f) {
	int oid, vnum;
	Object *obj;
	Verbdef *d;
	Program *p;
	Parser_Client pc = {
		.error = _error,
		.warning = _warning,
		.getch = _getc,
	};
	struct state st;
	if (fscanf(f, "#%d:%d\n", &oid, &vnum) != 2)
		return 1;
	obj = lmdb_objbyid(lmdb, oid);
	if (!obj)
		return 1;
	for (d = obj->verbdefs; d && vnum; d = d->next, vnum--)
		;
	if (!d)
		return 1;
	st.fh = f;
	st.oid = oid;
	st.name = d->name;
	st.prev = '\n';
	p = parse_program(current_version, pc, &st);
	d->program = p;
	printf("#%d:%s: program (%d bytes bytecode)\n", oid, d->name, p->main_vector.size);
	return 0;
}

struct Object *lmdb_objbyid(struct lmdb *lmdb, int id) {
	int i;
	for (i = 0; i < lmdb->nobjs; i++)
		if (lmdb->objs[i]->id == id)
			return lmdb->objs[i];
	return NULL;
}

int lmdb_read(struct lmdb *lmdb, FILE *f) {
	int dummy;
	int i;
	if (fscanf(f, "** LambdaMOO Database, Format Version %u **\n", &lmdb->dbver) != 1)
		return 1;
	printf("dbver: %u\n", lmdb->dbver);
	if (fscanf(f, "%d\n%d\n%d\n%d\n", &lmdb->nobjs, &lmdb->nprogs, &dummy, &lmdb->nusers) != 4)
		return 1;
	printf("db: %d objs %d progs %d users\n", lmdb->nobjs, lmdb->nprogs,
	       lmdb->nusers);
	lmdb->users = mymalloc(lmdb->nusers * sizeof(*lmdb->users), M_INTERNAL);
	for (i = 0; i < lmdb->nusers; i++) {
		if (rdobjid(f, &lmdb->users[i]))
			return 1;
		printf("user: %d\n", lmdb->users[i]);
	}
	lmdb->objs = mymalloc(lmdb->nobjs * sizeof(*lmdb->objs), M_INTERNAL);
	lmdb->lastobjid = -1;
	for (i = 0; i < lmdb->nobjs; i++) {
		if (rdobj(lmdb, f, &lmdb->objs[i]))
			return 1;
		if (lmdb->objs[i])
			printf("#%d %s\n", lmdb->objs[i]->id, lmdb->objs[i]->name);
	}
	for (i = 0; i < lmdb->nprogs; i++) {
		if (rdverb(lmdb, f))
			return 1;
	}
	return 0;
}

int lmdb_write(struct lmdb *lmdb, FILE *f) {
	return 0;
}
