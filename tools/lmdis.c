/* lmdis.c - lambdamoo database disassembler */

#include <stdio.h>

#include "lmdb.h"

int main(void) {
	struct lmdb *db = lmdb_new();
	return lmdb_read(db, stdin);
}
