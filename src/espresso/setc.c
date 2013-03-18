/*
    setc.c -- massive bit-hacking for performing special "cube"-type
    operations on a set

    The basic trick used for binary valued variables is the following:

    If a[w] and b[w] contain a full word of binary variables, then:

     1) to get the full word of their intersection, we use

	    x = a[w] & b[w];


     2) to see if the intersection is empty in any positions, we examine

	    x = ~(x | x >> 1) & DISJOINT;

	this will have a single 1 in each binary variable for which
	the intersection is empty.  In particular, if this is zero,
	then there are no disjoint variables; or, if this is nonzero,
	then there is at least one disjoint variable.  A "count_ones"
	over x will tell in how many variables they have an empty
	intersection.


     3)	to get a mask which selects the disjoint variables, we use

	    (x | x << 1)

	this provides a selector which can be used to see where
	they have an empty intersection


    cdist	return distance between two cubes
    cdist0	return true if two cubes are distance 0 apart
    cdist01	return distance, or 2 if distance exceeds 1
    consensus	return true if consensus nonempty, or false otherwise
		(also computes the consensus and saves it)
    force_lower	expand hack (for now), related to consensus
*/

#include "espresso.h"

/* see if the cube has a full row of 1's (with respect to cof) */
bool full_row(p, cof)
IN register pcube p, cof;
{
    register int i = LOOP(p);
    do if ((p[i] | cof[i]) != cube.fullset[i]) return FALSE; while (--i > 0);
    return TRUE;
}
/*
    cdist0 -- return TRUE if a and b are distance 0 apart
*/

bool cdist0(a, b)
register pcube a, b;
{
 {  /* Check binary variables */
    register int w, last; register unsigned int x;
    if ((last = cube.inword) != -1) {

	/* Check the partial word of binary variables */
	x = a[last] & b[last];
	if (~(x | x >> 1) & cube.inmask)
	    return FALSE;		/* disjoint in some variable */

	/* Check the full words of binary variables */
	for(w = 1; w < last; w++) {
	    x = a[w] & b[w];
	    if (~(x | x >> 1) & DISJOINT)
		return FALSE;		/* disjoint in some variable */
	}
    }
 }

 {  /* Check the multiple-valued variables */
    register int w, var, last; register pcube mask;
    for(var = cube.num_binary_vars; var < cube.num_vars; var++) {
	mask = cube.var_mask[var]; last = cube.last_word[var];
	for(w = cube.first_word[var]; w <= last; w++)
	    if (a[w] & b[w] & mask[w])
		goto nextvar;
	return FALSE;		/* disjoint in this variable */
    nextvar: ;
    }
 }
    return TRUE;
}
/*
    cdist01 -- return the "distance" between two cubes (defined as the
    number of null variables in their intersection).  If the distance
    exceeds 1, the value 2 is returned.
*/

int cdist01(a, b)
register pset a, b;
{
    int dist = 0;

 {  /* Check binary variables */
    register int w, last; register unsigned int x;
    if ((last = cube.inword) != -1) {

	/* Check the partial word of binary variables */
	x = a[last] & b[last];
	if (x = ~ (x | x >> 1) & cube.inmask)
	    if ((dist = count_ones(x)) > 1)
		return 2;

	/* Check the full words of binary variables */
	for(w = 1; w < last; w++) {
	    x = a[w] & b[w];
	    if (x = ~ (x | x >> 1) & DISJOINT)
		if (dist == 1 || (dist += count_ones(x)) > 1)
		    return 2;
        }
    }
 }

 {  /* Check the multiple-valued variables */
    register int w, var, last; register pcube mask;
    for(var = cube.num_binary_vars; var < cube.num_vars; var++) {
	mask = cube.var_mask[var]; last = cube.last_word[var];
	for(w = cube.first_word[var]; w <= last; w++)
	    if (a[w] & b[w] & mask[w])
		goto nextvar;
	if (++dist > 1)
	    return 2;
    nextvar: ;
    }
 }
    return dist;
}
/*
    cdist -- return the "distance" between two cubes (defined as the
    number of null variables in their intersection).
*/

int cdist(a, b)
register pset a, b;
{
    int dist = 0;

 {  /* Check binary variables */
    register int w, last; register unsigned int x;
    if ((last = cube.inword) != -1) {

	/* Check the partial word of binary variables */
	x = a[last] & b[last];
	if (x = ~ (x | x >> 1) & cube.inmask)
	    dist = count_ones(x);

	/* Check the full words of binary variables */
	for(w = 1; w < last; w++) {
	    x = a[w] & b[w];
	    if (x = ~ (x | x >> 1) & DISJOINT)
		dist += count_ones(x);
        }
    }
 }

 {  /* Check the multiple-valued variables */
    register int w, var, last; register pcube mask;
    for(var = cube.num_binary_vars; var < cube.num_vars; var++) {
	mask = cube.var_mask[var]; last = cube.last_word[var];
	for(w = cube.first_word[var]; w <= last; w++)
	    if (a[w] & b[w] & mask[w])
		goto nextvar;
	dist++;
    nextvar: ;
    }
 }
    return dist;
}
/*
    force_lower -- Determine which variables of a do not intersect b.
*/

