/*
    Following the notation of the Espresso-II monograph, we define:

	ff = an incompletely specified (multiple-valued) Boolean function
        F = ON-SET cover of the function ff
        D = DC-SET cover of the function ff
        E = relatively essential cubes of F
        R = redundant cubes of F
	Rt = totally redundant cubes of F
        Rp = partially redundant cubes of F
        Rc = minimal subset of partial redundant cubes

    A cube c of F belongs to E if "F u D - {c}" doesn't contain all of
    c, or c belongs to R if "F u D - {c}" does contain all of c.  The
    set E are "relatively" essential in that they must remain for F to
    remain a cover.  (They must remain because they cover some minterm
    that no other cube covers.) The cubes of E are not necessarily
    essential primes.  An essential prime must appear in any minimum
    cover of ff, while the cubes of E must appear in any minimum
    subcover of F.  (Note that if F is in fact the set of all primes,
    then E is the essential prime set.)

    A cube c of R belongs to Rt if "E u D" contains all of c, or c
    belongs to Rp if "E u D" doesn't contain all of c.  The cubes of Rt
    are totally redundant in the sense that any minimal subset of F
    will never contain a cube of Rt, and hence the cubes of Rt may be
    removed from F.  (Note that if F is the set of all primes, then Rt
    is the set of primes dominated by the essential prime set).  What
    remains are sets of "cyclic" constraints within Rp.  At least one
    cube of Rp can be removed, but we want to intelligently determine
    exactly which cubes can be removed.

    Consider the following much simpler attempt at irredundant:  pick
    any cube c of F, test if "F u D - c" contains c, and, if so, remove
    c from F.  We then repeat this until no cubes are redundant.  Any
    time during this algorithm that we test a member of E we will find
    the cube cannot be removed.  Any time that we test a member of Rt,
    we will remove it; further, independent of the order that we
    process the cubes, we will always remove every member of Rt.  The
    problem with this simpler algorithm are cubes of Rp.  We will
    remove at least one cube of Rp, but we have no guarantee that a
    good subset of Rp will be removed.
*/

#include "espresso.h"

static int taut_level = 0, ftaut_level = 0, Rp_size;
static pset Rp_start;
/*
   irredundant -- Return a minimal subset of F
*/
pcover irredundant(F, D)
pcover F, D;
{
    pcube *F1, *D1;

    irred(F1 = cube1list(F), D1 = cube1list(D));
    free_cubelist(F1);
    free_cubelist(D1);

    /* Recreate F by deleting inactive cubes */
    return sf_inactive(F);
}


/*
    irred -- mark the redundant cubes of F1 as inactive.
*/
void irred(F1, D1)
pcube *F1, *D1;
{
    register pcube p, *T, *T1;
    int zf, zr=0, ze=0, zrt=0, zrc=0, zrc_best=0;

    zf = CUBELISTSIZE(F1);

    /* Find which cubes of F are essential and which are redundant */
    T = mlist1(F1, D1);		/* all of F and D */
    for(T1 = F1+2; (p = *T1++) != NULL; )
        if (cube_is_covered(T, p)) {
	    /* cube is relatively redundant */
	    zr++;
	    SET(p, ACTIVE);
	    RESET(p, RELESSEN);
        } else {
	    /* cube is relatively essential */
            ze++;
	    RESET(p, ACTIVE);
	    SET(p, RELESSEN);
	}

    /* Re-examine the redundant cubes to see which are totally redundant */
    mlist2(F1, D1, T);		/* essentials of F, and all of D */
    for(T1 = F1+2; (p = *T1++) != NULL; )
        if (TESTP(p, ACTIVE) && cube_is_covered(T, p)) {
	    /* the relatively redundant cube is totally redundant */
	    zrt++;
	    RESET(p, ACTIVE);
	}

    /* Select only a subset of the partially redundant cubes */
    zrc_best = find_minimum(F1, T, &zrc);
    free_cubelist(T);

    if (debug & IRRED)
printf("# IRRED: F=%d, E=%d, R=%d, Rt=%d, Rp=%d, Rc=%d, Final=%d, Bound=%d\n",
	    zf, ze, zr, zrt, zr-zrt, zrc, zrc+ze, zrc_best+ze);
}
/*
    At this point, the cover F1 has been split into three parts:

	1. The essential cubes of F1 are not "ACTIVE" and "RELESSEN"
	2. The totally redundant cubes are not "ACTIVE" and not "RELESSEN"
	3. The partially redundant cubes are "ACTIVE" and not "RELESSEN"

    The partially redundant cubes are the nasty ones.  We want to find
    a maximal subset of them that we can discard.  This is done by
    building a "prime implicant table" using the unate recursive paradigm.
    Then a heuristic minimum cover of this table is selected.  The minimum
    cover algorithm also returns a bound on the size of a minimum cover.
*/
    
