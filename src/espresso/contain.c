/*
    contain.c -- set containment routines

    These are complex routines for performing containment over a
    family of sets, but they have the advantage of being much faster
    than a straightforward n*n routine.

    First the cubes are sorted by size, and as a secondary key they are
    sorted so that if two cubes are equal they end up adjacent.  We can
    than quickly remove equal cubes from further consideration by
    comparing each cube to its neighbor.  Finally, because the cubes
    are sorted by size, we need only check cubes which are larger (or
    smaller) than a given cube for containment.
*/

#include "espresso.h"


/*
    sf_contain -- perform containment on a set family (delete sets which
    are contained by some larger set in the family).  No assumptions are
    made about A, and the result will be returned in decreasing order of
    set size.
*/
pset_family sf_contain(A)
INOUT pset_family A;		/* disposes of A */
{
    int cnt;
    pset *A1;
    pset_family R;

    A1 = sf_sort(A, descend);		/* sort into descending order */
    cnt = rm_equal(A1, descend);	/* remove duplicates */
    cnt = rm_contain(A1);		/* remove contained sets */
    R = sf_unlist(A1, cnt, A->sf_size);	/* recreate the set family */
    sf_free(A);
    return R;
}


/* sf_dupl -- delete duplicate sets in a set family */
pset_family sf_dupl(A)
INOUT pset_family A;		/* disposes of A */
{
    register int cnt;
    register pset *A1;
    pset_family R;

    A1 = sf_sort(A, descend);		/* sort the set family */
    cnt = rm_equal(A1, descend);	/* remove duplicates */
    R = sf_unlist(A1, cnt, A->sf_size);	/* recreate the set family */
    sf_free(A);
    return R;
}


/*
    sf_union -- form the contained union of two set families (delete
    sets which are contained by some larger set in the family).  A and
    B are assumed already sorted in decreasing order of set size (and
    the SIZE field is assumed to contain the set size), and the result
    will be returned sorted likewise.
*/
pset_family sf_union(A, B)
INOUT pset_family A, B;		/* disposes of A and B */
{
    int cnt;
    pset_family R;
    pset *A1 = sf_list(A), *B1 = sf_list(B), *E1;

    E1 = (pset *) alloc((max(A->count,B->count)+1) * sizeof(pset));
    cnt = rm2_equal(A1, B1, E1, descend);
    cnt += rm2_contain(A1, B1) + rm2_contain(B1, A1);
    R = sf_merge(A1, B1, E1, cnt, A->sf_size);
    sf_free(A); sf_free(B);
    return R;
}

/*
    d1merge -- perform an efficient distance-1 merge of cubes of A
*/
pset_family d1merge(A, var)
INOUT pset_family A;		/* disposes of A */
IN int var;
{
    register pset pr, p, last, *A1;
    int cnt;
    pset_family R;

    set_copy(cube.temp[0], cube.var_mask[var]);
    A1 = sf_list(A);
    qsort((char *) A1, A->count, sizeof(pset), d1_order);

    cnt = A->count - d1_rm_equal(A1);
    R = sf_new(cnt, A->sf_size);
    R->count = cnt;
    pr = R->data;
    foreach_active_set(A, last, p) {
	INLINEset_copy(pr, p);
	pr += R->wsize;
    }

    mem_free((char *) A1);
    sf_free(A);
    return R;
}
/* d1_rm_equal -- distance-1 merge remove equal */
int d1_rm_equal(A1)
IN pcube *A1;
{
    register int xi, xj, del = 0;
    if (*A1 != NULL) {			/* If more than one set */
	for(xi = 0, xj = 1; A1[xj] != NULL; xj++)
	    if (d1_order(A1+xi, A1+xj) == 0) {
		INLINEset_or(A1[xi], A1[xi], A1[xj]);
		RESET(A1[xj], ACTIVE);
		del++;
	    } else {
		SET(A1[xi], ACTIVE);
		xi = xj;
	    }
	SET(A1[xi], ACTIVE);
    }
    return del;
}


/* rm_equal -- scan a sorted array of set pointers for duplicate sets */
int rm_equal(A1, compare)
INOUT pset *A1;			/* updated in place */
IN int (*compare)();
{
    register pset *p, *pdest = A1;

    if (*A1 != NULL) {			/* If more than one set */
	for(p = A1+1; *p != NULL; p++)
	    if ((*compare)(p, p-1) != 0)
		*pdest++ = *(p-1);
        *pdest++ = *(p-1);
	*pdest = NULL;
    }
    return pdest - A1;
}


/* rm_contain -- perform containment over a sorted array of set pointers */
int rm_contain(A1)
INOUT pset *A1;			/* updated in place */
{
    register pset *pa, *pb, *pcheck, a, b;
    pset *pdest = A1;
    int last_size = -1;

    /* Loop for all cubes of A1 */
    for(pa = A1; (a = *pa++) != NULL; ) {
	/* Update the check pointer if the size has changed */
	if (SIZE(a) != last_size)
	    last_size = SIZE(a), pcheck = pdest;
	for(pb = A1; pb != pcheck; ) {
	    b = *pb++;
	    INLINEsetp_implies(a, b, /* when_false => */ continue);
	    goto lnext1;
	}
	/* Set was not contained by some earlier set, so save it */
	*pdest++ = a;
    lnext1: ;
    }

    *pdest = NULL;
    return pdest - A1;
}


