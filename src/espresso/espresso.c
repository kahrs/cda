/*
    Module: espresso.c
    Purpose: The main espresso algorithm

    Returns a minimized version of the ON-set of a function

    The following global variables affect the operation of Espresso:
        trace -- print trace information as the minimization progresses
	remove_essential -- remove essential primes
	single_expand -- if true, stop after very first expand
*/

#include "espresso.h"

pcover espresso(F, D, R)
pcover F, D, R;
{
    pcover E, G, Dsave;
    cost_t cost, best_cost;
    int start, forever = TRUE;

    /* Make a copy of D for our own use -- leave original D intact */
    Dsave = D;
    D = sf_save(Dsave);

    /* Setup has always been a problem -- unravel and contain here */
    cover_cost(F, &cost);
    if ((cube.part_size[cube.num_vars - 1] > 1) && (cost.out < 5000))
        EXEC(F = sf_contain(unravel(F, cube.num_vars - 1)), "SETUP      ", F);

    /* Initial expand and irredundant */
    EXECUTE( F = expand(F, R, FALSE, FALSE), EXPAND_TIME, F);
    EXECUTE( F = irredundant(F, D), IRRED_TIME, F);
    if (single_expand)
	goto skip_loop;
    if (remove_essential && cube.num_binary_vars == cube.num_vars - 1) {
        EXECUTE( E = essential(&F, &D), ESSEN_TIME, E);
    } else
	E = new_cover(0);
    cover_cost(F, &best_cost);

    do {
	/* Repeat inner loop until solution becomes "stable" */
	do {
	    start = F->count;
	    EXECUTE(F = reduce(F, D), REDUCE_TIME, F);
	    EXECUTE_BREAK(F = expand(F, R, FALSE, FALSE), EXPAND_TIME, F);
	    EXECUTE_BREAK(F = irredundant(F, D), IRRED_TIME, F);
	    copy_cost(&cost, &best_cost);
	} while (F->count < start);

	/* Perturb solution to see if we can continue to iterate */
	EXECUTE( G = reduce_gasp(F, D), GREDUCE_TIME, G);
	EXECUTE( G = expand_gasp(G, R), GEXPAND_TIME, G);
	EXECUTE_BREAK( F = irred_gasp(F, D, G), GIRRED_TIME, F);
	copy_cost(&cost, &best_cost);

    } while (forever);

    /* Append the essential cubes to F */
    F = sf_append(F, E);
    if (trace) size_stamp(F, "ADJUST     ");

    /* Remove redundant parts from the multiple-valued variables */
skip_loop:
    cover_cost(F, &best_cost);
    do {
	EXECUTE_BREAK( F = mv_reduce(F, Dsave), MV_REDUCE_TIME, F);
	copy_cost(&cost, &best_cost);
	EXECUTE_BREAK( F = expand(F, R, FALSE, TRUE), RAISE_IN_TIME, F);
	copy_cost(&cost, &best_cost);
    } while (force_irredundant);

    return F;
}
