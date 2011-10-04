/* lmdb.h - lambdamoo databases */

#ifndef LMDB_H
#define LMDB_H

#include <stdio.h>

struct lmdb;
struct Object;

struct lmdb *lmdb_new(void);
void lmdb_free(struct lmdb *lmdb);
int lmdb_read(struct lmdb *lmdb, FILE *f);
int lmdb_write(struct lmdb *lmdb, FILE *f);

struct Object *lmdb_objbyid(struct lmdb *lmdb, int id);

struct Verbdef *lmdb_verbdefbyid(struct Object *obj, int idx);
struct Verbdef *lmdb_verbdefbyname(struct Object *obj, const char *name);

const char *lmdb_lasterror(struct lmdb *lmdb);

#endif /* !LMDB_H */
