/*
    Module: main.c
    Purpose: Main driver for minimization routines

    This is a complex main program supporting what is actually about 30
    different programs.  The options available are listed in "usage" which
    is usually kept up-to-date.
*/

#define EXTERN
#define INIT
#include "espresso.h"

void runtime()
{
    int i;
    double total = 0.001;

    for(i = 0; i < TIME_COUNT; i++)
    	total += total_time[i];
    for(i = 0; i < TIME_COUNT; i++)
        if (total_calls[i] != 0)
	    printf("# %s\t%2d call(s) for %s (%3.1f%%)\n",
	    total_name[i], total_calls[i], print_time(total_time[i]),
	    total_time[i]/total*100.0);
}


init_runtime()
{
    total_name[READ_TIME] =	"READ       ";
    total_name[COMPL_TIME] =	"COMPL      ";
    total_name[REDUCE_TIME] =	"REDUCE     ";
    total_name[EXPAND_TIME] =	"EXPAND     ";
    total_name[ESSEN_TIME] =	"ESSEN      ";
    total_name[IRRED_TIME] =	"IRRED      ";
    total_name[GREDUCE_TIME] =	"REDUCE_GASP";
    total_name[GEXPAND_TIME] =	"EXPAND_GASP";
    total_name[GIRRED_TIME] =	"IRRED_GASP ";
    total_name[MV_REDUCE_TIME] ="MV_REDUCE  ";
    total_name[RAISE_IN_TIME] =	"RAISE_IN   ";
    total_name[VERIFY_TIME] =	"VERIFY     ";
}

#define option(a) (equal(what_to_do, a))

usage()
{
    printf("%s\n\n", VERSION);
    printf("SYNOPSIS:\n\tespresso [type] [file] [options]\n\n");
    printf("[type] provides the logical form of the input file\n");
    printf("\t-f\tFile specifies ON-set\n");
    printf("\t-fd\tFile specifies ON-set and DC-set (default)\n");
    printf("\t-fr\tFile specifies ON-set and OFF-set\n");
    printf("\t-fdr\tFile specifies ON-set, OFF-set, and DC-set\n\n");
    printf("[options] allowed are:\n");
    printf("\t-d\tProvide verbose detail of program execution\n");
    printf("\t-fast\tStop after first EXPAND/IRRED\n");
    printf("\t-ness\tDo not remove essential primes\n");
    printf("\t-nirr\tDo not force the result to be irredundant\n");
    printf("\t-out xx\tSpecify output format as F, FD, FR, or FDR\n");
    printf("\t-pos\tSwap ON-set and OFF-set\n");
    printf("\t-s\tProvide short execution summary\n");
    printf("\t-t\tProvide longer execution trace\n");
    printf("\t-x\tSuppress printing of solution\n");
    printf(
    "\t-do [s]\tPerform alternate function [s], where [s] is one of\n");
    printf(
    "\t\tESPRESSO, check, compact, contain, echo, essen, expand,\n");
    printf(
    "\t\tintersect, irred, lexsort, map, mincov, mv_reduce, pop,\n");
    printf(
    "\t\tprimes, qm, reduce, sharp, stats, taut, union, unravel, verify\n");
    exit(0);
}

