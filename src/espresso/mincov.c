/*
    Module: mincov.c
    Author: Richard Rudell
    Purpose: Find a minimum cover of a family of sets

    The problem may be stated as this: 

	Given a family of sets, select a covering set which has a nonzero
	intersection with each member of the family.

    That is, we consider a set covered if one of its elements is in our
    covering set, and we want to cover each set of the family.
  
    This is inherently a difficult problem (NP-complete), and hence the
    goal is a simple polynomial heuristic which will produce
    satisfactory results.  We suggest the following heuristics:

	1.  simple greedy: select the most frequent element in the set
	family to belong to the covering set.  Remove all sets now
	covered, and repeat until every set is covered.  A little bit
	better is to weight each element as the sum of 1/set_ord(s) for
  	each set s which contains the element.

	2. clique greedy: Find a maximal independent set of sets and
	select the most frequent element in the set family which is
	also in one of the independent sets.  Add this to the covering
	set, remove all sets now covered, and repeat until every set
	if covered.  As above, the elements can be assigned a weight
	of sum over 1/set_ord(s) for each set s containing the element. 
	Another possibility is to assign the weight as the sum over
	1/set_ord(set_and(s,restrict)) where restrict is the union over
	the independent sets.

    The clique greedy technique has the wonderful side-affect of
    producing a lower bound on the number of elements needed in the
    lowering set.  Unfortunately, it also merely swaps one NP-complete
    problem (minimum cover) for another (maximum independent set).
    However, in practice, we have found that the simple greedy
    heuristic used for maximum independent set performs better than the
    simple greedy heuristic for minimum cover.

    We apply the above two methods and then select the best result we
    find.  By far the most expensive is the finding of a maximal
    independent set of sets, and hence we can afford to also run the
    much simpler greedy heuristic.
*/

#include "espresso.h"

#define SCALE	1024
/*  minimum_cover -- Find a minimal covering of A */
pset minimum_cover(A, bound)
pset_family A;
int *bound;
{
    register pset p, last;
    pset indep_set, temp, best, soln;
    int *count;
    indep_set=set_new(A->count); temp=set_new(A->count);
    best=set_new(A->sf_size); soln=set_new(A->sf_size);

    if (debug & MINCOV1) {
	printf("MINCOV: set_family is\n");
	sf_print(A);
  	write_set_family(stdout, A);
    }

    /* Quick check to make sure the problem makes sense */
    if (A->count <= 0)
	goto mincov_exit;
    foreach_set(A, last, p)
	if (setp_empty(p))
	    fatal("mincov.c/minimum_cover: no set covering possible");

    if (mincov_exact) {
	pset_family X = exact_minimum_cover(A);
	set_copy(best, GETSET(X,0));
	set_copy(indep_set, GETSET(X,0));
	sf_free(X);
	goto mincov_exit;
    }

    /* Simple column-weight greedy approach to the covering */
    count = sf_weight_count(sf_active(A));
    while (A->active_count != 0)
	greedy_select(A, count, best, (pset) NULL);
    mem_free((char *) count);
    if (debug & MINCOV)
	printf("MINCOV: weighted strategy gives %d\n", set_ord(best));

    /* Independent set selection strategy #1 */
    clique_select(sf_active(A), soln, indep_set, TRUE, TRUE);
    while (A->active_count != 0)
	clique_select(A, soln, temp, TRUE, TRUE);
    weed(A, soln);
    if (debug & MINCOV)
	printf("MINCOV: independent strategy #1 gives %d\n", set_ord(soln));
    if (set_ord(best) > set_ord(soln))
	set_copy(best, soln);

    /* If we achieved the lower bound, we can quit now */
    if (set_ord(indep_set) == set_ord(best))
	goto mincov_exit;

    /* Independent set selection strategy #2 -- reuse last indep set */
    set_clear(soln, A->sf_size);
    clique_select(sf_active(A), soln, indep_set, FALSE, FALSE);
    while (A->active_count != 0)
	clique_select(A, soln, temp, TRUE, FALSE);
    weed(A, soln);
    if (debug & MINCOV)
	printf("MINCOV: independent strategy #2 gives %d\n", set_ord(soln));
    if (set_ord(best) > set_ord(soln))
	set_copy(best, soln);

    /* It's always nice to check that we're correct */
mincov_exit:
    foreach_set(A, last, p)
	if (setp_disjoint(p, best))
	    fatal("mincov.c/minimum_cover: some set not covered!");
    *bound = set_ord(indep_set);
    set_free(indep_set);
    set_free(temp);
    set_free(soln);
    return best;
}

