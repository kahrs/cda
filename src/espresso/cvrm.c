/*
    module: cvrm.c
    Purpose: miscellaneous cover manipulation
	a) routines supporting sharp product and cube intersection
	b) verify two covers are equal, check consistency of a cover
	c) unravel a multiple-valued cover into minterms
	c) sort covers
*/

#include "espresso.h"

/* cv_sharp -- form the sharp product between two covers */
pcover cv_sharp(A, B)
pcover A, B;
{
    register pcube last, p;
    pcover T;

    T = new_cover(0);
    foreach_set(A, last, p)
        T = sf_union(T, cb_sharp(p, B));
    return T;
}


/* cb_sharp -- form the sharp product between a cube and a cover */
pcover cb_sharp(c, T)
pcube c;
pcover T;
{
    register pcube p, last, nlast;
    pcover temp;
    int n, lev, lvl;
    double start = ptime();
    struct {
	pcover sf;
	int level;
    } stack[32];		/* 32 suffices for 2 ** 32 cubes ! */

    for(n = T->count, lev = 0; n != 0; n >>= 1, lev++)   ;

    /* Balanced merge */
    stack[0].sf = sf_addset(new_cover(1), c);
    stack[0].level = lev;
    n = 1;
    nlast = GETSET(T, T->count - 1);
    foreach_set(T, last, p) {
	stack[n].sf = sharp(c, p);
	stack[n++].level = lev;
	while (n > 1 && (stack[n-1].level==stack[n-2].level || p == nlast)) {
	    temp = cv_intersect(stack[n-1].sf, stack[n-2].sf);
	    lvl = min(stack[n-1].level, stack[n-2].level) - 1;
	    if (debug & SHARP && stack[n-2].level < 5) {
		printf("# SHARP[%d]: %4d = %4d x %4d, time = %s\n",
		    lvl, temp->count, stack[n-1].sf->count,
		    stack[n-2].sf->count, print_time(ptime() - start));
		(void) fflush(stdout);
	    }
	    free_cover(stack[n-2].sf);
	    free_cover(stack[n-1].sf);
	    stack[n-2].sf = temp;
	    stack[n-2].level = lvl;
	    n--;
	}
    }

    return stack[0].sf;
}


/* sharp -- form the sharp product between two cubes */
pcover sharp(a, b)
pcube a, b;
{
    register int var;
    register pcube d=cube.temp[0], temp=cube.temp[1], temp1=cube.temp[2];
    pcover r = new_cover(cube.num_vars);

    if (cdist0(a, b)) {
	set_diff(d, a, b);
	for(var = 0; var < cube.num_vars; var++)
	    if (! setp_empty(set_and(temp, d, cube.var_mask[var]))) {
		set_diff(temp1, a, cube.var_mask[var]);
		set_or(GETSET(r, r->count++), temp, temp1);
	    }
    } else
	r = sf_addset(r, a);
    return r;
}

/* cv_intersect -- form the intersection of two covers */

#define MAGIC 500		/* save 500 cubes before containment */

pcover cv_intersect(A, B)
IN pcover A, B;
{
    register pcube pi, pj, lasti, lastj, pt;
    pcover T, Tsave = NULL;

    /* How large should each temporary result cover be ? */
    T = new_cover(MAGIC);
    pt = T->data;

    /* Form pairwise intersection of each cube of A with each cube of B */
    foreach_set(A, lasti, pi)
	foreach_set(B, lastj, pj)
	    if (cdist0(pi, pj)) {
		(void) set_and(pt, pi, pj);
		if (++T->count >= T->capacity) {
		    if (Tsave == NULL)
			Tsave = sf_contain(T);
		    else
			Tsave = sf_union(Tsave, sf_contain(T));
		    T = new_cover(MAGIC);
		    pt = T->data;
		} else
		    pt += T->wsize;
	    }

    
    if (Tsave == NULL)
	Tsave = sf_contain(T);
    else
	Tsave = sf_union(Tsave, sf_contain(T));

    return Tsave;
}

