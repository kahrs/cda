#include "espresso.h"

int power(b, n)
int b, n;
{
    int i, r = 1;
    for(i = 0; i < n; i++)
	r *= b;
    return r;
}

void allphase(PLA, first_output, last_output)
IN pPLA PLA;			/* sets phase variable to best phase */
IN int first_output, last_output;
{
    pcover F, D, R;
    int i, l, num, best, last, n;
    pcube bestphase;

    n = last_output - first_output + 1;
    last = power(2, n);
    PLA->phase = set_save(cube.fullset);
    best = (PLA->F)->count;
    bestphase = set_save(PLA->phase);

    PLA->F = espresso(PLA->F, PLA->D, PLA->R);
    printf("# phase is on-set, solution has %d cubes\n", (PLA->F)->count);
    PLA->R = espresso(PLA->R, PLA->D, PLA->F);
    printf("# phase is off-set, solution has %d cubes\n", (PLA->R)->count);

    for(i = 1; i < last-1; i++) {

	/* save the current PLA (covers) */
	F = sf_save(PLA->F); 
	D = sf_save(PLA->D);
	R = sf_save(PLA->R);

	/* compute the phase cube for this iteration */
	set_copy(PLA->phase, cube.fullset);
	num = i;
	l = cube.first_part[cube.num_vars - 1] + first_output;
	do {
	    if (num % 2 == 1)
		set_remove(PLA->phase, l);
	    l++;
	} while ((num /= 2) > 0);

	/* set the phase and call espresso */
	set_phase(PLA);
EXEC(PLA->F=espresso(PLA->F,PLA->D,PLA->R), "ESPRESSO   ", PLA->F);
	printf("# phase is %s, solution has %d cubes\n",
	    pc1(PLA->phase), (PLA->F)->count);

	/* see if this is the best so far */
	if ((PLA->F)->count < best) {
	    best = (PLA->F)->count;
	    set_copy(bestphase, PLA->phase);
	}

	/* now throw away the solution, and restore original covers */
	free_cover(PLA->F);
	free_cover(PLA->D);
	free_cover(PLA->R);
	PLA->F = F;
	PLA->D = D;
	PLA->R = R;
    }

    /* one more minimization to restore the best answer */
    PLA->phase = bestphase;
    set_phase(PLA);
    EXEC(PLA->F=espresso(PLA->F,PLA->D,PLA->R), "ESPRESSO   ", PLA->F);
}


pcover isolate(T, i)
pcover T;
register int i;
{
    pcover T1;
    register int j;
    register pcube pdest, p, last;

    T1 = new_cover(T->count);
    foreach_set(T, last, p)
	if (is_in_set(p, i)) {
	    pdest = set_copy(GETSET(T1, T1->count++), cube.emptyset);
	    for(j = 0; j < cube.size; j++)
		if (is_in_set(p, j))
		    set_insert(pdest, j);
	}
    return T1;
}

/* check fct vs complement for opo */
void minall(PLA)
pPLA PLA;
{
    int i, lastpart, firstpart;
    register pcover F1, R1, D1;

    PLA->phase = set_save(cube.fullset);
    firstpart = cube.first_part[cube.num_vars - 1];
    lastpart = cube.last_part[cube.num_vars - 1];
    cube.num_vars -= 1;
    cube_setup();

    for(i = firstpart; i <= lastpart; i++) {
	F1 = isolate(PLA->F, i);
	R1 = isolate(PLA->R, i);
	D1 = isolate(PLA->D, i);

	F1 = espresso(F1, D1, R1);
	R1 = espresso(R1, D1, F1);

	printf("# output # %d: on = %4d   off= %4d\n",
	    i-firstpart, F1->count, R1->count);

	if (F1->count > R1->count)
	    set_remove(PLA->phase, i);
	free_cover(F1);
	free_cover(R1);
	free_cover(D1);
    }

    cube.num_vars += 1;
    cube_setup();
    set_phase(PLA);
    PLA->F = espresso(PLA->F, PLA->D, PLA->R);
}
/*
    Phase assignment technique (T. Sasao):
	1. create a  with 2*m outputs which realizes the original
	 and its complement for each output
	2. minimize this 
	3. choose a minimum number of prime implicants from this minimization
	to realize each output or its complement
*/
void phase_assignment(PLA)
IN pPLA PLA;
{
    pPLA PLA1;

    PLA1 = (pPLA) alloc(sizeof(PLA_t));
    PLA1->F = sf_save(PLA->F);
    PLA1->R = sf_save(PLA->R);
    PLA1->D = sf_save(PLA->D);
    PLA1 = output_phase_setup(PLA1);

    PLA->phase = opo(espresso(PLA1->F, PLA1->D, PLA1->R));
    free_cover(PLA1->D); free_cover(PLA1->R);
    set_phase(PLA);
    PLA->F = espresso(PLA->F, PLA->D, PLA->R);
}

