#ifndef IO_H_
#define IO_H_

// Headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <set>
#include <vector>
using namespace std;

#include "instance.h"
#include "macros.h"
#include "types.h"

// Reads the graph file

void readg(const char *fn, edge *g, value *s, value *v, chunk *l);

// Reads the tree decomposition file

void readtd(const char *fn, vector<set<agent> > &tv, vector<set<agent> > &td);

#endif /* IO_H_ */
