#include "cfss.h"

void maskagents(const set<agent> &s, stack *st) {

	memset(st->m, 0, sizeof(chunk) * CMNE);
	memset(st->c, 0, sizeof(chunk) * CMNE);

	for (set<agent>::const_iterator it = s.begin(); it != s.end(); ++it)
		SET(st->m, *it);

	for (edge j = 1; j < E + 1; j++)
		if ((s.find(XV(st->a, j)) != s.end()) && (s.find(YV(st->a, j)) != s.end()))
			SET(st->c, j);
}

__attribute__((always_inline)) inline
void cs2sos(stack *st, set<set<agent> > &sos) {

	const agent *p = st->n + N + 1;
        agent i, m = st->n[N];

	do {
		i = *(p++);
		if (!GET(st->m, i)) continue;
		set<agent> s;
		for (agent j = 0; j < XV(st->s, i); j++)
			s.insert(st->cs[YV(st->s, i) + j]);
		sos.insert(s);
        } while (--m);
}

// Contract edge between v1 and v2

__attribute__((always_inline)) inline
void contract(stack *st, agent v1, agent v2) {

	agent i, m = st->n[N];
	const agent *p = st->n + N + 1;
	edge e, f;

	do if ((i = *(p++)) != v1)
		if ((e = st->g[i * N + v2])) {
			if ((f = st->g[i * N + v1])) {
				if (!GET(st->c, f)) CLEAR(st->c, e);
				CLEAR(st->c, f);
			}
			st->g[i * N + v1] = st->g[v1 * N + i] = e;
			XV(st->a, e) = v1;
			YV(st->a, e) = i;
		}
	while (--m);
}

// Merge coalitions of v1 and v2

__attribute__((always_inline)) inline
void merge(stack *st, agent v1, agent v2) {

	agent a, b, i, j, min = v1, max = v2, *p = st->n + N + 1;

	if (YV(st->s, max) < YV(st->s, min)) {
		b = max;
		max = min;
		min = b;
	}

	a = XV(st->s, min);
	b = XV(st->s, max);
	max = YV(st->s, max);
	YV(st->s, v1) = min = YV(st->s, min);
	XV(st->s, v1) = a + b;
	agent *c = (agent *)malloc(sizeof(agent) * b);
	memcpy(c, st->cs + max, sizeof(agent) * b);
	memmove(st->cs + min + a + b, st->cs + min + a, sizeof(agent) * (max - min - a));
	memmove(st->cs + min, st->cs + min, sizeof(agent) * a);
	memcpy(st->cs + min + a, c, sizeof(agent) * b);
	free(c);

	if ((j = st->n[st->n[N] + N]) != v2) {
		st->n[j] = st->n[v2];
		st->n[st->n[v2]] = j;
		st->n[v2] = st->n[N] + N;
	}

	j = --(st->n[N]);

	do if ((i = *(p++)) != v1) {
		a = YV(st->s, i);
		if (a > min && a < max) YV(st->s, i) = a + b;
	} while (--j);
}

// Print coalition structure

__attribute__((always_inline)) inline
void printcs(stack *st) {

	const agent *p = st->n + N + 1;
        agent i, m = st->n[N];

	do {
		i = *(p++);
		if (!GET(st->m, i)) continue;
                printf("{ ");
                for (agent j = 0; j < XV(st->s, i); j++)
                	printf("%s%u%s ", i == st->cs[YV(st->s, i) + j] ? "<" : "", 
			       st->cs[YV(st->s, i) + j], i == st->cs[YV(st->s, i) + j] ? ">" : "");
                printf("}\n");
        } while (--m);

	puts("");
}

void cfss(stack *st) {

	set<set<agent> > sos;
	cs2sos(st, sos);

	#ifdef DEBUG
	//printcs(st);
	printsos(sos);
	puts("");
	#endif

	chunk tmp[CMNE];
	memcpy(tmp, st->c, sizeof(chunk) * CMNE);
	edge popc = MASKPOPCNT(tmp, CMNE);

	for (edge i = 0, e = MASKFFS(tmp, CMNE); i < popc; i++, e = MASKCLEARANDFFS(tmp, e, CMNE)) {
		agent v1 = XV(st->a, e);
		agent v2 = YV(st->a, e);
		CLEAR(st->c, st->g[v1 * N + v2]);
		st[1] = st[0];
		merge(st + 1, v1, v2);
		contract(st + 1, v1, v2);
		cfss(st + 1);
	}
}
