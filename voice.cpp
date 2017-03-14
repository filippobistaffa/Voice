#include "voice.h"

int main(int argc, char *argv[]) {

	// Allocate data structures
	edge *isg = (edge *)malloc(sizeof(edge) * N * N);	// adjacency matrix
	value *v = (value *)malloc(sizeof(value) * E);		// edge weights
	value *s = (value *)malloc(sizeof(value) * N);		// self-loop weights
	chunk *l = (chunk *)calloc(C, sizeof(chunk));		// leaders bitmask
	vector<set<agent> > X(NTD, set<agent>());		// tree decomposition vertices
	vector<set<agent> > D(NTD, set<agent>());		// tree decomposition descendants

	// Read graph file

	readg(argv[1], isg, s, v, l);

	// Read tree decomposition

	readtd(argv[2], X, D);

	#ifdef DEBUG
	puts("Singletons");
	for (agent i = 0; i < N; i++)
		printf("%u = %f\n", i, s[i]);
	puts("\nEdges");
	for (agent i = 0; i < E; i++)
		printf("%u = %f\n", i + N, v[i]);
	puts("\nAdjacency matrix");
	for (agent i = 0; i < N; i++)
		printbuf(isg + i * N, N, NULL, "% 2u");
	puts("\nTree decomposition vertices");
	for (agent i = 0; i < NTD; i++) {
		printf("%u: ", i);
		printset(X[i]);
	}
	puts("\nDescendants in tree decomposition");
	for (agent i = 0; i < NTD; i++) {
		printf("%u: ", i);
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
		set_union(XU[i - 1].begin(), XU[i - 1].end(), X[i].begin(), X[i].end(), inserter(XU[i], XU[i].begin()));
		set_difference(X[i].begin(), X[i].end(), XU[i - 1].begin(), XU[i - 1].end(), inserter(Y[i], Y[i].begin()));
	}

	for (agent i = 0; i < NTD; i++) {
		set_difference(X[i].begin(), X[i].end(), Y[i].begin(), Y[i].end(), inserter(Z[i], Z[i].begin()));
		printf("Y_%u = ", i);
		printset(Y[i]);
		printf("Z_%u = ", i);
		printset(Z[i]);
	}

	free(isg);
	free(v);
	free(s);
	free(l);

	return 0;
}