/*
   here is the hard work -- find a minimum subset of the primes
*/
pcube opo(T)
pcover T;
{
    int i, j, ilast, n, lev, size, index, out, first;
    pset_family temp;
    pcover T1, Y; pcube ceil, last, phase;
    pset p, pdest;
    double start = ptime();
    struct {
	pset_family sf;
	int level;
    } stack[32];		/* 32 suffices for 2 ** 32 cubes ! */

    /* Balanced merge */
    size = cube.part_size[cube.num_vars - 1];
    temp = stack[0].sf = sf_new(1, T->sf_size);
    set_fill(GETSET(temp, temp->count++), T->sf_size);
    stack[0].level = 0;
    n = 1;
    ilast = size/2 - 1;

    /* Loop for each output (of the original function) */
    for(i = 0; i < size/2; i++) {
	stack[n].sf = temp = sf_new(2, T->count);
	stack[n++].level = 0;

	/* Find which primes are needed for the ON-set of this fct */
	index = cube.first_part[cube.num_vars - 1] + i;
	set_fill(pdest = GETSET(temp, temp->count++), T->count);
	foreachi_set(T, j, p)
	    if (is_in_set(p, index))
		set_remove(pdest, j);

	/* Find which primes are needed for the OFF-set of this fct */
	index = cube.first_part[cube.num_vars - 1] + i + size/2;
	set_fill(pdest = GETSET(temp, temp->count++), T->count);
	foreachi_set(T, j, p)
	    if (is_in_set(p, index))
		set_remove(pdest, j);

   	/* Pop the stack and perform the intersections */
	while (n > 1 && (stack[n-1].level==stack[n-2].level || i == ilast)) {
	    temp = sf_intersect(stack[n-1].sf, stack[n-2].sf);
	    lev = max(stack[n-1].level, stack[n-2].level) + 1;
	    if (debug & MINCOV) {
		printf("# OPO[%d]: %4d = %4d x %4d, time = %s\n",
		    lev, temp->count, stack[n-1].sf->count,
		    stack[n-2].sf->count, print_time(ptime() - start));
		(void) fflush(stdout);
	    }
	    free_cover(stack[n-2].sf);
	    free_cover(stack[n-1].sf);
	    stack[n-2].sf = temp;
	    stack[n-2].level = lev;
	    n--;
	}
    }

    /* largest set is on top */
    pdest = GETSET(stack[0].sf, 0);
    if (debug & MINCOV1)
	sf_print(stack[0].sf);
    T1 = new_cover(T->count);
    foreachi_set(T, j, p)
	if (! is_in_set(pdest, j))
	    T1 = sf_addset(T1, p);


    Y = cv_sharp(T, T1);
    ceil = new_cube();
    first = cube.first_part[cube.num_vars - 1];
    foreach_set(Y, last, p)
	set_or(ceil, ceil, p);

    cube.part_size[cube.num_vars - 1] -= size/2;
    cube_setup();
    phase = set_save(cube.fullset);
    for(out = first; out < first + size/2; out++)
	if (! is_in_set(ceil, out))
	    set_remove(phase, out+size/2);
	else if (! is_in_set(ceil, out+size/2))
	    set_remove(phase, out);
	else
	    fatal("espresso: error in output phase assignment");
    free_cover(T);
    free_cover(T1);
    free_cover(Y);
    return phase;
}
/*
    Take a PLA (ON-set, OFF-set and DC-set) and create the "double-phase
    characteristic function" which is merely a new function which has
    twice as many outputs and realizes both the function and the complement.
*/
pPLA output_phase_setup(PLA)
INOUT pPLA PLA;
{
    pcover F = PLA->F, R = PLA->R, D = PLA->D;
    register pcube last, p;
    register int i, last_part, numout;
    pcube pr, pf, pd, mask = cube.temp[0];
    int first_part;

    first_part = cube.first_part[cube.num_vars - 1];
    last_part = cube.last_part[cube.num_vars - 1];
    numout = cube.part_size[cube.num_vars - 1];
    cube.part_size[cube.num_vars - 1] += numout;
    cube_setup();
    set_diff(mask, cube.fullset, cube.var_mask[cube.num_vars - 1]);

    PLA->F = new_cover(F->count + R->count);
    PLA->R = new_cover(F->count + R->count);
    PLA->D = new_cover(D->count);

    foreach_set(F, last, p) {
	pf = GETSET(PLA->F, (PLA->F)->count++);
	pr = GETSET(PLA->R, (PLA->R)->count++);
	INLINEset_and(pf, mask, p);	
	INLINEset_and(pr, mask, p);
	for(i = first_part; i <= last_part; i++)
	    if (is_in_set(p, i))
		set_insert(pf, i);	
	for(i = first_part; i <= last_part; i++)
	    if (is_in_set(p, i))
		set_insert(pr, i+numout);	
    }

    foreach_set(R, last, p) {
	pf = GETSET(PLA->F, (PLA->F)->count++);
	pr = GETSET(PLA->R, (PLA->R)->count++);
	INLINEset_and(pf, mask, p);
	INLINEset_and(pr, mask, p);
	for(i = first_part; i <= last_part; i++)
	    if (is_in_set(p, i))
		set_insert(pf, i+numout);
	for(i = first_part; i <= last_part; i++)
	    if (is_in_set(p, i))
		set_insert(pr, i);
    }

    foreach_set(D, last, p) {
	pd = GETSET(PLA->D, (PLA->D)->count++);
	INLINEset_and(pd, mask, p);
	for(i = first_part; i <= last_part; i++)
	    if (is_in_set(p, i)) {
		set_insert(pd, i);
		set_insert(pd, i+numout);
	    }
    }

    free_cover(F);
    free_cover(D);
    free_cover(R);

    return PLA;
}
