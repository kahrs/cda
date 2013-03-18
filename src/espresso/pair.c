#include "espresso.h"

/* sf_delcol -- add or delete columns from a set family

    if n > 0 then n columns starting from firstcol are deleted
    if n < 0 then n blank columns are inserted starting at firstcol
	(i.e., the first new column number is firstcol)
*/
pset_family sf_delcol(A, firstcol, n)
pset_family A;
register int firstcol, n;
{
    register pset p, last, pdest;
    register int i;
    pset_family B;

    B = sf_new(A->count, A->sf_size - n);
    foreach_set(A, last, p) {
	pdest = set_clear(GETSET(B, B->count++), B->sf_size);
	for(i = 0; i < firstcol; i++)
	    if (is_in_set(p, i))
		set_insert(pdest, i);
	for(i = n > 0 ? firstcol + n : firstcol; i < A->sf_size; i++)
	    if (is_in_set(p, i))
		set_insert(pdest, i - n);
    }
    sf_free(A);

    return B;
}


pPLA set_pair(PLA)
pPLA PLA;
{
    int i, var, *paired, *oldsiz, oldvar, oldbinvar;
    ppair pair = PLA->pair;

    paired = (int *) alloc(cube.num_binary_vars * sizeof(bool));
    for(var = 0; var < cube.num_binary_vars; var++)
	paired[var] = FALSE;
    for(i = 0; i < pair->cnt; i++) {
	if (pair->var1[i] > 0 && pair->var1[i] <= cube.num_binary_vars)
	    paired[pair->var1[i]-1] = TRUE;
	else
	    fatal("espresso: can only pair binary-valued variables");
	if (pair->var2[i] > 0 && pair->var2[i] <= cube.num_binary_vars)
	    paired[pair->var2[i]-1] = TRUE;
	else
	    fatal("espresso: can only pair binary-valued variables");
    }

    PLA->F = delvar(pairvar(PLA->F, pair), paired);
    PLA->R = delvar(pairvar(PLA->R, pair), paired);
    PLA->D = delvar(pairvar(PLA->D, pair), paired);

    /* Now painfully adjust the cube size */
    oldsiz = cube.part_size;
    oldvar = cube.num_vars;
    oldbinvar = cube.num_binary_vars;

    cube.num_binary_vars = 0;
    for(var = 0; var < oldbinvar; var++)
	cube.num_binary_vars += (paired[var] == FALSE);

    cube.num_vars = oldvar - oldbinvar + cube.num_binary_vars + pair->cnt;

    cube.part_size = (int *) alloc(cube.num_vars*sizeof(int));
    for(var = 0; var < pair->cnt; var++)
	cube.part_size[cube.num_binary_vars + var] = 4;
    for(var = 0; var < oldvar-oldbinvar; var++)
	cube.part_size[cube.num_binary_vars + pair->cnt + var] =
	    oldsiz[oldbinvar + var];
    cube_setup();
    /* the paired variables should not be sparse (cf. mv_reduce/raise_in)*/
    for(var = 0; var < pair->cnt; var++)
	cube.sparse[cube.num_binary_vars + var] = 0;
    mem_free((char *) paired);
}


pcover pairvar(A, pair)
pcover A;
ppair pair;
{
    static int lookup[16][4] = {
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
	0,0,0,0, 1,0,0,0, 0,1,0,0, 1,1,0,0,
	0,0,0,0, 0,0,1,0, 0,0,0,1, 0,0,1,1,
	0,0,0,0, 1,0,1,0, 0,1,0,1, 1,1,1,1
    };
    register pcube last, p;
    int insert_col, p1, p2, v1, v2, val, i, pairnum;

    insert_col = cube.first_part[cube.num_vars - 1];

    /* stretch the cover matrix to make room for the paired variables */
    A = sf_delcol(A, insert_col, -4*pair->cnt);

    /* compute the paired values */
    foreach_set(A, last, p)
	for(pairnum = 0; pairnum < pair->cnt; pairnum++) {
	    p1 = cube.first_part[pair->var1[pairnum]-1]; 
	    p2 = cube.first_part[pair->var2[pairnum]-1];
	    v1 = (is_in_set(p, p1+1) != 0)*2 + (is_in_set(p, p1) != 0);
	    v2 = (is_in_set(p, p2+1) != 0)*2 + (is_in_set(p, p2) != 0);
	    val = v1*4 + v2;
	    for(i = 0; i < 4; i++)
		if (lookup[val][3-i])
	    	    set_insert(p, insert_col + pairnum*4 + i);
	}
    return A;
}


/* delvar -- delete variables from A, minimize the number of column shifts */
pcover delvar(A, paired)
pcover A;
bool paired[];
{
    bool run; int first_run, run_length, var, offset = 0;
            
    run = FALSE; run_length = 0;
    for(var = 0; var < cube.num_binary_vars; var++)
	if (paired[var])
	    if (run)
		run_length += cube.part_size[var];
	    else {
		run = TRUE;
		first_run = cube.first_part[var];
		run_length = cube.part_size[var];
	    }
	else
	    if (run) {
		A = sf_delcol(A, first_run-offset, run_length);
		run = FALSE;
		offset += run_length;
	    }
    if (run)
	A = sf_delcol(A, first_run-offset, run_length);
    return A;
}