int find_minimum(F1, DE, zrc)
IN pcube *F1, *DE;
OUT int *zrc;
{
    register pcube p, last, *T1, *DE1;
    register int i; int bound;
    pset_family table;
    pset cover;

    /* Mark each cube in DE as not part of the redundant set */
    for(DE1 = DE+2; (p = *DE1) != NULL; DE1++)
	RESET(p, REDUND);

    /* Compute an index for each partially redundant cube */
    i = 0;
    for(T1 = F1+2; (p = *T1++) != NULL; )
	if (TESTP(p, ACTIVE)) {
	    PUTSIZE(p, i);		/* save the cube index */
	    SET(p, REDUND);		/* belongs to redundant set */
	    *DE1++ = p;			/* add to the end of DE list */
	    i++;
	}
    *DE1++ = NULL;			/* sentinel */
    DE[1] = (pcube) DE1;		/* save pointer to last in array */
    Rp_size = i;

    /* For each cube in Rp, find ways to cover its minterms */
    table = sf_new(0, Rp_size);
    Rp_start = set_new(Rp_size);
    i = 0;
    for(T1 = F1+2; (p = *T1++) != NULL; )
	if (TESTP(p, ACTIVE)) {
	    set_fill(Rp_start, Rp_size);
	    set_remove(Rp_start, i);
	    table = sf_append(table, fcube_is_covered(DE, p));
	    RESET(p, REDUND);
	    i++;
	}
    table = sf_contain(table);
    set_fill(Rp_start, Rp_size);
    foreach_set(table, last, p)
	INLINEset_diff(p, Rp_start, p);

    /* Find a heuristic minimum cover */
    cover = minimum_cover(table, &bound);

    /* Mark those not in the cover as redundant (hence inactive) */
    i = 0;
    for(T1 = F1+2; (p = *T1++) != NULL; )
	if (TESTP(p, RELESSEN)) 
	    SET(p, ACTIVE);	/* make the relatively essentials active */
	else if (TESTP(p, ACTIVE)) {
	    if (! is_in_set(cover, i))
		RESET(p, ACTIVE);	/* make a redundant cube inactive */
	    i++;
	}

    *zrc = set_ord(cover);
    sf_free(table);
    set_free(cover);
    set_free(Rp_start);
    return bound;
}
/* cube_is_covered -- determine if a cover "covers" a single cube */
bool cube_is_covered(T, c)
IN pcube *T, c;
{    return tautology(cofactor(T,c));}


/* fcube_is_covered -- determine exactly how a cover "covers" a cube */
pset_family fcube_is_covered(T, c)
IN pcube *T, c;
{    return ftautology(cofactor(T,c));}


/* tautology -- answer the tautology question for T */
bool tautology(T)
INOUT pcube *T;		/* T will be disposed of */
{
    register pcube cl, cr;
    register int best, result;

    if (debug & TAUT) 
	debug_print(T, "TAUTOLOGY", taut_level++);

    if ((result = taut_special_cases(T)) == MAYBE) {
	best = binate_split_select(T,cl = new_cube(), cr = new_cube(), TAUT);
	result = tautology(scofactor(T, cl, best)) &&
		 tautology(scofactor(T, cr, best));
	free_cubelist(T); 
	free_cube(cl);
	free_cube(cr);
    }

    if (debug & TAUT)
	printf("exit TAUTOLOGY[%d]: %s\n", --taut_level, print_bool(result));
    return result;
}


