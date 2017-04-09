#include "cfss.h"

value maxVi;

void initstack(stack *st) {

	st->n[N] = N;

	for (agent i = 0; i < N; i++) {
		XV(st->s, i) = 1;
		YV(st->s, i) = st->cs[i] = i;
		st->n[st->n[i] = N + i + 1] = i;
	}
}

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

/*__attribute__((always_inline)) inline
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
}*/

value setvalue(const set<agent> &s, const stack *st) {

	value ret = 0;

	for (set<agent>::const_iterator it1 = s.begin(); it1 != s.end(); ++it1) {
		ret += st->vs[*it1];
		for (set<agent>::const_iterator it2 = s.begin(); it2 != s.end(); ++it2)
			ret += *it1 > *it2 ? st->ve[st->g[*it1 * N + *it2]] : 0;
	}

	return ret;
}

void lines1115(stack *st, bool outer) {

	set<set<agent> > sos;
	cs2sos(st, sos);

	if (outer) {

		// outer loop

		maskagents(*(st->Xi), st->st);
		st->st->C = &sos;
		maxVi = -FLT_MAX;
		lines1115(st->st, false);
		(*st->Vi)[sos] = maxVi;

		#ifdef DEBUG
		printf("vi(C) <- %f\n\n", maxVi);
		#endif

	} else {

		// inner loop

		set<set<agent> > EZi;
		cal(sos, *(st->Zi), EZi);

		#ifdef DEBUG
		printsos(*(st->C), "C  ");
		printset(*(st->Zi), "Zi ");
		printsos(sos, "E  ");
		printsos(EZi, "EZi");
		printset(*(st->Yi), "Yi ");
		puts("");
		#endif

		if (EZi == *(st->C)) {

			value sum = 0;

			for (set<set<agent> >::const_iterator it = sos.begin(); it != sos.end(); ++it) {

				const value vC = setvalue(*it, st);
				#ifdef DEBUG
				printf("v(C) = v(");
				printset(*it, NULL, NULL, ") = ");
				printf("%f\n", vC);
				#endif

				set<agent> CmYi;
				SETOP(set_difference, *it, *(st->Yi), CmYi);
				const value vCmYi = setvalue(CmYi, st);
				#ifdef DEBUG
				printf("v(C \\ Yi) = v(");
				printset(CmYi, NULL, NULL, ") = ");
				printf("%f\n", vCmYi);
				#endif

				sum += (vC - vCmYi);
			}

			#ifdef DEBUG
			printf("Σ[v(C) - v(C \\ Yi)] = %f\n", sum);
			puts("");
			printset(*(st->Di), "Di");
			puts("");
			#endif

			for (set<agent>::const_iterator it = st->Di->begin(); it != st->Di->end(); ++it) {

				set<set<agent> > EZj;
				cal(sos, *ATP(*(st->Z), *it), EZj);
				const value vjEZj = (*(st->V))[*it][EZj];

				#ifdef DEBUG
				printf("j = %u\n", *it);
				printf("vj(EZj) = v(");
				printsos(EZj, NULL, NULL, ") = ");
				printf("%f\n", vjEZj);
				#endif

				sum += vjEZj;
			}

			#ifdef DEBUG
			printf("Σ[v(C) - v(C \\ Yi)] + Σ[v(EZj)] = %f\n", sum);
			puts("");
			printf("Old maxVi = %f\n", maxVi);
			#endif

			if (sum > maxVi) maxVi = sum;

			#ifdef DEBUG
			printf("New maxVi = %f\n", maxVi);
			puts("");
			#endif
		}
	}

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
		lines1115(st + 1, outer);
	}
}
