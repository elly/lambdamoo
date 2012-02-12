/* utility macros and functions */

#ifndef UTIL_H
#define UTIL_H

#include <assert.h>

#define unused(x) (void)(x)

static inline int ui2si(unsigned int i) {
	assert(i <= INT_MAX);
	return (int)i;
}

static inline unsigned int si2ui(int i) {
	assert(i >= 0);
	return (unsigned int)i;
}

#endif /* !UTIL_H */
