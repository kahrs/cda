/*
    module: essen.c
    purpose: Find essential primes in a multiple-valued function
*/

#include "espresso.h"

/*
    essential -- return a cover consisting of the cubes of F which are
    essential prime implicants (with respect to F u D); Further, remove
    these cubes from the ON-set F, and add them to the OFF-set D.

    Sometimes EXPAND can determine that a cube is not an essential prime.
    If so, it will set the "NONESSEN" flag in the cube.  Also, IRREDUNDANT
    will mark all cubes in the relatively essential set as "RELESSEN".
    Note that all essential prime implicants must be relatively essential.
*/

pcover essential(Fp, Dp)
IN pcover *Fp, *Dp;
{
    register pcube last, p;
    pcover E, F = *Fp, D = *Dp;

    /* set all cubes in F active */
    sf_active(F);

    /* Loop for all cubes of F */
    E = new_cover(10);
    foreach_set(F, last, p)
	/* Only need to check relative essentials which aren't already known
	to be nonessential primes */
        if (TESTP(p, RELESSEN) && ! TESTP(p, NONESSEN))
	    if (essen_cube(F, D, p)) {
		if (debug & ESSEN)
		    printf("ESSENTIAL: %s\n", pc1(p));
		E = sf_addset(E, p);
		RESET(p, ACTIVE);
		F->active_count--;
	    }

    *Fp = sf_inactive(F);		/* delete the inactive cubes from F */
    *Dp = sf_append(D, sf_save(E));	/* add the inactive cubes to D */
    return E;
}



/*
    essen_cube -- check if a single cube is essential or not
*/
bool essen_cube(F, D, c)
IN pcover F, D;
IN pcube c;
{
    register pcube p, last, pr, *R1;
    register pcover R;
    bool essen;

    /* Form R = consensus of c with each cube of F and D */
    R = new_cover(F->count + D->count);
    pr = R->data;

    foreach_set(F, last, p)
	if (p != c && consensus(pr, p, c))
	    pr += R->wsize, R->count++;
    foreach_set(D, last, p)
	if (consensus(pr, p, c))
	    pr += R->wsize, R->count++;

    R1 = cube1list(R);
    essen = ! cube_is_covered(R1, c);
    free_cubelist(R1);

    free_cover(R);
    return essen;
}
