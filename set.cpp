#include "set.h"

void U(const set<set<agent> > &pcal, const set<agent> &p, const set<set<agent> > &qcal, const set<agent> &q,
       set<set<agent> > &res) {

	// P \ Q
	set<agent> pmq;
	SETOP(set_difference, p, q, pmq);

	// Q \ P
	set<agent> qmp;
	SETOP(set_difference, q, p, qmp);

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
			SETOP(set_intersection, A, B, AiB);
			if (!AiB.empty()) {
				set<agent> AuB;
				SETOP(set_union, A, B, AuB);
				res.insert(AuB);
			}
		}
	}
}

void cal(const set<set<agent> > &scal, const set<agent> &s, set<set<agent> > &res) {

	for (set<set<agent> >::const_iterator it = scal.begin(); it != scal.end(); ++it) {
		set<agent> C = *it, i;
		SETOP(set_intersection, C, s, i);
		if (!i.empty()) res.insert(i);
	}
}