/* ftautology -- find ways to make a tautology */
pset_family ftautology(T)
INOUT pcube *T;		/* T will be disposed of */
{
    pset_family result;
    register pcube cl, cr;
    register int best;

    if (debug & FTAUT)
	debug_print(T, "FIND_TAUTOLOGY", ftaut_level++);

    if (ftaut_special_cases(T, &result) == MAYBE) {
	best = binate_split_select(T,cl=new_cube(), cr=new_cube(), FTAUT);
	result = sf_union(
	             ftautology(scofactor(T, cl, best)),
		     ftautology(scofactor(T, cr, best))
		 );
	free_cubelist(T); 
	free_cube(cl); 
	free_cube(cr);
    }

    if (debug & FTAUT)
	sf_debug_print(result, "exit FIND_TAUTOLOGY", --ftaut_level);
    return result;
}
bool taut_special_cases(T)
INOUT pcube *T;			/* will be disposed if answer is determined */
{
    register pcube *T1, *Tsave, p, ceil=cube.temp[0], temp=cube.temp[1];
    int var;

    /* Check for a row of all 1's which implies tautology */
    for(T1 = T+2; (p = *T1++) != NULL; )
	if (full_row(p, T[0])) {
	    free_cubelist(T);
	    return TRUE;
	}

    /* Check for a column of all 0's which implies no tautology */
start:
    INLINEset_copy(ceil, T[0]);
    for(T1 = T+2; (p = *T1++) != NULL; )
	INLINEset_or(ceil, ceil, p);
    if (! setp_equal(ceil, cube.fullset)) {
	free_cubelist(T);
	return FALSE;
    }

    /* Collect column counts, determine unate variables, etc. */
    massive_count(T);

    /* If function is unate (and no row of all 1's), then no tautology */
    if (cdata.vars_unate == cdata.vars_active) {
	free_cubelist(T);
	return FALSE;

    /* If active in a single variable (and no column of 0's) then tautology */
    } else if (cdata.vars_active == 1) {
	free_cubelist(T); 
	return TRUE;

    /* Check for unate variables, and reduce cover if there are any */
    } else if (cdata.vars_unate != 0) {
	/* Form a cube "ceil" with full variables in the unate variables */
	(void) set_copy(ceil, cube.emptyset);
	for(var = 0; var < cube.num_vars; var++)
	    if (cdata.is_unate[var])
		INLINEset_or(ceil, ceil, cube.var_mask[var]);

	/* Save only those cubes that are "full" in all unate variables */
	for(Tsave = T1 = T+2; (p = *T1++) != NULL; )
	    if (setp_implies(ceil, set_or(temp, p, T[0])))
		*Tsave++ = p;
  	*Tsave++ = (pcube) NULL;
	T[1] = (pcube) Tsave;

	if (debug & TAUT)
	    printf("UNATE_REDUCTION: %d unate variables, reduced to %d\n",
		cdata.vars_unate, CUBELISTSIZE(T));
	goto start;

    /* Check for possibility of component reduction */
    } else if (cdata.var_zeros[cdata.best] < 0.4*CUBELISTSIZE(T))
	return taut_component_reduction(T);

    /* We tried as hard as we could, but must recurse from here on */
    return MAYBE;
}
/*
    taut_component_reduction -- decide if it's best to perform component 
    reduction and either return MAYBE if it's not possible, or return
    the result of the tautology check after the reduction
*/
bool taut_component_reduction(T)
pcube *T;
{
    int change, comp_size = 0, numcube;
    register pcube p, *TL, *TL1, *T1, seed = cube.temp[0];
    pcube temp1 = cube.temp[6], cof = T[0];

    numcube = CUBELISTSIZE(T);

    /* Mark all cubes -- uncovered cubes belong to right partition */
    for(T1 = T+2; (p = *T1++) != NULL; )
	RESET(p, COVERED);

    /* Pick up cubes common to seed and add to the left partition */
    set_or(seed, cof, largest_cube(T));
    do {
	change = FALSE;
	for(T1 = T+2; (p = *T1++) != NULL; )
	    if (! TESTP(p, COVERED) && ccommon(set_or(temp1, p, cof), seed)) {
		SET(p, COVERED);
		INLINEset_and(seed, seed, temp1);
		change = TRUE; comp_size++;
	    }
    } while (change);
    if (debug & TAUT)
	printf("COMPONENT_REDUCTION: split into %d %d\n",
	    comp_size, numcube - comp_size);
    if (comp_size == numcube)
	return MAYBE;

    /* Check tautology on the left partition */
    TL = TL1 = (pcube *) alloc((numcube+3) * sizeof(pcube));
    *TL++ = set_copy(new_cube(), T[0]);
    TL++;
    for(T1 = T+2; (p = *T1++) != NULL; )
	if (TESTP(p, COVERED))
	    *TL++ = p;    
    *TL++ = NULL;
    TL1[1] = (pcube) TL;		/* save pointer to last in array */
    if (tautology(TL1))	{
	free_cubelist(T);
	return TRUE;
    }

    /* Check tautology on the right partition */
    TL = TL1 = (pcube *) alloc((numcube+3) * sizeof(pcube));
    *TL++ = set_copy(new_cube(), T[0]);
    TL++;
    for(T1 = T+2; (p = *T1++) != NULL; )
	if (! TESTP(p, COVERED))
	    *TL++ = p;    
    *TL++ = NULL;
    TL1[1] = (pcube) TL;
    
    free_cubelist(T);
    return tautology(TL1);
}
bool ftaut_special_cases(T, result)
INOUT pcube *T;			/* will be disposed if answer is determined */
OUT pset_family *result;	/* returned only if answer determined */
{
    register pcube *T1, *Tsave, p, temp = cube.temp[0], ceil = cube.temp[1];
    int i, cnt, var;

    /* Check for a row of all 1's in essential cubes */
    for(T1 = T+2; (p = *T1++) != NULL; )
	if (! TESTP(p, REDUND))
	    if (full_row(p, T[0])) {
		*result = sf_new(0, Rp_size);
		free_cubelist(T);
		return TRUE;
	    }

    /* Collect column counts, determine unate variables, etc. */
start:
    massive_count(T);

    /* If function is unate, find the rows of all 1's */
    if (cdata.vars_unate == cdata.vars_active) {
	*result = sf_new(1, Rp_size);
	(*result)->count = 1;
	ceil = set_copy((*result)->data, Rp_start);
	cnt = Rp_size;
	for(T1 = T+2; (p = *T1++) != NULL; )
	    if (TESTP(p, REDUND))
		/* See if a redundant cube covers this leaf */
		if (full_row(p,T[0]))
		    cnt--, i = SIZE(p), set_remove(ceil, i);
	PUTSIZE(ceil, cnt);
	free_cubelist(T);
	return TRUE;

    /* Perform unate reduction if there are any unate variables */
    } else if (cdata.vars_unate != 0) {
	/* Form a cube "ceil" with full variables in the unate variables */
	(void) set_copy(ceil, cube.emptyset);
	for(var = 0; var < cube.num_vars; var++)
	    if (cdata.is_unate[var])
		INLINEset_or(ceil, ceil, cube.var_mask[var]);

	/* Save only those cubes that are "full" in all unate variables */
	for(Tsave = T1 = T+2; (p = *T1++) != NULL; )
	    if (setp_implies(ceil, set_or(temp, p, T[0])))
		*Tsave++ = p;
  	*Tsave++ = (pcube) NULL;
	T[1] = (pcube) Tsave;

	if (debug & TAUT)
	    printf("UNATE_REDUCTION: %d unate variables, reduced to %d\n",
		cdata.vars_unate, CUBELISTSIZE(T));
	goto start;
    }

    /* Not much we can do about it */
    return MAYBE;
}
/* mlist1 -- take two "cube lists" and merge them into a single cube list */
pcube *mlist1(F1, D1)
pcube *F1, *D1;
{
    register pcube p, *T1, *plist, *list;
    register int listlen = CUBELISTSIZE(F1) + CUBELISTSIZE(D1) + 3;

    /* Allocate a large enough cube list, and setup cofactored variables */
    list = plist = (pcube *) alloc(listlen * sizeof(pcube));
    *plist++ = set_copy(new_cube(), F1[0]);
    plist++;

    /* Save pointers to all cubes of F and all cubes of D */
    for(T1 = F1+2; (p = *T1++) != NULL; )
	*plist++ = p;
    for(T1 = D1+2; (p = *T1++) != NULL; )
	*plist++ = p;

    *plist++ = NULL;			/* sentinel */
    list[1] = (pcube) plist;		/* pointer to last */
    return list;
}


/*
   mlist2 -- merge two cube lists (reusing an already allocated cube list)
   We take the essential cubes of F and all of the cubes of D.
*/
void *mlist2(F1, D1, list)
register pcube *F1, *D1, *list;
{
    register pcube p, *T1, *plist;

    /* Setup first pointer which is the cofactored variables cube */
    plist = list;
    (void) set_copy(*plist++, F1[0]);
    plist++;

    /* Save pointers only to relatively essential cubes of F */
    for(T1 = F1+2; (p = *T1++) != NULL; )
	if (TESTP(p, RELESSEN))
	    *plist++ = p;
    /* Save pointers to all cubes of D */
    for(T1 = D1+2; (p = *T1++) != NULL; )
	*plist++ = p;

    *plist++ = NULL;			/* sentinel */
    list[1] = (pcube) plist;		/* pointer to last */
}
