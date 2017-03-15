#ifndef TYPES_H_
#define TYPES_H_

#define __STDC_LIMIT_MACROS

#include <stdint.h>
typedef uint64_t chunk;
#define BITSPERCHUNK 64
#define ZERO 0ULL
#define ONE 1ULL
typedef uint32_t agent;
typedef uint32_t edge;
typedef float value;

#define CEIL(X, Y) (1 + (((X) - 1) / (Y)))
#define CEILBPC(X) CEIL(X, BITSPERCHUNK)
#define CMNE CEILBPC((N) > (E) ? (N) : (E))

typedef struct __attribute__((aligned(128))) {
	agent a[2 * (E + 1)], n[2 * N + 1];
	value vs[N], ve[E + 1];
	chunk c[CMNE], m[CMNE];
	agent s[2 * N], cs[N];
	edge g[N * N];
} stack;

#endif /* TYPES_H_ */
