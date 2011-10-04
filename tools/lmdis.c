/* lmdis.c - lambdamoo database disassembler */

#include <stdio.h>

#include "lmdb.h"

int main(int argc, char *argv[]) {
	struct lmdb *db = lmdb_new();
	int r = lmdb_read(db, stdin);
	if (r) {
		fprintf(stderr, "%s\n", lmdb_lasterror(db));
		return 1;
	}

	return 0;
}