pset force_lower(xlower, a, b)
INOUT pset xlower;
IN register pset a, b;
{

 {  /* Check binary variables (if any) */
    register int w, last; register unsigned int x;
    if ((last = cube.inword) != -1) {

	/* Check the partial word of binary variables */
	x = a[last] & b[last];
	if (x = ~(x | x >> 1) & cube.inmask)
	    xlower[last] |= (x | (x << 1)) & a[last];

	/* Check the full words of binary variables */
	for(w = 1; w < last; w++) {
	    x = a[w] & b[w];
	    if (x = ~(x | x >> 1) & DISJOINT)
		xlower[w] |= (x | (x << 1)) & a[w];
	}
    }
 }

 {  /* Check the multiple-valued variables */
    register int w, var, last; register pcube mask;
    for(var = cube.num_binary_vars; var < cube.num_vars; var++) {
	mask = cube.var_mask[var]; last = cube.last_word[var];
	for(w = cube.first_word[var]; w <= last; w++)
	    if (a[w] & b[w] & mask[w])
		goto nextvar;
	for(w = cube.first_word[var]; w <= last; w++)
	    xlower[w] |= a[w] & mask[w];
    nextvar: ;
    }
 }
    return xlower;
}
/*
    consensus -- multiple-valued consensus

    The consensus of two cubes is the largest cube which has a minterm
    in each of the two cubes and is contained in their union.
*/
bool consensus(r, a, b)
INOUT pcube r;
IN register pcube a, b;
{
    int dist = 0; bool empty;
 {  register int i=LOOP(a); *r = i; do r[i]=0; while (--i > 0);}

 {  /* Check binary variables (if any) */
    register int w, last; register unsigned int x;
    if ((last = cube.inword) != -1) {
	/* Check the partial word of binary variables */
	r[last] = x = a[last] & b[last];
	if (x = ~(x | x >> 1) & cube.inmask)
	    if ((dist = count_ones(x)) > 1)
		return FALSE;				/* distance > 1 */
	    else
		r[last] |= (x | (x << 1)) & (a[last] | b[last]);
	/* Check the full words of binary variables */
	for(w = 1; w < last; w++) {
	    r[w] = x = a[w] & b[w];
	    if (x = ~(x | x >> 1) & DISJOINT)
		if (dist == 1 || (dist += count_ones(x)) > 1)
		    return FALSE;			/* distance > 1 */
		else
		    r[w] |= (x | (x << 1)) & (a[w] | b[w]);
	}
    }
 }
 {  /* Check the multiple-valued variables */
    register int w, var, last; register pcube mask; unsigned int x;
    for(var = cube.num_binary_vars; var < cube.num_vars; var++) {
	mask = cube.var_mask[var]; last = cube.last_word[var]; empty = TRUE;
	for(w = cube.first_word[var]; w <= last; w++)
	    if (x = a[w] & b[w] & mask[w])
		empty = FALSE, r[w] |= x;
	if (empty)
	    if (++dist > 1) return FALSE; else goto orpart;
	else if (dist == 0) {
	    /* check if O(b) is contained in O(a) */
	    for(w = cube.first_word[var]; w <= last; w++)
		if (a[w] & mask[w] & ~b[w])
		    goto orpart;		/* not contained, */
	    goto nextvar;
	} else
	    goto nextvar;
    orpart:
	for(w = cube.first_word[var]; w <= last; w++)
	    r[w] |= mask[w] & (a[w] | b[w]);
    nextvar: ;
    }
 }
    return TRUE;
}
/*
    ccommon -- return TRUE if a and b are share "active" variables
*/

bool ccommon(a, b)
register pcube a, b;
{
 {  /* Check binary variables */
    register int w, last;
    if ((last = cube.inword) != -1) {

	/* Check the partial word of binary variables */
	if (~(a[last] & a[last]>>1) & ~(b[last] & b[last]>>1) & cube.inmask)
	    return TRUE;

	/* Check the full words of binary variables */
	for(w = 1; w < last; w++)
	    if (~(a[w]&a[w]>>1) & ~(b[w]&b[w]>>1) & DISJOINT)
		return TRUE;
    }
 }

 {  /* Check the multiple-valued variables */
    register int w, var, last; register pcube mask;
    for(var = cube.num_binary_vars; var < cube.num_vars; var++) {
	mask = cube.var_mask[var]; last = cube.last_word[var];
	/* Check for some part missing from a */
	for(w = cube.first_word[var]; w <= last; w++)
	    if (mask[w] & ~a[w]) {

		/* If so, check for some part missing from b */
		for(w = cube.first_word[var]; w <= last; w++)
		    if (mask[w] & ~b[w])
			return TRUE;		/* both active */
		break;
	    }
    }
 }
    return FALSE;
}
