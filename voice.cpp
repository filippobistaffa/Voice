#include "voice.h"

int main(int argc, char *argv[]) {

	// Allocate data structures
	stack *st = (stack *)malloc(sizeof(stack) * N);		// stack
	vector<set<agent> > X(NTD, set<agent>());		// tree decomposition vertices
	vector<set<agent> > D(NTD, set<agent>());		// tree decomposition descendants

	// Read graph file

	readg(argv[1], st->g, st->a, st->vs, st->ve/*, st->l*/);

	// Read tree decomposition

	readtd(argv[2], X, D);

	#ifdef DEBUG
	puts("Singletons");
	for (agent i = 0; i < N; i++)
		printf("%u = %f\n", i, st->vs[i]);
	puts("\nEdges");
	for (edge i = 1; i < E + 1; i++)
		printf("%u (%u, %u) = %f\n", i, XV(st->a, i), YV(st->a, i), st->ve[i]);
	puts("\nAdjacency matrix");
	for (agent i = 0; i < N; i++)
		printbuf(st->g + i * N, N, NULL, "% 2u");
	puts("\nTree decomposition vertices");
	for (agent i = 0; i < NTD; i++) {
		printf("X_%u = ", i);
		printset(X[i]);
	}
	puts("\nDescendants in tree decomposition");
	for (agent i = 0; i < NTD; i++) {
		printf("D_%u = ", i);
		printset(D[i]);
	}
	puts("");
	#endif

	/*set<agent> p, q;
	agent pbuf[] = {1, 2, 3};
	agent qbuf[] = {3, 4, 5};
	p.insert(pbuf, pbuf + 3);
	q.insert(qbuf, qbuf + 3);

	printset(p, "p");
	printset(q, "q");
	puts("");

	agent pcal1buf[] = {1};
	agent pcal2buf[] = {2, 3};
	set<agent> pcal1, pcal2;
	pcal1.insert(pcal1buf, pcal1buf + 1);
	pcal2.insert(pcal2buf, pcal2buf + 2);
	set<set<agent> > pcal;
	pcal.insert(pcal1);
	pcal.insert(pcal2);
	printsos(pcal, "pcal");
	puts("");

	agent qcal1buf[] = {3, 4};
	agent qcal2buf[] = {5};
	set<agent> qcal1, qcal2;
	qcal1.insert(qcal1buf, qcal1buf + 2);
	qcal2.insert(qcal2buf, qcal2buf + 1);
	set<set<agent> > qcal;
	qcal.insert(qcal1);
	qcal.insert(qcal2);
	printsos(qcal, "qcal");
	puts("");

	set<set<agent> > u;
	U(pcal, p, qcal, q, u);
	printsos(u, "u");
	puts("");

	agent ppbuf[] = {1, 2};
	set<agent> pp;
	pp.insert(ppbuf, ppbuf + 2);
	printset(pp, "pp");
	puts("");

	set<set<agent> > pcalp;
	cal(pcal, pp, pcalp);
	printsos(pcalp, "pcalp");
	puts("");

	agent qpbuf[] = {4, 5};
	set<agent> qp;
	qp.insert(qpbuf, qpbuf + 2);
	printset(qp, "qp");
	puts("");

	set<set<agent> > qcalq;
	cal(qcal, qp, qcalq);
	printsos(qcalq, "qcalq");*/

	vector<set<agent> > Y(NTD, set<agent>());
	vector<set<agent> > Z(NTD, set<agent>());
	vector<set<agent> > XU(NTD, set<agent>());
	Y[0] = XU[0] = X[0];

	for (agent i = 1; i < NTD; i++) {
		SETOP(set_union, XU[i - 1], X[i], XU[i]);
		SETOP(set_difference, X[i], XU[i - 1], Y[i]);
	}

	for (agent i = 0; i < NTD; i++) {
		SETOP(set_difference, X[i], Y[i], Z[i]);
		#ifdef DEBUG
		printf("Y_%u = ", i);
		printset(Y[i]);
		printf("Z_%u = ", i);
		printset(Z[i]);
		#endif
	}

	#ifdef DEBUG
	puts("");
	#endif
	value vi[NTD];
	
	// initialise stack for CFSS

	st->n[N] = N;

	for (agent i = 0; i < N; i++) {
		XV(st->s, i) = 1;
		YV(st->s, i) = st->cs[i] = i;
		st->n[st->n[i] = N + i + 1] = i;
	}

	for (int i = NTD - 1; i >= 0; i--) {

		#ifdef DEBUG
		printf("Z_%u = ", i);
		printset(Z[i]);
		#endif
		memset(st->m, 0, sizeof(chunk) * CMNE);
		memset(st->c, 0, sizeof(chunk) * CMNE);

		for (set<agent>::const_iterator it = Z[i].begin(); it != Z[i].end(); ++it)
			SET(st->m, *it);

		for (edge j = 1; j < E + 1; j++)
			if ((Z[i].find(XV(st->a, j)) != Z[i].end()) && (Z[i].find(YV(st->a, j)) != Z[i].end()))
				SET(st->c, j);

		#ifdef DEBUG
		bitset<BITSPERCHUNK> bsm(st->m[0]);
		cout << "st->m = " << bsm << endl;
		bitset<BITSPERCHUNK> bsc(st->c[0]);
		cout << "st->c = " << bsc << endl << endl;
		#endif

		cfss(st);
	}

	free(st);

	return 0;
}