/*
    clique_select -- heuristic solution to covering problem

    The idea is to find a group of sets of A that are pairwise disjoint --
    i.e., a group of sets from A such that no two sets have an element in
    common.  It is obvious that the minimum solutions must contain one element
    from each of these sets.  Then we heuristically select one element from
    each set in the independent group.
*/
void clique_select(A, select, indep_set, compute_indep_set, weighted_select)
pset_family A;
pset select, indep_set;
bool compute_indep_set, weighted_select;
{
    int index, *count;
    pset_family G;
    pset p, last, restrict;
    restrict = set_new(A->sf_size);

    /* Find a "maximal" independent set of sets */
    if (compute_indep_set) {
	G = family_to_graph(A);
	maxclique_greedy(G, indep_set);
	sf_free(G);
	if (debug & MINCOV)
	    printf("CLIQUE_SELECT: %d sets of size %d; %d are independent\n",
		A->active_count, A->sf_size, set_ord(indep_set));
    }

    /* Union over all "independent" sets will be the restricted set */
    index = 0;
    foreach_active_set(A, last, p) {
	if (is_in_set(indep_set, index)) {
	    SET(p, PRIME);			/* mark set as indep. set */
	    INLINEset_or(restrict, restrict, p);
	} else
	    RESET(p, PRIME);		/* mark set as dep. set */
	index++;
    }

    if (weighted_select) {
	/* simple column weight strategy */
	count = sf_weight_count(A);
	while (! setp_empty(restrict))
	    greedy_select(A, count, select, restrict);
	mem_free((char *) count);
    } else
	/* compute weights based on size of intersection to indep set */
	while (! setp_empty(restrict)) {
	    count = cl_weight(A, restrict);
	    greedy_select(A, count, select, restrict);
	    mem_free((char *) count);
	}

    set_free(restrict);
}
/*
    family_to_graph -- Create a graph where the nodes are sets of A and
    an edge exists between two nodes if the two sets contain no
    elements in common.

    A clique in this graph is a set of sets all of which have no
    elements in common.  A maximum-clique corresponds to a maximum
    independent set of sets of A.

    A set family is used to represent the graph in an adjacency matrix
    form.  The graph is symmetric, but the full matrix is still
    stored to improve the efficiency of finding a large clique.

    Note that the call to setp_disjoint is expanded in-line; also, the
    macros foreach_active_set have been expanded to allow for half as
    many calls.
*/

