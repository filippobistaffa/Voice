#include "set.h"

void U(const set<set<agent> > &pcal, const set<agent> &p, const set<set<agent> > &qcal, const set<agent> &q,
       set<set<agent> > &res) {

	// P \ Q
	set<agent> pmq;
	set_difference(p.begin(), p.end(), q.begin(), q.end(), inserter(pmq, pmq.begin()));

	// Q \ P
	set<agent> qmp;
	set_difference(q.begin(), q.end(), p.begin(), p.end(), inserter(qmp, qmp.begin()));

	for (set<set<agent> >::const_iterator it = pcal.begin(); it != pcal.end(); ++it) {
		set<agent> A = *it;
		if (includes(pmq.begin(), pmq.end(), A.begin(), A.end())) res.insert(A);
	}

	for (set<set<agent> >::const_iterator it = qcal.begin(); it != qcal.end(); ++it) {
		set<agent> B = *it;
		if (includes(qmp.begin(), qmp.end(), B.begin(), B.end())) res.insert(B);
	}

	for (set<set<agent> >::const_iterator itp = pcal.begin(); itp != pcal.end(); ++itp) {
		for (set<set<agent> >::const_iterator itq = qcal.begin(); itq != qcal.end(); ++itq) {
			set<agent> A = *itp, B = *itq, AiB;
			set_intersection(A.begin(), A.end(), B.begin(), B.end(), inserter(AiB, AiB.begin()));
			if (!AiB.empty()) {
				set<agent> AuB;
				set_union(A.begin(), A.end(), B.begin(), B.end(), inserter(AuB, AuB.begin()));
				res.insert(AuB);
			}
		}
	}
}

void cal(const set<set<agent> > &scal, const set<agent> &s, set<set<agent> > &res) {

	for (set<set<agent> >::const_iterator it = scal.begin(); it != scal.end(); ++it) {
		set<agent> C = *it, i;
		set_intersection(C.begin(), C.end(), s.begin(), s.end(), inserter(i, i.begin()));
		res.insert(i);
	}
}
