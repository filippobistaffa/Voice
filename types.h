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
#include <float.h>
#define MINVAL -FLT_MAX

#define CEIL(X, Y) (1 + (((X) - 1) / (Y)))
#define CEILBPC(X) CEIL(X, BITSPERCHUNK)
#define EFULL (N * (N - 1) / 2)
#define CMNE CEILBPC((N) > (EFULL) ? (N) : (EFULL))

#include <set>
#include <map>
#include <vector>
using namespace std;

typedef struct __attribute__((aligned(128))) stack {

	// CFSS
	agent a[2 * (EFULL + 1)], n[2 * N + 1];
	value vs[N], ve[EFULL + 1];
	chunk c[CMNE], m[CMNE];
	agent s[2 * N], cs[N];
	edge g[N * N], *isg;

	// Voice
	stack *st;
	set<set<agent> > *C;
	vector<set<agent> > *Z;
	set<agent> *Di, *Xi, *Yi, *Zi;
	map<set<set<agent> >, value> *Vi;
	vector<map<set<set<agent> >, value> > *V;
	set<set<agent> > *Cim1, *Ci;

} stack;

#endif /* TYPES_H_ */
