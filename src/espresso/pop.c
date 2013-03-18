#include "espresso.h"

bool pop_expand(Fin, D)
INOUT pcover *Fin;
IN pcover D;
{
    bool change, expanded = FALSE, removed = FALSE;
    pcover F = *Fin;
    pcube *FD;
    register pcube p, p1, last, newp;
    register int i, n = cube.num_binary_vars - 1;
    newp = new_cube();

    /* Use cube-size heuristic to order the cubes for expansion */
    sf_active(F = size_sort(F));

    /* Attempt to expand each nonprime and noncovered cube */
    FD = cube2list(F, D);
    foreach_set(F, last, p)
	if (TESTP(p, ACTIVE) && ! TESTP(p, PRIME)) {
	    change = FALSE;
	    set_copy(newp, p);
	    for(i = 0; i <= cube.last_part[n]; i++)
		if (! is_in_set(newp, i)) {
		    set_insert(newp, i);
		    if (cube_is_covered(FD, newp))
			change = TRUE;
		    else
			set_remove(newp, i);
		}
	    if (change) {
		if (debug & EXPAND)
		    printf("EXPAND: %s to %s\n", pc1(p), pc2(newp));
		set_copy(p, newp);
		for(p1 = p + F->wsize; p1 < last; p1 += F->wsize)
		    if (setp_implies(p1, p)) {
			if (debug & EXPAND)
			    printf("deleted %s\n", pc1(p1));
			removed = TRUE, F->active_count--, RESET(p1, ACTIVE);
		    }
		expanded = TRUE;
	    }
	    SET(p, PRIME);
	}
    free_cubelist(FD);

    /* Delete any cubes of F which became covered during the expansion */
    if (removed)
	F = sf_inactive(F);

    free_cube(newp);
    *Fin = F;
    return expanded;
}

bool pop_reduce(Fin, D)
INOUT pcover *Fin;
IN pcover D;
{
    bool change, reduced = FALSE, removed = FALSE;
    pcover F = *Fin;
    pcube *FD;
    register int i, n = cube.num_vars - 1;
    register pcube p, last, newp = cube.temp[7], temp = cube.temp[8];

    /* Use cube-size heuristic to order the cubes for reduction */
    sf_active(F = size_sort(F));

    /* Attempt to reduce each cube of F */
    FD = cube2list(F, D);
    foreach_set(F, last, p) {
	change = FALSE;
	set_copy(newp, p);
	for(i = cube.first_part[n]; i <= cube.last_part[n]; i++)
	    if (is_in_set(p, i)) {
		set_remove(p, i);
		if (cube_is_covered(FD, newp))
		    change = TRUE;
	 	else
		    set_insert(p, i);
	    }
	if (change) {
	    if (debug & REDUCE)
		printf("POP_REDUCE: %s reduced to %s\n", pc1(newp), pc2(p));
	    reduced = TRUE;
	    RESET(p, PRIME);
	    if (setp_empty(set_and(temp, p, cube.var_mask[n])))
		removed = TRUE, F->active_count--, RESET(p, ACTIVE);
	}
    }
    free_cubelist(FD);

    /* Delete any cubes of F which disappeared during the reduction */
    if (removed)
	F = sf_inactive(F);
    *Fin = F;
    return reduced;
}
