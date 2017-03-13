#include "voice.h"

int main(int argc, char *argv[]) {

	// Allocate data structures
	edge *isg = (edge *)malloc(sizeof(edge) * N * N);	// adjacency matrix
	value *v = (value *)malloc(sizeof(value) * E);		// edge weights
	value *s = (value *)malloc(sizeof(value) * N);		// self-loop weights
	chunk *l = (chunk *)calloc(C, sizeof(chunk));		// leaders bitmask
	vector<set<agent> > tv(NTD, set<agent>());		// tree decomposition vertices
	vector<set<agent> > td(NTD, set<agent>());		// tree decomposition descendants

	// Read graph file

	readg(argv[1], isg, s, v, l);

	// Read tree decomposition

	readtd(argv[2], tv, td);

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
		printset(tv[i]);
	}
	puts("\nDescendants in tree decomposition");
	for (agent i = 0; i < NTD; i++) {
		printf("%u: ", i);
		printset(td[i]);
	}
	puts("");
	#endif

	free(isg);
	free(v);
	free(s);
	free(l);

	return 0;
}
