/*
    module: gasp.c

    The "last_gasp" heuristic computes the reduction of each cube in
    the cover (without replacement) and then performs an expansion of
    these cubes.  The cubes which expand to cover some other cube are
    added to the original cover and irredundant finds a minimal subset.

    If one of the reduced cubes expands to cover some other reduced
    cube, then the new prime thus generated is a candidate for reducing
    the size of the cover.

    The expansion expands all cubes (even those that get covered), and
    returns only those cubes which succeed in covering some other cube.
*/
#include "espresso.h"

/* reduce_gasp -- return a cover of the reduction of each cube of F */
pcover reduce_gasp(F, D)
pcover F, D;
{
    pcube p, last, cunder, *FD;
    pcover G = new_cover(F->count);

    /* Reduce cubes of F without replacement -- save those that reduce */
    FD = cube2list(F, D);
    foreach_set(F, last, p) {
	cunder = reduce_cube(FD, p);
        if (! setp_equal(cunder, p))
            G = sf_addset(G, cunder);
	free_cube(cunder);
    }
    free_cubelist(FD);
    return G;
}


/* expand_gasp -- re-expand the set of cubes which was reduceable */
pcover expand_gasp(G, R)
pcover G, R;
{    return sf_dupl(expand(G, R, TRUE, FALSE));}


/* irred_gasp -- Add new primes to F and find an irredundant subset */
pcover irred_gasp(F, D, G)
pcover F, D, G;			/* G is disposed of */
{
    if (G->count != 0)
        F = irredundant(sf_append(F, G), D);
    else
	free_cover(G);
    return F;
}
