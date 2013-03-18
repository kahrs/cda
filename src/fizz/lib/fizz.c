#ifdef PLAN9
#include <u.h>
#include <libc.h>
#endif
#ifndef PLAN9
#include <stdlib.h>
#include <string.h>
#endif
#include "fizz.h"

extern void chkchip(char *), chkpkg(char *), chktype(char *);
void initformat(void), syminit(void), rectinit(void);

void
fizzinit(void)
{
	initformat();
	pininit();
	syminit();
	rectinit();
}

int
fizzplace(void)
{
	extern f_nunplaced;

	/* do regions */
	symtraverse(S_PACKAGE, chkpkg);
	if(f_nerrs)
		return(-1);
	symtraverse(S_TYPE, chktype);
	if(f_nerrs)
		return(-1);
	f_nunplaced = 0;
	symtraverse(S_CHIP, chkchip);
	if(f_nerrs)
		return(-1);
	return(f_nunplaced);
}
