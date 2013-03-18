/*
    module: reduce.c
    purpose: Perform the Espresso-II reduction step
*/

#include "espresso.h"

static int sccc_level = 0;
static bool toggle = TRUE;

/*
    Reduction is a technique used to explore larger regions of the
    optimization space.  We replace each cube of F with a smaller
    cube while still maintaining a cover of the same logic function.

    This operation is inherently cube-order dependent.  The heuristic
    of reducing the cubes in order by size is used.

    The real workhorse of this section is the routine SCCC which is
    used to find the Smallest Cube Containing the Complement of a cover.
    Reduction as proposed by Espresso-II takes a cube and computes its
    maximal reduction as the intersection between the cube and the
    smallest cube containing the complement of (F u D - {c}) cofactored
    against c.

    As usual, the unate-recursive paradigm is used to compute SCCC.
    The SCCC of a unate cover is trivial to compute, and thus we perform
    shannon cofactor expansion attempting to drive the cover to be unate
    as fast as possible.
*/
/*
    reduce -- replace each cube in F with its reduction

    The reduction of a cube is the smallest cube contained in
    the cube which can replace the cube in the original cover without
    changing the cover.  This can be computed directly.  The problem is
    that the order in which the cubes are reduced can greatly affect the
    final result.
*/

pcover reduce(F, D)
INOUT pcover F;
IN pcover D;
{
    register pcube last, p, cunder, *FD;

    /* Order the cubes into descending order of size */
    sf_active(F);
    F = toggle ? order_reduce(F) : mini_order(F, descend);
    toggle = ! toggle;

    /* Try to reduce each cube */
    FD = cube2list(F, D);
    foreach_set(F, last, p) {
	cunder = reduce_cube(FD, p);
 	if (! setp_equal(cunder, p)) {
	    if (debug & REDUCE)
		printf("REDUCE: %s to %s\n", pc1(p), pc2(cunder));
	    set_copy(p, cunder);
	    if (setp_empty(cunder))
		RESET(p, ACTIVE);
	}
	free_cube(cunder);
    }
    free_cubelist(FD);

    /* Delete any cubes of F which reduced to the empty cube */
    F = sf_inactive(F);
    return F;
}


/* reduce_cube -- find the maximal reduction of a cube */
pcube reduce_cube(FD, p)
IN pcube *FD, p;
{
    pcube cunder;
    cunder = sccc(cofactor(FD, p));
    (void) set_and(cunder, cunder, p);
    RESET(cunder, PRIME);
    SET(cunder, ACTIVE);
    return cunder;
}
/* sccc -- find Smallest Cube Containing the Complement of a cover */
pcube sccc(T)
INOUT pcube *T;		/* T will be disposed of */
{
    pcube r;
    register pcube cl, cr;
    register int best;

    if (debug & REDUCE1)
	debug_print(T, "SCCC", sccc_level++);

    if (sccc_special_cases(T, &r) == MAYBE) {
	best=binate_split_select(T,cl = new_cube(), cr = new_cube(), REDUCE1);
	r = sccc_merge(
	        sccc(scofactor(T, cl, best)),
		sccc(scofactor(T, cr, best)), 
		cl, cr);
	free_cubelist(T);
    }

    if (debug & REDUCE1)
	printf("SCCC[%d]: result is %s\n", --sccc_level, pc1(r));
    return r;
}

