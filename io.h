#ifndef IO_H_
#define IO_H_

// Headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <set>
#include <vector>
using namespace std;

#include "instance.h"
#include "macros.h"
#include "types.h"

// Prints the content of a buffer
template <typename type>
__attribute__((always_inline)) inline
void printbuf(const type *buf, unsigned n, const char *name = NULL, const char *format = NULL, const char *after = NULL) {

	if (name) printf("%s = [ ", name);
	else printf("[ ");
	while (n--) {
		if (format) { printf(format, *(buf++)); printf(" "); }
		else cout << *(buf++) << " ";
	}
	printf("]%s", (after) ? after : "\n");
}

// Prints the content of a set

template <typename type>
__attribute__((always_inline)) inline
void printset(const set<type> &s, const char *name = NULL, const char *format = NULL, const char *after = NULL) {

	if (name) printf("%s = [ ", name);
	else printf("[ ");
	for (typename set<type>::const_iterator it = s.begin(); it != s.end(); ++it) {
		if (format) { printf(format, *it); printf(" "); }
		else cout << *it << " ";
	}
	printf("]%s", (after) ? after : "\n");
}

// Prints a set of sets

template <typename type>
__attribute__((always_inline)) inline
void printsos(const set<set<type> > &sos, const char *name = NULL) {

	if (name) cout << name << ":" << endl;
	for (typename set<set<type> >::const_iterator it = sos.begin(); it != sos.end(); ++it)
		printset(*it);
}

// Reads the graph file

void readg(const char *fn, edge *g, value *s, value *v, chunk *l);

// Reads the tree decomposition file

void readtd(const char *fn, vector<set<agent> > &tv/*, vector<set<agent> > &td*/);

#endif /* IO_H_ */
