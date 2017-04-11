#include "set.h"

value setvalue(const set<agent> &s, const edge *g, const value *vs, const value *ve) {

	value ret = 0;

	for (set<agent>::const_iterator it1 = s.begin(); it1 != s.end(); ++it1) {
		ret += vs[*it1];
		for (set<agent>::const_iterator it2 = s.begin(); it2 != s.end(); ++it2)
			ret += *it1 > *it2 ? ve[g[*it1 * N + *it2]] : 0;
	}

	return ret;
}

void U(const set<set<agent> > &pcal, const set<agent> &p, const set<set<agent> > &qcal, const set<agent> &q,
       set<set<agent> > &res) {

	//printsos(pcal, "pcal");
	//printsos(qcal, "qcal");
	//printset(p, "p");
	//printset(q, "q");
	//printsos(res, "res");

	// P \ Q
	set<agent> pmq;
	SETOP(set_difference, p, q, pmq);
	//printset(pmq, "pmq");

	// Q \ P
	set<agent> qmp;
	SETOP(set_difference, q, p, qmp);
	//printset(qmp, "qmp");

	for (set<set<agent> >::const_iterator it = pcal.begin(); it != pcal.end(); ++it) {
		set<agent> A = *it;
		if (includes(pmq.begin(), pmq.end(), A.begin(), A.end())) res.insert(A);
	}

	//printsos(res, "res");

	for (set<set<agent> >::const_iterator it = qcal.begin(); it != qcal.end(); ++it) {
		set<agent> B = *it;
		if (includes(qmp.begin(), qmp.end(), B.begin(), B.end())) res.insert(B);
	}

	//printsos(res, "res");

	for (set<set<agent> >::const_iterator itp = pcal.begin(); itp != pcal.end(); ++itp) {
		for (set<set<agent> >::const_iterator itq = qcal.begin(); itq != qcal.end(); ++itq) {
			set<agent> A = *itp, B = *itq, AiB;
			SETOP(set_intersection, A, B, AiB);
			if (!AiB.empty()) {
				set<agent> AuB;
				SETOP(set_union, A, B, AuB);
				res.insert(AuB);
			}
		}
	}

	//printsos(res, "res");
}

void cal(const set<set<agent> > &scal, const set<agent> &s, set<set<agent> > &res) {

	for (set<set<agent> >::const_iterator it = scal.begin(); it != scal.end(); ++it) {
		set<agent> C = *it, i;
		SETOP(set_intersection, C, s, i);
		if (!i.empty()) res.insert(i);
	}
}

void mergeall(const set<set<agent> > &sos, set<agent> &res) {

	res.clear();

	for (set<set<agent> >::const_iterator it = sos.begin(); it != sos.end(); ++it)
		res.insert(it->begin(), it->end());
}
