#ifdef PLAN9
#include <u.h>
#include <libc.h>
#endif
#ifndef PLAN9
#include <stdlib.h>
#include <string.h>
#endif
#include "fizz.h"

Signal *maxsig;

void
netlen(Signal *s)
{
	if((s->type == NORMSIG) || (s->type == SUBVSIG))
		if((maxsig == (Signal *) 0) || (s->n > maxsig->n))
			maxsig = s;
}
