/* lmdb.h - lambdamoo databases */

#ifndef LMDB_H
#define LMDB_H

#include <stdio.h>

#include "program.h"
#include "structures.h"

struct lmdb;
struct Object;

struct lmdb_stats {
	unsigned int dbver;
	int nobjs;
	int nprogs;
	int nusers;
};

struct lmdb *lmdb_new(void);
void lmdb_free(struct lmdb *lmdb);
void lmdb_stats(struct lmdb *lmdb, struct lmdb_stats *stats);
int lmdb_read(struct lmdb *lmdb, FILE *f);
int lmdb_write(struct lmdb *lmdb, FILE *f);

struct Object *lmdb_objbyid(struct lmdb *lmdb, int id);
Objid lmdb_objid(struct Object *obj);
const char *lmdb_objname(struct Object *obj);

struct Verbdef *lmdb_verbdefbyid(struct Object *obj, int idx);
struct Verbdef *lmdb_verbdefbyname(struct Object *obj, const char *name);
const char *lmdb_verbdefname(struct Verbdef *vdef);
const char *lmdb_verbdefperms(struct Verbdef *vdef);
const char *lmdb_verbdefprep(struct Verbdef *vdef);
Program *lmdb_verbdefprog(struct Verbdef *vdef);
int lmdb_verbdefsize(struct Verbdef *vdef);

struct Propdef *lmdb_propdefbyid(struct Object *obj, int idx);
const char *lmdb_propdefname(struct Propdef *pdef);

const char *lmdb_lasterror(struct lmdb *lmdb);

#endif /* !LMDB_H */