pset_family family_to_graph(A)
pset_family A;
{
    pset_family G;
    register pset pi, pj;
    register int indexi, indexj, k, ws = A->wsize;
    int num;

    /* Allocate space for the adjacency matrix of the graph */
    G = sf_new(A->active_count, A->active_count);
    num = G->count = A->active_count;

    /* Clear all sets in the set family G */
    foreachi_set(G, indexi, pi)
	INLINEset_clear(pi, G->sf_size);

    /* Loop over all sets (outer loop) */
    for(pi = A->data, indexi = 0; indexi < num-1; pi += ws)
	if (TESTP(pi, ACTIVE)) {
	    /* Loop over all sets (inner loop) */
	    for(pj = pi + ws, indexj = indexi+1; indexj < num; pj += ws)
	        if (TESTP(pj, ACTIVE)) {
		    k=LOOP(pi); do if (pi[k]&pj[k]) goto next; while (--k>0);
			/* The sets are disjoint */
		        set_insert(GETSET(G,indexi), indexj);
		        set_insert(GETSET(G,indexj), indexi);
		    next: indexj++;
		}
	    indexi++;
	}

    return G;
}
/*
    maxclique_greedy -- simple routine to find a large clique in a graph

    Finding the largest clique in a graph is again a difficult problem,
    so the following greedy heuristic is used:
	1) Pick the node of largest degree in the graph and add to clique
	2) Delete any nodes which are not completely connected to the clique
	3) If any nodes left in graph, go back to 1

    A simple tie-breaking rule is used in step 1 if two nodes have the
    same degree.  We restrict the graph to just these nodes, and ask
    which node has the largest degree in this restriction.
*/
void maxclique_greedy(G, clique)
register pset_family G;
pset clique;
{
    register pset p;
    register int i, best, bestd, degree;
    pset potential, conset, temp;
    potential = set_fill(set_new(G->sf_size), G->sf_size);
    conset = set_new(G->sf_size); temp = set_new(G->sf_size);

    sf_active(G);		/* make all members of G active */
    set_clear(clique, G->sf_size);
    while (G->active_count != 0) {
	/* Find which nodes have largest degree within potential set */
	best = -1;
	foreachi_active_set(G, i, p)
	    if ((degree = set_dist(p, potential)) > best) {
		set_clear(conset, G->sf_size);
		set_insert(conset, i);
		best = degree;
	    } else if (degree == best)
		set_insert(conset, i);

	/* Now chose element of largest degree within conset */
	best = bestd = -1;
	foreachi_active_set(G, i, p)
	    if (is_in_set(conset,i))
		if ((degree = set_dist(p, conset)) > bestd)
		    best = i, bestd = degree;

	/* Remove sets from the potential clique set */
        set_insert(clique, best);
	set_remove(potential, best);
	G->active_count--; RESET(GETSET(G, best), ACTIVE);
	foreachi_active_set(G, i, p)
	    if (! setp_empty(set_diff(temp, clique, p))) {
		G->active_count--; RESET(p, ACTIVE);
		set_remove(potential, i);
	    }
    }
    set_free(potential); 
    set_free(conset); 
    set_free(temp);
}
/*
    The clique greedy routine has the unfortunate property that is does
    not guarantee a minimal covering (a covering is called minimal if
    every element of the covering is essential to cover some set).
*/

void weed(A, select)
pset_family A;
pset select;
{
    int *count;
    register pset p, last;
    pset temp, essen;
    temp = set_new(A->sf_size); essen = set_new(A->sf_size);

    /* Find which elements of "select" are nonessential in some set */
    foreach_set(A, last, p)
	if (set_ord(set_and(temp, p, select)) == 1)
	    INLINEset_or(essen, essen, temp);

    if (! setp_empty(set_diff(temp, select, essen))) {
	if (debug & MINCOV)
	    printf("WEED: %d nonessential elements\n", set_ord(temp));
	/* Make active those sets not covered by the essential elements */
	A->active_count = 0;
	foreach_set(A, last, p)
	    if (setp_disjoint(p, essen))
		A->active_count++, SET(p, ACTIVE);

	/* Now apply a simple greedy strategy to these sets */
	count = sf_weight_count(A);
	while (A->active_count != 0)
	    greedy_select(A, count, essen, (pset) NULL);
	mem_free((char *) count);
    }

    set_copy(select, essen);
    set_free(temp); set_free(essen);
}


int *sf_weight_count(A)
pset_family A;
{
    register pset p, last;
    register int i, base, *count = (int *) alloc(A->sf_size*sizeof(int));
    register unsigned int val;
    int r;
    for(i = 0, base = A->sf_size; i < base; i++)
	count[i] = 0;
    foreach_active_set(A, last, p)
	for(r = SCALE / set_ord(p), i = LOOP(p); i > 0;)
	    for(val = p[i], base = --i << LOGBPI; val != 0; base++, val >>= 1)
		if (val & 1)
		    count[base] += r;
    return count;
}


int *cl_weight(A, restrict)
register pset_family A;
pset restrict;
{
    register pset p, last;
    register int i, base, *count = (int *) alloc(A->sf_size*sizeof(int));
    register unsigned int val;
    int r;
    for(i = 0, base = A->sf_size; i < base; i++)
	count[i] = 0;
    foreach_active_set(A, last, p) 
	if ((r = set_dist(p, restrict)) != 0)
	    for(r = SCALE / r, i = LOOP(p); i > 0;)
	        for(val=p[i], base = --i << LOGBPI; val !=0; base++,val >>= 1)
		    if (val & 1)
			count[base] += r;
    return count;
}