/* rm2_equal -- check two sorted arrays of set pointers for equal cubes */
int rm2_equal(A1, B1, E1, compare)
INOUT register pset *A1, *B1;		/* updated in place */
OUT pset *E1;
IN int (*compare)();
{
    register pset *pda = A1, *pdb = B1, *pde = E1;

    /* Walk through the arrays advancing pointer to larger cube */
    for(; *A1 != NULL && *B1 != NULL; )
	switch((*compare)(A1, B1)) {
	    case -1:	/* "a" comes before "b" */
		*pda++ = *A1++; break;
	    case 0:	/* equal cubes */
		*pde++ = *A1++; B1++; break;
	    case 1:	/* "a" is to follow "b" */
		*pdb++ = *B1++;	break;
	}

    /* Finish moving down the pointers of A and B */
    while (*A1 != NULL)
	*pda++ = *A1++;
    while (*B1 != NULL)
	*pdb++ = *B1++;
    *pda = *pdb = *pde = NULL;

    return pde - E1;
}


/* rm2_contain -- perform containment between two arrays of set pointers */
int rm2_contain(A1, B1)
INOUT pset *A1;			/* updated in place */
IN pset *B1;			/* unchanged */
{
    register pset *pa, *pb, a, b, *pdest = A1;

    /* for each set in the first array ... */
    for(pa = A1; (a = *pa++) != NULL; ) {
	/* for each set in the second array which is larger ... */
	for(pb = B1; (b = *pb++) != NULL && SIZE(b) > SIZE(a); ) {
	    INLINEsetp_implies(a, b, /* when_false => */ continue);
	    /* set was contained in some set of B, so don't save pointer */
	    goto lnext1;
	}
	/* set wasn't contained in any set of B, so save the pointer */
	*pdest++ = a;
    lnext1: ;
    }

    *pdest = NULL;			/* sentinel */
    return pdest - A1;			/* # elements in A1 */
}



/* sf_sort -- sort the sets of A */
pset *sf_sort(A, compare)
IN pset_family A;
IN int (*compare)();
{
    register pset p, last, *pdest, *A1;

    /* Create a single array pointing to each cube of A */
    pdest = A1 = (pset *) alloc((A->count+1)*sizeof(pset));
    foreach_set(A, last, p) {
	PUTSIZE(p, set_ord(p));		/* compute the set size */
	*pdest++ = p;			/* save the pointer */
    }
    *pdest = NULL;			/* Sentinel -- never seen by sort */

    /* Sort cubes by size */
    qsort((char *) A1, A->count, sizeof(pset), compare);
    return A1;
}


/* sf_list -- make a list of pointers to the sets in a set family */
pset *sf_list(A)
IN register pset_family A;
{
    register pset p, last, *pdest, *A1;

    /* Create a single array pointing to each cube of A */
    pdest = A1 = (pset *) alloc((A->count+1) * sizeof(pset));
    foreach_set(A, last, p)
	*pdest++ = p;			/* save the pointer */
    *pdest = NULL;			/* Sentinel */
    return A1;
}


/* sf_unlist -- make a set family out of a list of pointers to sets */
pset_family sf_unlist(A1, totcnt, size)
IN pset *A1;
IN int totcnt, size;
{
    register pset pr, p, *pa;
    pset_family R = sf_new(totcnt, size);

    R->count = totcnt;
    for(pr = R->data, pa = A1; (p = *pa++) != NULL; pr += R->wsize)
	INLINEset_copy(pr, p);
    mem_free((char *) A1);
    return R;
}


/* sf_merge -- merge three sorted lists of set pointers */
pset_family sf_merge(A1, B1, E1, totcnt, size)
INOUT pset *A1, *B1, *E1;		/* will be disposed of */
IN int totcnt, size;
{
    register pset pr, pa, pb, pe;
    register pset_family R = sf_new(totcnt, size);
    pset *SA1 = A1, *SB1 = B1, *SE1 = E1;

    /* Merge the results of the three arrays */
    R->count = totcnt;
    pa = *A1++; pb = *B1++; pe = *E1++;

    for(pr = R->data; ; pr += R->wsize)
	if (pa != NULL)
	    if (pb != NULL && desc1(pb, pa) < 0)
		if (pe != NULL && desc1(pe, pb) < 0)
		    {INLINEset_copy(pr, pe); pe = *E1++;}
		else
		    {INLINEset_copy(pr, pb); pb = *B1++;}
	    else
		if (pe != NULL && desc1(pe, pa) < 0)
		    {INLINEset_copy(pr, pe); pe = *E1++;}
		else
		    {INLINEset_copy(pr, pa); pa = *A1++;}
	else
	    if (pb != NULL)
		if (pe != NULL && desc1(pe, pb) < 0)
		    {INLINEset_copy(pr, pe); pe = *E1++;}
		else
		    {INLINEset_copy(pr, pb); pb = *B1++;}
	    else
		if (pe != NULL)
		    {INLINEset_copy(pr, pe); pe = *E1++;}
		else
		   break;

    mem_free((char *) SA1);
    mem_free((char *) SB1);
    mem_free((char *) SE1);
    return R;
}
