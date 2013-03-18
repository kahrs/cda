/*
    module: compl.c
    purpose: compute the complement of a multiple-valued function

    The "unate recursive paradigm" is used.  After a set of special cases
    are examined, the function is split on the "most active variable".
    These two halves are complemented recursively, and then the results
    are merged.
*/

#include "espresso.h"

static compl_level = 0;


/* complement -- compute the complement of T */
pcover complement(T)
INOUT pcube *T;		/* T will be disposed of */
{
    pcover Tbar;
    register pcube cl, cr;
    register int best;

    if (debug & COMPL)
	debug_print(T, "COMPLEMENT", compl_level++);

    if (compl_special_cases(T, &Tbar) == MAYBE) {
	if (cdata.vars_unate == cdata.vars_active)
	    best = unate_split_select(T, cl=new_cube(), cr=new_cube(), COMPL);
	else
	    best = binate_split_select(T,cl=new_cube(), cr=new_cube(), COMPL);
	Tbar = compl_merge(
		   complement(scofactor(T, cl, best)),
		   complement(scofactor(T, cr, best)), 
		   cl, cr, best);
	free_cubelist(T);
    }

    if (debug & COMPL)
	debug1_print(Tbar, "exit COMPLEMENT", --compl_level);
    return Tbar;
}
bool compl_special_cases(T, Tbar)
INOUT pcube *T;			/* will be disposed if answer is determined */
OUT pcover *Tbar;		/* returned only if answer determined */
{
    register pcube *T1, p, cof=T[0], temp=cube.temp[0], ceil=cube.temp[1];
    pcover ceil_compl;

    /* Check for no cubes in the cover */
    if (T[2] == NULL) {
	*Tbar = sf_addset(new_cover(1), cube.fullset);
	goto exit1;
    }

    /* Check for only a single cube in the cover */
    if (T[3] == NULL) {
	*Tbar = compl_cube(set_or(temp, cof, T[2]));
	goto exit1;
    }

    /* Check for a row of all 1's (implies complement is null) */
    for(T1 = T+2; (p = *T1++) != NULL; )
	if (full_row(p, cof)) {
	    *Tbar = new_cover(0);
	    goto exit1;
	}
	
    /* Check for a column of all 0's which can be factored out */
    INLINEset_copy(ceil, cof);
    for(T1 = T+2; (p = *T1++) != NULL; )
	INLINEset_or(ceil, ceil, p);
    if (! setp_equal(ceil, cube.fullset)) {
	/* Factor out the cube "ceil" */
	set_or(T[0], T[0], set_diff(temp, cube.fullset, ceil));
	ceil_compl = compl_cube(ceil);
	*Tbar = sf_append(complement(T), ceil_compl);
	return TRUE;
    }

    /* Collect column counts, determine unate variables, etc. */
    massive_count(T);

    /* If single active variable not factored out above, then tautology ! */
    if (cdata.vars_active == 1) {
	*Tbar = new_cover(0);
	goto exit1;
    }
	
    /* Not much we can do about it */
    return MAYBE;

exit1:
    free_cubelist(T);
    return TRUE;
}
/*
    compl_merge -- merge the two cofactors around the splitting variable

    The merge operation involves intersecting each cube of the left
    cofactor with cl, and intersecting each cube of the right cofactor
    with cr.  The union of these two covers is the merged result.

    In order to reduce the number of cubes, a distance-1 merge is
    performed (note that two cubes can only combine distance-1 in the
    splitting variable).  Also, a simple expand is performed in the
    splitting variable (simple implies the covering check for the
    expansion is not full containment, but single-cube containment).
*/

pcover compl_merge(L, R, cl, cr, var)
IN pcover L, R;
IN register pcube cl, cr;
IN int var;
{
    register pcube p, last, pt;
    register pcover Tbar;
    pcube *L1, *R1;

    /* Intersect each cube with the cofactored cube */
    foreach_set(L, last, p)
	INLINEset_and(p, p, cl);
    foreach_set(R, last, p) {
	INLINEset_and(p, p, cr);
	SET(p, ACTIVE);
    }

    /* Sort the arrays for a distance-1 merge */
    set_copy(cube.temp[0], cube.var_mask[var]);
    qsort((char *) (L1 = sf_list(L)), L->count, sizeof(pset), d1_order);
    qsort((char *) (R1 = sf_list(R)), R->count, sizeof(pset), d1_order);

    compl_d1merge(L1, R1);
    compl_lift(L1, R1, cr, var);
    compl_lift(R1, L1, cl, var);
    mem_free((char *) L1); 
    mem_free((char *) R1);

    /* Re-create the merged cover */
    Tbar = new_cover(L->count + R->count);
    pt = Tbar->data;
    foreach_set(L, last, p) {
	INLINEset_copy(pt, p);
	Tbar->count++;
	pt += Tbar->wsize;
    }
    foreach_active_set(R, last, p) {
	INLINEset_copy(pt, p);
	Tbar->count++;
	pt += Tbar->wsize;
    }
	
    free_cover(L); 
    free_cover(R);
    free_cube(cl); 
    free_cube(cr);
    return Tbar;
}


/*
    compl_d1merge -- distance-1 merge in the splitting variable
*/
void compl_d1merge(L1, R1)
IN register pcube *L1, *R1;
{
    register pcube pl, pr;

    /* Find equal cubes between the two cofactors */
    for(pl = *L1, pr = *R1; (pl != NULL) && (pr != NULL); )
	switch (d1_order(L1, R1)) {
	    case 1:
		pr = *(++R1); break;		/* advance right pointer */
	    case -1:
		pl = *(++L1); break;		/* advance left pointer */
	    case 0:
		RESET(pr, ACTIVE);
		INLINEset_or(pl, pl, pr);
		pr = *(++R1);
	}
}


/*
    compl_lift -- simple expand in the splitting variable
*/
void compl_lift(A1, B1, bcube, var)
IN pcube *A1, *B1, bcube;
IN int var;
{
    register pcube a, b, *B2, lift = cube.temp[4], liftor = cube.temp[5];
    pcube mask = cube.var_mask[var];

    (void) set_and(liftor, bcube, mask);

    /* for each cube in the first array ... */
    for(; (a = *A1++) != NULL; )
	if (TESTP(a, ACTIVE)) {

	    /* create a lift of this cube in the merging coord */
	    (void) set_merge(lift, bcube, a, mask);

 	    /* for each cube in the second array */
	    for(B2 = B1; (b = *B2++) != NULL; ) {
		INLINEsetp_implies(lift, b, /* when_false => */ continue);

		/* cube of A1 was contained by some cube of B1 */
		INLINEset_or(a, a, liftor);
		break;
	    }
	}
}


/* compl_cube -- return the complement of a single cube (De Morgan's law) */
pcover compl_cube(p)
register pcube p;
{
    register int var;
    register pcube temp = cube.temp[7], pdest, mask;
    pcover R = new_cover(cube.num_vars);

    for(var = 0; var < cube.num_vars; var++) {
	mask = cube.var_mask[var];
	if (! setp_implies(mask, p)) {
	    pdest = GETSET(R, R->count++);
	    INLINEset_and(temp, p, mask);
	    INLINEset_diff(pdest, cube.fullset, temp);
	}
    }
    return R;
}
