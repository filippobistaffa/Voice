#include "io.h"

#define MAXLINE 1000
static char line[MAXLINE];

void readg(const char *fn, edge *g, value *s, value *v, chunk *l) {

	FILE *f = fopen(fn, "r");

	for (agent i = 0; i < N; i++) {
		fgets(line, MAXLINE, f);
		char *pch = line;
		if (*pch == '*') {
			SET(l, i);
			pch++;
		}
		s[i] = atof(pch);
	}

	for (agent i = 0; i < E; i++) {
		agent v1, v2;
		fscanf(f, "%u %u %f", &v1, &v2, v + i);
		g[v2 * N + v1] = g[v1 * N + v2] = N + i;
	}

	fclose(f);
}

void readtd(const char *fn, vector<set<agent> > &tv, vector<set<agent> > &td) {

	FILE *f = fopen(fn, "r");

	// Skip first line
	fgets(line, MAXLINE, f);

	for (agent i = 0; i < NTD; ++i) {
		fgets(line, MAXLINE, f);
		char *pch = strtok(line, " ");
		while (pch != NULL) {
			tv[i].insert(atoi(pch));
			pch = strtok(NULL, " ");
		}
	}

	for (agent i = 0; i < NTD; ++i) {
		fgets(line, MAXLINE, f);
		if (strlen(line) == 1) continue;
		char *pch = strtok(line, " ");
		while (pch != NULL) {
			td[i].insert(atoi(pch));
			pch = strtok(NULL, " ");
		}
	}
}