/* greedy_select -- scan the weight array to find the best variable */
void greedy_select(A, count, select, restrict)
pset_family A;
register int count[];
pset select, restrict;
{
    register pset p, last;
    register int element, best_element;
    int best;

    /* Find element of maximum weight which is also in "restrict" */
    best = 0; best_element = -1;
    for(element = 0; element < A->sf_size; element++)
	if (restrict == NULL || is_in_set(restrict, element))
	    if (count[element] > best)
		best = count[element], best_element = element;
    if (best_element == -1)
	fatal("mincov.c/greedy_select: some problem here");

    /* Add this element to the selected set */
    if (debug & MINCOV1)
        printf("GREEDY_SELECT: best is %d with weight of %d\n",
	    best_element, count[best_element]);
    set_insert(select, best_element);

    /* Mark as inactive those sets of A which are newly covered */
    foreach_active_set(A, last, p)
        if (is_in_set(p, best_element)) {
	    set_adjcnt(p, count, (int) (-SCALE / set_ord(p)));
	    if (restrict != NULL && TESTP(p, PRIME))
		INLINEset_diff(restrict, restrict, p);
	    A->active_count--; RESET(p, ACTIVE);
	}
}
pset_family exact_minimum_cover(T)
IN pset_family T;
{
    register pset p, last, p1;
    register int i, n;
    int lev, lvl;
    pset nlast;
    pset_family temp;
    double start = ptime();
    struct {
	pset_family sf;
	int level;
    } stack[32];		/* 32 suffices for 2 ** 32 cubes ! */

    if (T->count <= 0)
	return sf_new(1, T->sf_size);
    for(n = T->count, lev = 0; n != 0; n >>= 1, lev++)   ;

    /* A simple heuristic ordering */
    T = lex_sort(sf_save(T));

    /* Push a full set on the stack to get things started */
    n = 1;
    stack[0].sf = sf_new(1, T->sf_size);
    stack[0].level = lev;
    set_fill(GETSET(stack[0].sf, stack[0].sf->count++), T->sf_size);

    nlast = GETSET(T, T->count - 1);
    foreach_set(T, last, p) {

	/* "unstack" the set into a family */
	temp = sf_new(set_ord(p), T->sf_size);
	for(i = 0; i < T->sf_size; i++)
	    if (is_in_set(p, i)) {
		p1 = set_fill(GETSET(temp, temp->count++), T->sf_size);
		set_remove(p1, i);
	    }
	stack[n].sf = temp;
	stack[n++].level = lev;

	/* Pop the stack and perform (leveled) intersections */
	while (n > 1 && (stack[n-1].level==stack[n-2].level || p == nlast)) {
	    temp = sf_intersect(stack[n-1].sf, stack[n-2].sf);
	    lvl = min(stack[n-1].level, stack[n-2].level) - 1;
	    if (debug & MINCOV && lvl < 10) {
		printf("# EXACT_MINCOV[%d]: %4d = %4d x %4d, time = %s\n",
		    lvl, temp->count, stack[n-1].sf->count,
		    stack[n-2].sf->count, print_time(ptime() - start));
		(void) fflush(stdout);
	    }
	    sf_free(stack[n-2].sf);
	    sf_free(stack[n-1].sf);
	    stack[n-2].sf = temp;
	    stack[n-2].level = lvl;
	    n--;
	}
    }

    temp = stack[0].sf;
    p1 = set_fill(set_new(T->sf_size), T->sf_size);
    foreach_set(temp, last, p)
	INLINEset_diff(p, p1, p);
    set_free(p1);
    if (debug & MINCOV1) {
	printf("MINCOV: family of all minimal coverings is\n");
	sf_print(temp);
    }
    sf_free(T);		/* this is the copy of T we made ... */
    return temp;
}

#define MAGIC 500		/* save 500 cubes before containment */
pset_family sf_intersect(A, B)
pset_family A, B;
{
    register pset pi, pj, lasti, lastj, pt;
    pset_family T, Tsave = NULL;

    /* How large should each temporary result cover be ? */
    T = sf_new(MAGIC, A->sf_size);
    pt = T->data;

    /* Form pairwise intersection of each cube of A with each cube of B */
    foreach_set(A, lasti, pi)
	foreach_set(B, lastj, pj) {
	    INLINEset_and(pt, pi, pj);
	    if (++T->count >= T->capacity) {
		if (Tsave == NULL)
		    Tsave = sf_contain(T);
		else
		    Tsave = sf_union(Tsave, sf_contain(T));
		T = sf_new(MAGIC, A->sf_size);
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
