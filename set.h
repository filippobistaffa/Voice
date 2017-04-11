#ifndef SET_H_
#define SET_H_

// Headers

#include <set>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;

#include "instance.h"
#include "types.h"
#include "io.h"

#define SETOP(OP, X, Y, R) (OP((X).begin(), (X).end(), (Y).begin(), (Y).end(), inserter((R), (R).begin())))

value setvalue(const set<agent> &s, const edge *g, const value *vs, const value *ve);

void U(const set<set<agent> > &pcal, const set<agent> &p, const set<set<agent> > &qcal, const set<agent> &q,
       set<set<agent> > &res);

void cal(const set<set<agent> > &pcal, const set<agent> &p, set<set<agent> > &res);

void mergeall(const set<set<agent> > &sos, set<agent> &res);

#endif /* SET_H_ */
