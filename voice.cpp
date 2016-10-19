#include "voice.h"

int main(int argc, char *argv[]) {

	// Allocate data structures
	edge *isg = (edge *)malloc(sizeof(edge) * N * N);
	value *v = (value *)malloc(sizeof(value) * E);
	value *s = (value *)malloc(sizeof(value) * N);
	chunk *l = (chunk *)calloc(C, sizeof(chunk));
	agent *tv = (edge *)malloc(sizeof(edge) * NTD * NTD);
	agent *td = (edge *)malloc(sizeof(edge) * NTD * NTD);

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
		printbuf(tv + i * NTD + 1, tv[i * NTD]);
	}
	puts("\nDescendants in tree decomposition");
	for (agent i = 0; i < NTD; i++) {
		printf("%u: ", i);
		printbuf(td + i * NTD + 1, td[i * NTD]);
	}
	puts("");
	#endif

	free(isg);
	free(tv);
	free(td);
	free(v);
	free(s);
	free(l);

	return 0;
}
