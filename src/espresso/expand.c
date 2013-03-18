/*
    module: expand.c
    purpose: Perform the Espresso-II Expansion Step

    The idea is to take each nonprime cube of the on-set and expand it
    into a prime implicant such that we can cover as many other cubes
    of the on-set.  If no cube of the on-set can be covered, then we
    expand each cube into a large prime implicant by transforming the
    problem into a minimum covering problem which is solved by the
    heuristics of minimum_cover.

    These routines revolve around having a representation of the
    OFF-set.  (In contrast to the Espresso-II manuscript, we do NOT
    require an "unwrapped" version of the OFF-set).  The main idea is
    the concept of distance between two cubes extended to the general
    multiple-valued case (and the output part is treated as a multiple-
    valued input).
*/

#include "espresso.h"
#define HACK

static int num_covered;
static pcube ESSEN_LOWER, LOWER, RAISE, XFREE, INIT_LOWER;
static pcover BB, CC;
/*
    expand -- expand each nonprime cube of F into a prime implicant

    If gasp is true, a cube is saved only if it expands to cover another
    cube; if it cannot cover another cube, expansion is stopped and the
    cube is not returned in the result

    If nonsparse is true, only the non-sparse variables will be expanded;
    this is done by forcing all of the sparse variables into the lowering
    set.
*/

pcover expand(F, R, gasp, nonsparse)
INOUT pcover F;
IN pcover R;
IN bool gasp;			/* perform gasp expansion strategy */
IN bool nonsparse;		/* expand non-sparse variables only */
{
    int var;
    bool change;
    register pcube last, p;

    /* Order the cubes according to "chewing-away from the edges" of mini */
    F = mini_order(F, ascend);

    /* Setup the global variables for these routines */
    ESSEN_LOWER = new_cube(); INIT_LOWER = new_cube();
    LOWER = new_cube(); XFREE = new_cube();
    BB = R; CC = F;

    /* Setup the initial lowering set (differs for nonsparse) */
    (void) set_copy(INIT_LOWER, cube.emptyset);
    if (nonsparse)
	for(var = 0; var < cube.num_vars; var++)
	    if (cube.sparse[var])
		(void) set_or(INIT_LOWER, INIT_LOWER, cube.var_mask[var]);

    /* Mark all cubes as not covered, and maybe essential */
    foreach_set(F, last, p)
	RESET(p, COVERED), RESET(p, NONESSEN);

    /* Try to expand each nonprime and noncovered cube */
    foreach_set(F, last, p)
	/* do not expand if PRIME or (COVERED and not gasp) */
	if (! (TESTP(p, PRIME) || (! gasp && TESTP(p, COVERED)))) {
	    expand1(p, gasp);
	    if (debug & EXPAND)
		printf("EXPAND: %s (covered %d)\n", pc1(p), num_covered);

	    /* See if any cubes were covered */
	    if (num_covered == 0) {
		/* If gasping, don't save it (it didn't cover anything) */
		if (gasp)
		    SET(p, COVERED);
		/* If not equal to overexpanded cube than nonessential */
		if (! setp_equal(LOWER, ESSEN_LOWER))
		    SET(p, NONESSEN);
	    }
	}

    /* Delete any cubes of F which became covered during the expansion */
    F->active_count = 0;
    change = FALSE;
    foreach_set(F, last, p)
	if (TESTP(p, COVERED)) {
	    RESET(p, ACTIVE);
	    change = TRUE;
	} else {
	    SET(p, ACTIVE);
	    F->active_count++;
  	}
    if (change)
	F = sf_inactive(F);
    free_cube(ESSEN_LOWER); free_cube(LOWER);
    free_cube(XFREE); free_cube(INIT_LOWER);
    return F;
}
/*
    expand1 -- Expand a single cube against the OFF-set
*/
void expand1(c, gasp)
INOUT pcube c;
IN bool gasp;
{
    int bestindex;

    if (debug & EXPAND1)
	printf("\nEXPAND1: cube is %s\n", pc1(c));

    /* Initialize the lowering, raising and unassigned sets */
    RAISE = c;
    if (! setp_empty(set_copy(LOWER, INIT_LOWER))) {
	(void) set_diff(LOWER, LOWER, c);
	elim_lowering();
    }
    set_diff(XFREE, cube.fullset, set_or(XFREE, RAISE, LOWER));

    /* initialize BB, and CC */
    SET(c, PRIME);
    setup_BB_CC();
    num_covered = 0;

    /* Determine what can and can't be raised, and save the lowering set */
    (void) set_copy(ESSEN_LOWER, essen_parts());

    /* While there are still cubes which can be covered, cover them ! */
    while (select_feasible() != (pcube) NULL)
	(void) essen_parts();
    if (gasp && num_covered == 0)
	return;		/* quit early if unsuccessful gasp expansion */

    /* While there are still cubes covered by the overexpanded cube ... */
    while (CC->active_count > 0) {
	bestindex = most_frequent();
	(void) set_insert(RAISE, bestindex);
	(void) set_diff(XFREE, XFREE, RAISE);
	(void) essen_parts();
    }

    /* Finally, when all else fails, choose the largest possible prime */
    while (BB->active_count > 0)
	mincov();
}
/*
    essen_parts -- determine which parts are forced into the lowering
    set to insure that the cube be orthognal to the OFF-set.

    General rule: if any cube of the OFF-set is distance 1 from the
    raising cube, then we must lower all parts of the conflicting
    variable.  (If the cube is distance 0, we detect this error here.)

    If there are essentially lowered parts, we can remove from consideration
    any cubes of the OFF-set which are more than distance 1 from the 
    overexpanded cube of RAISE.

    After eliminating any such cubes of the off-set, we may then have
    essentially raised parts which can be automatically added to the
    raising set.
*/
pset essen_parts()
{
    register pcube last, p, xlower = cube.temp[0];

    (void) set_copy(xlower, cube.emptyset);
    foreach_active_set(BB, last, p)
	switch (cdist01(p, RAISE)) {
	    case 0:
		fatal("espresso: ON-set and OFF-set are not orthogonal");
	    case 1:
		(void) force_lower(xlower, p, RAISE);
		BB->active_count--;
		RESET(p, ACTIVE);
	}
    if (debug & EXPAND1)
        printf("ESSEN_LOWERING: xlower is: %s\n", pc1(xlower));

    (void) set_or(LOWER, LOWER, xlower);	/* add to lowering set */
    (void) set_diff(XFREE, XFREE, xlower);	/* remove from free set */

    if (! setp_empty(xlower)) {
	elim_lowering();
	essen_raising();
    }
    return xlower;
}