/*
    verify -- check that all minterms of F are contained in (Fold u Dold)
    and that all minterms of Fold are contained in (F u Dold).
*/
bool verify(F, Fold, Dold)
IN pcover F, Fold, Dold;
{
    pcube p, last, *FD;
    bool verify_error = FALSE;

    /* Make sure the function didn't grow too large */
    FD = cube2list(Fold, Dold);
    foreach_set(F, last, p)
	if (! cube_is_covered(FD, p)) {
	    printf("some minterm in F is not covered by Fold u Dold\n");
	    verify_error = TRUE;
	    if (verbose_debug) printf("%s\n", pc1(p)); else break;
	}
    free_cubelist(FD);

    /* Make sure minimized function covers the original function */
    FD = cube2list(F, Dold);
    foreach_set(Fold, last, p)
	if (! cube_is_covered(FD, p)) {
	    printf("some minterm in Fold is not covered by F u Dold\n");
	    verify_error = TRUE;
	    if (verbose_debug) printf("%s\n", pc1(p)); else break;
	}
    free_cubelist(FD);

    return verify_error;
}
/*
    check_consistency -- test that the ON-set, OFF-set and DC-set form
    a partition of the boolean space.
*/
bool check_consistency(PLA)
pPLA PLA;
{
    bool verify_error = FALSE;
    pcover T;

    T = cv_intersect(PLA->F, PLA->D);
    if (T->count == 0)
	printf("ON-SET and DC-SET are disjoint\n");
    else {
	printf("Some minterm(s) belong to both the ON-SET and DC-SET !\n");
	if (verbose_debug)
	    cprint(T);
	verify_error = TRUE;
    }
    free_cover(T);

    T = cv_intersect(PLA->F, PLA->R);
    if (T->count == 0)
	printf("ON-SET and OFF-SET are disjoint\n");
    else {
	printf("Some minterm(s) belong to both the ON-SET and OFF-SET !\n");
	if (verbose_debug)
	    cprint(T);
	verify_error = TRUE;
    }
    free_cover(T);

    T = cv_intersect(PLA->D, PLA->R);
    if (T->count == 0)
	printf("DC-SET and OFF-SET are disjoint\n");
    else {
	printf("Some minterm(s) belong to both the OFF-SET and DC-SET !\n");
	if (verbose_debug)
	    cprint(T);
	verify_error = TRUE;
    }
    free_cover(T);

    if (tautology(cube3list(PLA->F, PLA->D, PLA->R)))
	printf("Union of ON-SET, OFF-SET and DC-SET is the universe\n");
    else {
	T = complement(cube3list(PLA->F, PLA->D, PLA->R));
	printf("There are minterms left unspecified !\n");
	if (verbose_debug)
	    cprint(T);
	verify_error = TRUE;
	free_cover(T);
    }
    return verify_error;
}
pcover cb_unravel(c, start, startbase)
IN register pcube c;
IN int start;
IN pcube startbase;
{
    pcube base = cube.temp[0], p, last;
    int expansion, place, skip, var, size;
    register pcover B;
    register int i, j, k, n;

    /* Determine how many cubes it will blow up into, and create a mask
	for those parts that have only a single coordinate 
    */
    expansion = 1;
    (void) set_copy(base, startbase);
    for(var = start; var < cube.num_vars; var++)
	if ((size = set_dist(c, cube.var_mask[var])) < 2)
	    (void) set_or(base, base, cube.var_mask[var]);
	else if ((expansion *= size) > 1000000) 
	    fatal("espresso: too big of blowup in unravel");
    (void) set_and(base, c, base);

    B = new_cover(expansion);
    B->count = expansion;
    foreach_set(B, last, p)
	INLINEset_copy(p, base);

    place = expansion;
    for(var = start; var < cube.num_vars; var++)
	if ((size = set_dist(c, cube.var_mask[var])) > 1) {
	    skip = place;
	    place = place / size;
	    n = 0;
	    for(i = cube.first_part[var]; i <= cube.last_part[var]; i++)
	 	if (is_in_set(c, i)) {
		    for(j = n; j < expansion; j += skip)
			for(k = 0; k < place; k++)
			    (void) set_insert(GETSET(B, j + k), i);
		    n += place;
		}
	}
    return B;
}

