#include "espresso.h"

/*
    The cofactor of a cover against a cube "c" is a cover formed by the
    cofactor of each cube in the cover against c.  The cofactor of two
    cubes is null if they are distance 1 or more apart.  If they are
    distance zero apart, the cofactor is the restriction of the cube
    to the minterms of c.

    The cube list contains the following information:

	T[0] = pointer to a cube identifying the variables that have
		been cofactored against
	T[1] = pointer to just beyond the sentinel (i.e., T[n] in this case)
	T[2] 
	  .
          .  = pointers to cubes
	  .
        T[n-2]
	T[n-1] = NULL pointer (sentinel)


    Cofactoring involves repeated application of "cdist0" to check if a
    cube of the cover intersects the cofactored cube.  This can be
    slow, especially for the recursive descent of the espresso
    routines.  Therefore, a special cofactor routine "scofactor" is
    provided which assumes the cofactor is only in a single variable.
*/


/* cofactor -- compute the cofactor of a cover with respect to a cube */
pcube *cofactor(T, c)
IN pcube *T;
IN register pcube c;
{
    pcube temp = cube.temp[0], *Tc_save, *Tc, *T1;
    register pcube p;
    int listlen;

    listlen = CUBELISTSIZE(T) + 5;

    /* Allocate a new list of cube pointers (max size is previous size) */
    Tc_save = Tc = (pcube *) alloc(listlen * sizeof(pcube));

    /* pass on which variables have been cofactored against */
    *Tc++ = set_or(new_cube(), T[0], set_diff(temp, cube.fullset, c));
    Tc++;

    /* Loop for each cube in the list, determine suitability, and save */
    for(T1 = T+2; (p = *T1++) != NULL; )
	if (p != c) {
/* if (cdist0(p, c)) */
    {register int w,last;register unsigned int x;if((last=cube.inword)!=-1)
    {x=p[last]&c[last];if(~(x|x>>1)&cube.inmask)goto false;for(w=1;w<last;w++)
    {x=p[w]&c[w];if(~(x|x>>1)&DISJOINT)goto false;}}}{register int w,var,last;
    register pcube mask;for(var=cube.num_binary_vars;var<cube.num_vars;var++){
    mask=cube.var_mask[var];last=cube.last_word[var];for(w=cube.first_word[var
    ];w<=last;w++)if(p[w]&c[w]&mask[w])goto nextvar;goto false;nextvar:;}}

	    *Tc++ = p;
	false: ;
        }

    *Tc++ = NULL;				/* sentinel */
    Tc_save[1] = (pcube) Tc;			/* save pointer to last */
    return Tc_save;
}
/*
    scofactor -- compute the cofactor of a cover with respect to a cube, 
    where the cube is "active" in only a single variable.

    This routine has been optimized for speed.
*/
  