/*
    essen_raising -- determine which parts may always be added to
    the raising set without restricting further expansions

    General rule: if some part is not blocked by any cube of BB, then
    this part can always be raised.
*/
void essen_raising()
{
    register pcube last, p, xraise = cube.temp[1];

    /* Form union of all cubes of BB, and then take complement wrt XFREE */
    INLINEset_copy(xraise, cube.emptyset);
    foreach_active_set(BB, last, p)
        INLINEset_or(xraise, xraise, p);
    INLINEset_diff(xraise, XFREE, xraise);
    if (debug & EXPAND1)
        printf("ESSEN_RAISING: xraise is: %s\n", pc1(xraise));

    INLINEset_or(RAISE, RAISE, xraise);		/* add to raising set */
    INLINEset_diff(XFREE, XFREE, xraise);	/* remove from free set */
}
/*
    elim_lowering -- after adding parts to the lowering set (LOWER), we
    can reduce the size of both BB and CC.  BB can be reduced by those
    cubes of the off-set which are orthogonal to the expanding cube, and
    CC can be reduced by those cubes of the on-set which cannot be covered
    by the expanding cube.
*/
void elim_lowering()
{
    register pcube p, praise = cube.temp[2];
    pcube last;

    /* Potentially raised set (raising all remaining free coordinates) */
    INLINEset_or(praise, RAISE, XFREE);

    /* Remove sets of BB which are orthogonal to future expansions */
    foreach_active_set(BB, last, p) {
#ifndef HACK
	if (! cdist0(p, praise))
#else
 {register int w,lastw;register unsigned int x;
if((lastw=cube.inword)!=-1){x=p[lastw]&praise[lastw];if(~(x|x>>1)&cube.inmask)
goto false;for(w=1;w<lastw;w++){x=p[w]&praise[w];if(~(x|x>>1)&DISJOINT)goto
false;}}}{register int w,var,lastw;register pcube mask;for(var=cube.
num_binary_vars;var<cube.num_vars;var++){mask=cube.var_mask[var];lastw=
cube.last_word[var];for(w=cube.first_word[var];w<=lastw;w++)if(p[w]&praise[w]&
mask[w])goto nextvar;goto false;nextvar:;}}continue;
 false:
#endif
	    BB->active_count--, RESET(p, ACTIVE);
    }

    /* Remove sets of CC which cannot be covered by future expansions */
    foreach_remaining_set(CC, last, RAISE, p)
#ifndef HACK
	if (TESTP(p, ACTIVE) && ! setp_implies(p, praise)) {
#else
	if (TESTP(p, ACTIVE)) {
	    INLINEsetp_implies(p, praise, goto false1);
	    continue;
	false1:
#endif
	    CC->active_count--, RESET(p, ACTIVE);
	}
}


/*
    feasibly_covered -- determine if the cube represented by the cover
    set c can be feasibly covered (i.e., if it is possible to raise all of
    the necessary variables while still insuring orthogonality with R)

    General rule: if the raising cube would remain distance 1 or more
    from each cube of the OFF-set, then the cubes is feasibly covered.
    Equivalently, if any cube of the OFF-set becomes distance 0 to the
    raising cube, then the cube is not feasibly covered.
*/

bool feasibly_covered(c)
IN pset c;
{
    register pcube p, last, praise = set_or(cube.temp[2], RAISE, c);

    foreach_active_set(BB, last, p) {
#ifndef HACK
	if (cdist0(p, praise))
#else
 {register int w,lastw;register unsigned int x;
if((lastw=cube.inword)!=-1){x=p[lastw]&praise[lastw];if(~(x|x>>1)&cube.inmask)
goto false;for(w=1;w<lastw;w++){x=p[w]&praise[w];if(~(x|x>>1)&DISJOINT)goto
false;}}}{register int w,var,lastw;register pcube mask;for(var=cube.
num_binary_vars;var<cube.num_vars;var++){mask=cube.var_mask[var];lastw=
cube.last_word[var];for(w=cube.first_word[var];w<=lastw;w++)if(p[w]&praise[w]&
mask[w])goto nextvar;goto false;nextvar:;}}
#endif

	    return FALSE;
    false: ;
    }
    return TRUE;
}


/*
    setup_BB_CC -- set up the blocking and covering set families;

    Note that the blocking family is merely the set of cubes of R, and
    that CC is the set of cubes of F which might possibly be covered
    (i.e., nonprime cubes, and cubes not already covered)
*/

void setup_BB_CC()
{
    register pcube p, last;

    /* Create the block and cover set families */
    BB->active_count = BB->count;
    foreach_set(BB, last, p)
	SET(p, ACTIVE);

    CC->active_count = CC->count;
    foreach_set(CC, last, p)
	if (TESTP(p, COVERED) || TESTP(p, PRIME))
	    CC->active_count--, RESET(p, ACTIVE);
	else
	    SET(p, ACTIVE);
}
/*
    select_feasible -- Determine if there are cubes which can be covered,
    and if so, raise those parts necessary to cover as many as possible.
*/

pcube select_feasible()
{
    register pcube p, last, bestfeas, *feas;
    register int i, j;
    int bestcover, bestsize;
    int numfeas, *feas_covers, *feas_size;

    /* Find all of the feasibly covered cubes */
    feas = (pcube *) alloc(CC->active_count * sizeof(pcube));
    numfeas = 0;
    foreach_remaining_set(CC, last, RAISE, p)
	if (TESTP(p,ACTIVE))
            if (setp_implies(p, RAISE)) {
		num_covered++;
		CC->active_count--;
		RESET(p, ACTIVE);
		SET(p, COVERED);
	    } else if (feasibly_covered(p))
		feas[numfeas++] = p;

    /* Exit here if there are no feasibly covered cubes */
    if (numfeas == 0) {
	mem_free((char *) feas);
	return (pcube) NULL;
    }

    /* Count how many other cubes each feasibly covered cube covers */
    feas_covers = (int *) alloc(numfeas * sizeof(int));
    feas_size = (int *) alloc(numfeas * sizeof(int));
    for(i = 0; i < numfeas; i++) {
        feas_covers[i] = 0;
	feas_size[i] = set_ord(feas[i]);
    }
    for(i = 0; i < numfeas; i++)
        for(j = 0; j < numfeas; j++)
            if (setp_implies(feas[i], feas[j]))
                feas_covers[j]++;

    /* Select the smallest feasibly covered cube that covers the most cubes */
    bestfeas = (pcube) NULL;
    bestcover = bestsize = 0;
    for(i = 0; i < numfeas; i++)
        if (feas_covers[i] > bestcover) {
            bestcover = feas_covers[i];
            bestsize = feas_size[i];
            bestfeas = feas[i];
	} else if (feas_covers[i]==bestcover && feas_size[i]<bestsize) {
            bestsize = feas_size[i];
            bestfeas = feas[i];
        }

    /* Add the necessary parts to the raising set, and see who we covered */
    (void) set_or(RAISE, RAISE, bestfeas);
    (void) set_diff(XFREE, XFREE, RAISE);
    for(i = 0; i < numfeas; i++)
	if (setp_implies(p=feas[i], RAISE)) {
	    num_covered++;
	    CC->active_count--;
	    RESET(p, ACTIVE);
	    SET(p, COVERED);
	}
    if (debug & EXPAND1)
	printf("SELECT_FEASIBLE: can cover %d by covering %s\n",
	    bestcover, pc1(bestfeas));

    mem_free((char *) feas);
    mem_free((char *) feas_covers);
    mem_free((char *) feas_size);
    return bestfeas;
}
/* 
    most_frequent -- When all else fails, select a reasonable part to raise

    The active cubes of CC are the cubes which are covered by the
    overexpanded cube of the original cube (however, we know that none
    of them can actually be covered by a feasible expansion of the
    original cube).  We resort to the MINI strategy of selecting to
    raise the part which will cover the same part in the most cubes of
    CC.
*/

int most_frequent()
{
    register int i, best_variable, best_count, *count;
    register pset p, last;

    /* Count occurences of each variable */
    count = (int *) alloc(cube.size * sizeof(int));
    for(i = 0; i < cube.size; i++)
	count[i] = 0;
    foreach_remaining_set(CC, last, RAISE, p)
	if (TESTP(p, ACTIVE))
	    set_adjcnt(p, count, 1);

    /* Now find which free part occurs most often */
    best_count = best_variable = -1;
    for(i = 0; i < cube.size; i++)
        if (is_in_set(XFREE,i) && count[i] > best_count) {
            best_variable = i;
            best_count = count[i];
        }
    mem_free((char *) count);

    if (debug & EXPAND1)
        printf("MOST_FREQUENT: %d is most frequent\n", best_variable);
    return best_variable;
}
/*
    mincov -- transform the problem of expanding a cube to a maximally-
    large prime implicant into the problem of selecting a minimum
    cardinality cover over a family of sets.

    When we get to this point, we must unravel the remaining off-set.
    This may be painful.
*/

void mincov()
{
    int bound, expansion, nset, var, dist;
    pset_family B;
    register pcube xraise=cube.temp[1], xlower, p, last;

    /* Create B which are those cubes which we must avoid intersecting */
    B = new_cover(BB->active_count);
    foreach_active_set(BB, last, p)
	(void) force_lower(set_copy(GETSET(B, B->count++),cube.emptyset),
			    p, RAISE);

    /* Determine how many sets it will blow up into after the unravel */
    nset = 0;
    foreach_set(B, last, p) {
	expansion = 1;
	for(var = cube.num_binary_vars; var < cube.num_vars; var++)
	    if ((dist=set_dist(p, cube.var_mask[var])) > 1)
		expansion *= dist;
        nset += expansion;
	if (nset > 1000) {	/* oops, too many sets! */
	    sf_free(B);
	    set_insert(RAISE, most_frequent());	/* pick any part */
	    set_diff(XFREE, XFREE, RAISE);
	    (void) essen_parts();
	    return;
	}
    }

    /* Perform a containment to make it minimal */
    B = unravel(B, cube.num_binary_vars);
    foreach_set(B, last, p)
	INLINEset_diff(p, cube.fullset, p);
    B = sf_contain(B);
    foreach_set(B, last, p)
	INLINEset_diff(p, cube.fullset, p);
    xlower = minimum_cover(B, &bound);
    
    /* Add lowered parts to the lowering set */
    (void) set_or(LOWER, xlower, LOWER);

    /* Add any remaining free parts to the raising set */
    (void) set_diff(xraise, XFREE, xlower);
    (void) set_or(RAISE, xraise, RAISE);

    /* free set is now empty */
    (void) set_copy(XFREE, cube.emptyset);
    BB->active_count = 0;

    if (debug & EXPAND1)
	printf("MINCOV: lower=%s, raise=%s\n", pc1(xlower), pc2(xraise));
    sf_free(B);
    set_free(xlower);
}
pcover mini_order(F, compare)
pcover F;
int (*compare)();
{
    register int *count, cnt, n = cube.size, i;
    register pcube p, last;
    pcube *F1;

    /* Perform a column sum over the set family */
    count = sf_count(F);

    /* weight is "inner product of the cube and the column sums" */
    foreach_set(F, last, p) {
	cnt = 0;
	for(i = 0; i < n; i++)
	    if (is_in_set(p, i))
		cnt += count[i];
	PUTSIZE(p, cnt);
    }

    mem_free((char *) count);
    qsort((char *) (F1 = sf_list(F)), F->count, sizeof(pcube), compare);
    F = sf_unlist(F1, F->count, F->sf_size);
    return F;
}
