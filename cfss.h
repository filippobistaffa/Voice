#ifndef CFSS_H_
#define CFSS_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instance.h"
#include "params.h"
#include "macros.h"
#include "types.h"

#define CONTAINS(V, I) ((V)[I] <= (V)[N] + N)

void initstack(stack *st);

void maskagents(const set<agent> &s, stack *st);

void lines1115(stack *st, bool outer);

void lines1721(stack *st);

#ifdef DEBUG
#include "io.h"
#endif

#include "set.h"

#endif /* CFSS_H_ */
