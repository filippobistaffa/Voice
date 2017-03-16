#ifndef CFSS_H_
#define CFSS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instance.h"
#include "params.h"
#include "macros.h"
#include "types.h"

#define CONTAINS(V, I) ((V)[I] <= (V)[N] + N)

void maskagents(const set<agent> &s, stack *st);

void cfss(stack *st);

#ifdef DEBUG
#include "io.h"
#endif

#endif /* CFSS_H_ */