main(argc, argv)
int argc;
char *argv[];
{
    pPLA PLA, PLA1;
    int in_type, out_type = F_type, i, j, first_output, last_output;
    bool readone, readtwo, verify_error = FALSE, print_solution;
    char what_to_do[256];
    cost_t cost, best_cost;
    double start = ptime();

    init_runtime();

    /* Check for the help option */
    if (check_arg(&argc, argv, "-help") || check_arg(&argc, argv, "?"))
	usage();

    /* Scan the argument list for something to do (default is ESPRESSO) */
    strcpy(what_to_do, "ESPRESSO");
    for(i = 1; i < argc-1; i++)
	if (equal(argv[i], "-do")) {
	    strcpy(what_to_do, argv[i+1]);
	    delete_arg(&argc, argv, i+1);
	    delete_arg(&argc, argv, i);
	    break;
	}

    /* Scan the argument list for the output format */
    if (option("echo"))
	out_type = FDR_type;
    for(i = 1; i < argc-1; i++)
	if (equal(argv[i], "-out")) {
	    for(j = 0; pla_types[j].key != 0; j++)
	 	if (equal(pla_types[j].key+1, argv[i+1])) {
		    out_type = pla_types[j].value;
		    delete_arg(&argc, argv, i+1);
		    delete_arg(&argc, argv, i);
		    break;
		}
	    if (pla_types[j].key == 0) {
		fprintf(stderr, "espresso: Unknown output format %s\n",
		    argv[i+1]);
		exit(-1);
	    } 
	    break;
        }

    /* Check for other miscellaneous options */
    chk_debug(&argc, argv);

    /* Espresso minimization options */
    single_expand	= check_arg(&argc, argv, "-fast");
    remove_essential	= ! check_arg(&argc, argv, "-ness");
    force_irredundant	= ! check_arg(&argc, argv, "-nirr");
    pos			= check_arg(&argc, argv, "-pos");
    kiss		= check_arg(&argc, argv, "-kiss");

    /* Miscellaneous options */
    print_solution	= ! check_arg(&argc, argv, "-x");
    summary = check_arg(&argc, argv, "-s") || option("stats");
    trace = check_arg(&argc, argv, "-t") || (debug != 0);
    mincov_exact = check_arg(&argc, argv, "-exact");

    /* Decide how many PLAs to read from files */
    readone = ! (option("mincov") || option("mincov1"));
    readtwo = option("union") || option("intersect") || option("sharp") ||
	option("verify");
    /* Decide if function needs offset and set in_type accordingly */
    if (option("ESPRESSO") || option("expand") || option("echo") || 
	option("mv_reduce") || option("primes") || option("qm") || 
	option("newexpand") || option("opo") || option("badopo") ||
	option("check"))
	in_type = FD_type;
    else
	in_type = FDR_type;
    if (summary || trace)
	printf("# %s\n", VERSION);

    /* Read the (first) or only PLA */
    if (readone || readtwo)
	PLA = read_pla(&argc, argv, in_type);

    /* Read the second PLA if one is needed */
    if (readtwo)
	PLA1 = read_pla(&argc, argv, in_type);

    if (option("allphase")) {
	first_output = atol(argv[1]);
	last_output = atol(argv[2]);
	argc -= 2;
    }
    chk_arglist(&argc, argv);


/*
    Now a giant IF-statement to decide what to do
*/

    /* Espresso minimization */
    if (option("ESPRESSO")) {
	pcover Fold = sf_save(PLA->F);
	PLA->F = espresso(PLA->F, PLA->D, PLA->R);
	EXECUTE(verify_error=verify(PLA->F,Fold,PLA->D), VERIFY_TIME, PLA->F);
	if (verify_error) {
	    PLA->F = Fold;
	    print_solution = FALSE;
	    (void) check_consistency(PLA);
   	}

    /* Test drivers for each step of the ESPRESSO algorithm */
    } else if (option("essen")) {
        EXECUTE(PLA->F = essential(&(PLA->F), &(PLA->D)), ESSEN_TIME, PLA->F);
    } else if (option("expand")) {
	pcover Fold = sf_save(PLA->F);
	EXECUTE(PLA->F=expand(PLA->F,PLA->R,FALSE,FALSE),EXPAND_TIME, PLA->F);
	EXECUTE(verify_error=verify(PLA->F,Fold,PLA->D), VERIFY_TIME, PLA->F);
    } else if (option("irred")) {
	pcover Fold = sf_save(PLA->F);
        EXECUTE(PLA->F = irredundant(PLA->F, PLA->D), IRRED_TIME, PLA->F);
	EXECUTE(verify_error=verify(PLA->F,Fold,PLA->D), VERIFY_TIME, PLA->F);
    } else if (option("reduce")) {
	pcover Fold = sf_save(PLA->F);
        EXECUTE(PLA->F = reduce(PLA->F, PLA->D), REDUCE_TIME, PLA->F);
	EXECUTE(verify_error=verify(PLA->F,Fold,PLA->D), VERIFY_TIME, PLA->F);
    } else if (option("taut")) {
	printf("ON-set is%sa tautology\n",
	    tautology(cube1list(PLA->F)) ? " " : " not ");
	print_solution = FALSE;
    } else if (option("verify")) {
	pcover Fold = PLA->F, F = PLA1->F, Dold = PLA->D;
	EXECUTE(verify_error=verify(F, Fold, Dold), VERIFY_TIME, PLA->F);
    } else if (option("mv_reduce")) {
	pcover F = PLA->F, D = PLA->D, R = PLA->R;
	cover_cost(PLA->F, &best_cost);
	do {
	    EXECUTE_BREAK( F = mv_reduce(F, D), MV_REDUCE_TIME, F);
	    copy_cost(&cost, &best_cost);
	    EXECUTE_BREAK( F = expand(F, R, FALSE, TRUE), RAISE_IN_TIME, F);
	    copy_cost(&cost, &best_cost);
	} while (force_irredundant);
	PLA->F = F;
    } else if (option("mincov") || option("mincov1")) {
    	int best;
	pset_family A = read_set_family(stdin);
	pset c;
	if (option("mincov"))
	    c = minimum_cover(A, &best);
/*	else
	    c = exact_minimum_cover(A);*/
	printf("minimum cover has %d elements\n", set_ord(c));
	sf_free(A);
	set_free(c);
	print_solution = FALSE;
    }

    /* The presto/pop algorithm using unate tautology */
    else if (option("pop")) {
	pcover Fold = sf_save(PLA->F); bool better;
	EXECUTE(better = pop_expand(&(PLA->F), PLA->D), EXPAND_TIME, PLA->F);
	do {
	    EXECUTE(better=pop_reduce(&(PLA->F),PLA->D), REDUCE_TIME, PLA->F);
	    if (! better)
		break;
	    EXECUTE(better=pop_expand(&(PLA->F),PLA->D), EXPAND_TIME, PLA->F);
	} while (better);
	EXECUTE(verify_error=verify(PLA->F,Fold,PLA->D), VERIFY_TIME, PLA->F);
    }

    /* A more traditional type of minimization */
    else if (option("primes") || option("qm")) {
	free_cover(PLA->F);
	PLA->R = sf_contain(unravel(PLA->R, cube.num_vars-1));
        EXEC(PLA->R = lex_sort(PLA->R), "SORT       ", PLA->R);
	EXEC(PLA->F = cb_sharp(cube.fullset, PLA->R), "PRIMES     ", PLA->F);
	if (option("qm"))
	    EXEC(PLA->F = irredundant(PLA->F, PLA->D), "COVER      ", PLA->F);
    }

    else if (option("dcset")) {	/* find where we intersect DC set */
	pcover T;
   	EXEC(T = cv_intersect(PLA->F, PLA->D), "INTERSECT  ", T);
	PLA->R = complement(cube1list(T));
	EXEC(PLA->F=espresso(T,new_cover(0),PLA->R), "ESPRESSO   ", PLA->F);
    }

    else if (option("map"))
	map(PLA->F);
    else if (option("opo"))
	phase_assignment(PLA);
    else if (option("badopo")) {
/*	PLA->phase = badopo(PLA->F);
	set_phase(PLA);*/
	minall(PLA);
    } else if (option("allphase"))
	allphase(PLA, first_output, last_output);

 
    /* Simple things ... */
    else if (option("echo")) 			/* merely echo the pla */
	;
    else if (option("contain"))			/* single cube containment */
	PLA->F = sf_contain(PLA->F);
    else if (option("intersect"))		/* cover intersection */
	PLA->F = cv_intersect(PLA->F, PLA1->F);
    else if (option("union"))			/* cover union */
	PLA->F = sf_union(PLA->F, PLA1->F);
    else if (option("sharp"))			/* cover sharp */
	PLA->F = cv_sharp(PLA->F, PLA1->F);
    else if (option("lexsort"))			/* lexical sort order */
	PLA->F = lex_sort(PLA->F);		
    else if (option("miniexpord"))		/* mini expand order */
	PLA->F = mini_order(PLA->F, ascend);
    else if (option("miniredord"))		/* mini reduce order */
	PLA->F = mini_order(PLA->F, descend);
    else if (option("stats"))			/* print info on size */
	summary = print_solution = FALSE;
    else if (option("unravel"))			/* unravel output */
	PLA->F = unravel(PLA->F, cube.num_vars - 1);
    else if (option("minterms"))		/* explode into minterms ! */
	PLA->F = sf_contain(unravel(PLA->F, 0));
    else if (option("d1merge")) 		/* full distance-1 merge */
	for(i = 0; i < cube.num_vars; i++)
	    PLA->F = d1merge(PLA->F, i);
    else if (option("compact"))			/* distance-1 merge outputs */
	PLA->F = d1merge(PLA->F, cube.num_vars - 1);
    else if (option("check")) {			/* check consistency */
	verify_error = check_consistency(PLA);
	print_solution = FALSE;

    /* Anything else is an error */
    } else {
	fprintf(stderr, "Unknown program option %s\n", what_to_do);
	exit(-1);
    }

    if (trace)
	runtime();
    if (summary || trace) {
	mem_stats();
	if (! option("mincov") || option("mincov1"))
	    print_trace(PLA->F, what_to_do, ptime() - start);
    }
    if (print_solution)
	fprint_pla(stdout, PLA, out_type);

    if (verify_error)
	fatal("Minimized cover verification failed");
    exit(0);
}