pcover unravel(B, start)
IN pcover B;
IN int start;
{
    int var;
    pcover B1;
    register pcube p, last, startbase = cube.temp[1];

    (void) set_copy(startbase, cube.emptyset);
    for(var = 0; var < start; var++)
	(void) set_or(startbase, startbase, cube.var_mask[var]);

    B1 = new_cover(B->count);
    foreach_set(B, last, p)
	B1 = sf_append(B1, cb_unravel(p, start, startbase));
    free_cover(B);
    return B1;
}
/* descend -- compare two sets for the qsort() routine, and return

	-1 if set a is to precede set b
	 0 if set a and set b are equal
	 1 if set a is to follow set b
   
    The SIZE field of the set is assumed to contain the size of the set (which
    will save recomputing the set size during the sort).
*/

/* descend -- comparison for descending sort on set size */
int descend(a, b)
pset *a, *b;
{
    register pset a1 = *a, b1 = *b;
    if (SIZE(a1) > SIZE(b1)) return -1;
    else if (SIZE(a1) < SIZE(b1)) return 1;
    else {
	register int i = LOOP(a1);
	do
	    if (a1[i] > b1[i]) return -1; else if (a1[i] < b1[i]) return 1;
	while (--i > 0);
    }
    return 0;
}

/* ascend -- comparison for ascending sort on set size */
int ascend(a, b)
pset *a, *b;
{
    register pset a1 = *a, b1 = *b;
    if (SIZE(a1) > SIZE(b1)) return 1;
    else if (SIZE(a1) < SIZE(b1)) return -1;
    else {
	register int i = LOOP(a1);
	do
	    if (a1[i] > b1[i]) return 1; else if (a1[i] < b1[i]) return -1;
	while (--i > 0);
    }
    return 0;
}

/* lex_order -- comparison for "lexical" ordering of cubes */
int lex_order(a, b)
pset *a, *b;
{
    register pset a1 = *a, b1 = *b;
    register int i = LOOP(a1);
    do
	if (a1[i] > b1[i]) return -1; else if (a1[i] < b1[i]) return 1;
    while (--i > 0);
    return 0;
}

/* d1_order -- comparison for distance-1 merge routine */
int d1_order(a, b)
pset *a, *b;
{
    register pset a1 = *a, b1 = *b, c1 = cube.temp[0];
    register int i = LOOP(a1);
    register unsigned int x1, x2;
    do
	if ((x1 = a1[i] | c1[i]) > (x2 = b1[i] | c1[i])) return -1;
	else if (x1 < x2) return 1;
    while (--i > 0);
    return 0;
}

/* desc1 -- comparison (without indirection) for descending sort */
int desc1(a, b)
register pset a, b;
{
    if (SIZE(a) > SIZE(b)) return -1;
    else if (SIZE(a) < SIZE(b)) return 1;
    else {
	register int i = LOOP(a);
	do
	    if (a[i] > b[i]) return -1; else if (a[i] < b[i]) return 1;
	while (--i > 0);
    }
    return 0;
}

/* lex_sort -- sort cubes in a standard lexical fashion */
pcover lex_sort(T)
pcover T;
{
    pcover T1 = sf_unlist(sf_sort(T, lex_order), T->count, T->sf_size);
    free_cover(T);
    return T1;
}

/* size_sort -- sort cubes by their size */
pcover size_sort(T)
pcover T;
{
    pcover T1 = sf_unlist(sf_sort(T, descend), T->count, T->sf_size);
    free_cover(T);
    return T1;
}