pcube *scofactor(T, c, var)
IN pcube *T, c;
IN int var;
{
    pcube *Tc, *Tc_save;
    register pcube p, mask = cube.temp[1], *T1;
    register int first = cube.first_word[var], last = cube.last_word[var];
    int listlen;

    listlen = CUBELISTSIZE(T) + 5;

    /* Allocate a new list of cube pointers (max size is previous size) */
    Tc_save = Tc = (pcube *) alloc(listlen * sizeof(pcube));

    /* pass on which variables have been cofactored against */
    *Tc++ = set_or(new_cube(), T[0], set_diff(mask, cube.fullset, c));
    Tc++;

    /* Setup for the quick distance check */
    (void) set_and(mask, cube.var_mask[var], c);

    /* Loop for each cube in the list, determine suitability, and save */
    for(T1 = T+2; (p = *T1++) != NULL; )
	if (p != c) {
	    register int i = first;
	    do
		if (p[i] & mask[i]) {
		    *Tc++ = p;
		    break;
		}
	    while (++i <= last);
	}

    *Tc++ = NULL;				/* sentinel */
    Tc_save[1] = (pcube) Tc;			/* save pointer to last */
    return Tc_save;
}
void massive_count(T)
IN pcube *T;
{
    register int *count = cdata.part_zeros;
    pcube *T1;

    /* Clear the column counts (count of # zeros in each column) */
 {  register int i, last;
    for(i = 0, last = cube.size; i < last; i++)
	count[i] = 0;
 }

    /* Count the number of zeros in each column */
 {  register int i, base; register unsigned int val;
    register pcube p, cof = T[0];
    for(T1 = T+2; (p = *T1++) != NULL; )
	for(i = LOOP(p); i > 0;) {
	    val = cube.fullset[i] & ~ (p[i] | cof[i]);
	    for(base = --i << LOGBPI; val != 0; base++, val >>= 1)
		if (val & 1)
		    count[base]++;
	}
 }

    /*
     * Perform counts for each variable:
     *    cdata.var_zeros[var] = number of zeros in the variable
     *    cdata.parts_active[var] = number of active parts for each variable
     *    cdata.vars_active = number of variables which are active
     *    cdata.vars_unate = number of variables which are active and unate
     *
     *    best -- the variable which is best for splitting based on:
     *    mostactive -- most # active parts in any variable
     *    mostzero -- most # zeros in any variable
     *    mostbalanced -- minimum over the maximum # zeros / part / variable
     */
	
 {  register int var, i, lastbit, active, maxactive;
    int best = -1, mostactive = 0, mostzero = 0, mostbalanced = 32000;
    cdata.vars_unate = cdata.vars_active = 0;

    for(var = 0; var < cube.num_vars; var++) {
	if (var < cube.num_binary_vars) { /* special hack for binary vars */
	    i = count[var*2];
	    lastbit = count[var*2 + 1];
	    active = (i > 0) + (lastbit > 0);
	    cdata.var_zeros[var] = i + lastbit;
	    maxactive = max(i, lastbit);
	} else {
	    maxactive = active = cdata.var_zeros[var] = 0;
	    lastbit = cube.last_part[var];
	    for(i = cube.first_part[var]; i <= lastbit; i++) {
		cdata.var_zeros[var] += count[i];
		active += (count[i] > 0);
		if (active > maxactive) maxactive = active;
	    }
	}

	/* first priority is to maximize the number of active parts */
	/* for binary case, this will usually select the output first */
	if (active > mostactive)
	    best = var, mostactive = active, mostzero = cdata.var_zeros[best],
            mostbalanced = maxactive;
	else if (active == mostactive)
	    /* secondary condition is to maximize the number zeros */
	    /* for binary variables, this is the same as minimum # of 2's */
	    if (cdata.var_zeros[var] > mostzero)
		best = var, mostzero = cdata.var_zeros[best], 
		mostbalanced = maxactive;
	    else if (cdata.var_zeros[var] == mostzero)
		/* third condition is to pick a balanced variable */
		/* for binary vars, this means roughly equal # 0's and 1's */
		if (maxactive < mostbalanced)
		    best = var, mostbalanced = maxactive;
		
	cdata.parts_active[var] = active;
	cdata.is_unate[var] = (active == 1);
	cdata.vars_active += (active > 0);
	cdata.vars_unate += (active == 1);
    }
    cdata.best = best;
 }
}
int binate_split_select(T, cleft, cright, debug_flag)
IN pcube *T;
IN register pcube cleft, cright;
IN int debug_flag;
{
    int best = cdata.best;
    register int i, lastbit = cube.last_part[best], halfbit = 0;
    register pcube cof=T[0];

    /* Create the cubes to cofactor against */
    set_diff(cleft, cube.fullset, cube.var_mask[best]);
    set_diff(cright, cube.fullset, cube.var_mask[best]);
    for(i = cube.first_part[best]; i <= lastbit; i++)
	if (! is_in_set(cof,i))
	    halfbit++;
    for(i = cube.first_part[best], halfbit = halfbit/2; halfbit > 0; i++)
	if (! is_in_set(cof,i))
	    halfbit--, set_insert(cleft, i);
    for(; i <= lastbit; i++)
	if (! is_in_set(cof,i))
	    set_insert(cright, i);

    if (debug & debug_flag) {
	printf("BINATE_SPLIT_SELECT: split against %d\n", best);
	if (verbose_debug)
	    printf("cl=%s\ncr=%s\n", pc1(cleft), pc2(cright));
    }
    return best;
}
int unate_split_select(T, cleft, cright, debug_flag)
IN pcube *T;
IN register pcube cleft, cright;
IN int debug_flag;
{
    register int i, var; int best, bestsize;
    pcube largest = cube.temp[0];

    /* Find the largest cube and add the cofactored variables */
    set_or(largest, T[0], largest_cube(T));

    /* Find which variable in the largest cube is most active */
    best = -1; bestsize = 0;
    for(var = 0; var < cube.num_vars; var++)
	if (! setp_implies(cube.var_mask[var], largest))
	    if (cdata.var_zeros[var] > bestsize)
		best = var, bestsize = cdata.var_zeros[var];
    if (best == -1)
	fatal("cofactor.c/unate_split_select: best is -1!");

    /* Create the cubes to cofactor against */
    set_copy(cleft, cube.fullset);
    set_diff(cright, cube.fullset, cube.var_mask[best]);
    for(i = cube.first_part[best]; ; i++)
	if (cdata.part_zeros[i] > 0) {
	    set_insert(cright, i);
	    set_remove(cleft, i);
	    break;
	}

    if (debug & debug_flag) {
	printf("UNATE_SPLIT_SELECT: split against %d\n", best);
	if (verbose_debug)
	    printf("cl=%s\ncr=%s\n", pc1(cleft), pc2(cright));
    }
    return best;
}

pcube largest_cube(T)
pcube *T;
{
    register pcube temp = cube.temp[0], largest = NULL, *T1, p;
    register int bestsize = -1, size;
    /* find largest cube */
    for(T1 = T+2; (p = *T1++) != NULL; )
	if ((size = set_ord(set_or(temp, T[0], p))) > bestsize)
	    largest = p, bestsize = size;
    if (largest == NULL)
	fatal("cofactor.c/largest_cube: largest is NULL");
    return largest;
}
pcube *cube1list(A)
pcover A;
{
    register pcube last, p, *plist, *list;
    list = plist = (pcube *) alloc((A->count+3) * sizeof(pcube));
    *plist++ = new_cube();
    plist++;
    foreach_set(A, last, p)
	*plist++ = p;
    *plist++ = NULL;			/* sentinel */
    list[1] = (pcube) plist;
    return list;
}

pcube *cube2list(A, B)
pcover A, B;
{
    register pcube last, p, *plist, *list;
    list = plist = (pcube *) alloc((A->count+B->count+3) * sizeof(pcube));
    *plist++ = new_cube();
    plist++;
    foreach_set(A, last, p)
	*plist++ = p;
    foreach_set(B, last, p)
	*plist++ = p;
    *plist++ = NULL;
    list[1] = (pcube) plist;
    return list;
}

pcube *cube3list(A, B, C)
pcover A, B, C;
{
    register pcube last, p, *plist, *list;
    plist = (pcube *) alloc((A->count+B->count+C->count+3) * sizeof(pcube));
    list = plist;
    *plist++ = new_cube();
    plist++;
    foreach_set(A, last, p)
	*plist++ = p;
    foreach_set(B, last, p) 
	*plist++ = p;
    foreach_set(C, last, p)
	*plist++ = p;
    *plist++ = NULL;
    list[1] = (pcube) plist;
    return list;
}