pcover order_reduce(T)
IN pcover T;
{
    register pcube p, last, largest = NULL;
    register int bestsize = -1, size, n = cube.num_vars;
    pcube *T1;

    if (T->count == 0)
	return T;

    /* find largest cube */
    foreach_set(T, last, p)   
	if ((size = set_ord(p)) > bestsize)
	    largest = p, bestsize = size;

    foreach_set(T, last, p)
	PUTSIZE(p, ((n - cdist(largest,p)) << 7) + min(set_ord(p),127));

    T1 = sf_list(T);
    qsort((char *) T1, T->count, sizeof(pset), descend);
    T = sf_unlist(T1, T->count, T->sf_size);
    return T;
}
bool sccc_special_cases(T, result)
INOUT pcube *T;			/* will be disposed if answer is determined */
OUT pcube *result;		/* returned only if answer determined */
{
    register pcube *T1, p, temp = cube.temp[1], ceil, cof = T[0];

    /* empty cover => complement is universe => SCCC is universe */
    if (T[2] == NULL) {
	*result = set_save(cube.fullset); 
	goto exit1;
    }

    /* row of 1's => complement is empty => SCCC is empty */
    for(T1 = T+2; (p = *T1++) != NULL; )
	if (full_row(p, cof)) {
	    *result = new_cube();
	    goto exit1;
	}

    /* Collect column counts, determine unate variables, etc. */
    massive_count(T);

    /* If cover is unate or single cube, apply simple rules to find SCCCU */
    if (cdata.vars_unate == cdata.vars_active || T[3] == NULL) {
	*result = set_save(cube.fullset);
	for(T1 = T+2; (p = *T1++) != NULL; )
	    (void) sccc_cube(*result, set_or(temp, p, cof));
	goto exit1;	
    }

    /* Check for a cube which can be easily factored */
    set_copy(ceil = new_cube(), cof);
    for(T1 = T+2; (p = *T1++) != NULL; )
	INLINEset_or(ceil, ceil, p);
    if (! setp_equal(ceil, cube.fullset)) {
	*result = sccc_cube(set_save(cube.fullset), ceil);
	if (setp_equal(*result, cube.fullset))
	    free_cube(ceil);
	else
	    *result=sccc_merge(sccc(cofactor(T,ceil)), set_save(cube.fullset),
			ceil, *result);
	goto exit1;
    }
    free_cube(ceil);

    /* Single active column at this point => tautology => SCCC is empty */
    if (cdata.vars_active == 1) {
	*result = new_cube();
	goto exit1;
    }

    /* Not much we can do about it */
    return MAYBE;

exit1:
    free_cubelist(T);
    return TRUE;
}
pcube sccc_merge(left, right, cl, cr)
INOUT register pcube left, right;	/* will be disposed of ... */
INOUT register pcube cl, cr;		/* will be disposed of ... */
{
    INLINEset_and(left, left, cl);
    INLINEset_and(right, right, cr);
    INLINEset_or(left, left, right);
    free_cube(right);
    free_cube(cl);
    free_cube(cr);
    return left;
}


/*
    sccc_cube -- find the smallest cube containing the complement of a cube

    It is simple to see that the SCCC is the universe if the cube has more
    than two active variables.  If there is only a single active variable,
    then the SCCC is merely the complement of the cube.  A last special case
    is no active variables, in which case the SCCC is empty.

    This is "anded" with the incoming cube result.
*/
pcube sccc_cube(result, p)
register pcube result, p;
{
    register pcube temp = cube.temp[0];
    register int var, nactive = 0, active = -1;

    for(var = 0; var < cube.num_vars; var++)
	if (! setp_implies(cube.var_mask[var], p))
	    if (++nactive > 1)
		break;
	    else 
		active = var;

    if (nactive == 1) {
        INLINEset_and(temp, cube.var_mask[active], p);
	INLINEset_diff(temp, cube.fullset, temp);
	INLINEset_and(result, result, temp);
    }
    return result;
}
/*
    mv_reduce -- perform an "optimal" reduction of the variables which
    we desire to be sparse

    This could be done using "reduce" and then saving just the desired
    part of the reduction.  Instead, this version uses IRRED to find
    which cubes of an output are redundant.  Note that this gets around
    the cube-ordering problem.

    In normal use, it is expected that the cover is irredundant and
    hence no cubes will be reduced to the empty cube (however, this is
    checked for and such cubes will be deleted)
*/

pcover mv_reduce(F, D)
pcover F, D;
{
    register int i, var;
    register pcube p, *T1, cof;
    pcube *F1, *D1, *F2, *D2, last;
    cof = new_cube();

    F1 = cube1list(F);
    D1 = cube1list(D);

    /* loop for each multiple-valued variable */
    for(var = 0; var < cube.num_vars; var++)
	if (cube.sparse[var])

	/* loop for each part of the variable */
	for(i = cube.first_part[var]; i <= cube.last_part[var]; i++) {

	    /* Create the cube to cofactor against */
 	    set_diff(cof, cube.fullset, cube.var_mask[var]);
	    set_insert(cof, i);
	    F2 = scofactor(F1, cof, var);
	    D2 = scofactor(D1, cof, var);
	    irred(F2, D2);

	    set_copy(cof, cube.fullset);
	    set_remove(cof, i);
	    for(T1 = F2 + 2; (p = *T1++); )
		if (! TESTP(p, ACTIVE))
		    if (var == cube.num_vars-1 ||
			 ! setp_implies(cube.var_mask[var], p)) {
		    INLINEset_and(p, p, cof);
		    RESET(p, PRIME);
		}

	    free_cubelist(F2);
	    free_cubelist(D2);
	}
    free_cube(cof);

    /* Check if any cubes disappeared */
    sf_active(F);
    for(var = 0; var < cube.num_vars; var++)
	if (cube.sparse[var])
	    foreach_active_set(F, last, p)
		if (setp_disjoint(p, cube.var_mask[var]))
		    RESET(p, ACTIVE), F->active_count--;
    if (F->count != F->active_count)
	F = sf_inactive(F);
    return F;
}
