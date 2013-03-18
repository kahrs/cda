#include "espresso.h"

/* fatal -- report fatal error message and take a dive */
void fatal(s)
char *s;
{
    fprintf(stderr, "%s\n", s);
    exit(-1);
}

/* cost -- compute the cost of a cover */
void cover_cost(F, cost)
pcover F;
pcost cost;
{
    register pcube m, p, last, temp = cube.temp[0], temp1 = cube.temp[1];

    cost->cubes = F->count;

    /* #in is number of transistors in binary variables */
    m = cube.binary_mask;
    cost->in = 0;
    foreach_set(F, last, p)
        cost->in += set_ord(set_diff(temp, m, p));

    /* #out is number of transistors everywhere else */
    m = set_diff(temp1, cube.fullset, m);
    cost->out = 0;
    foreach_set(F, last, p)
        cost->out += set_ord(set_and(temp, m, p));

    cost->primes = 0;
    foreach_set(F, last, p)
	cost->primes += TESTP(p, PRIME) != 0;
}


/* print_cost -- return a string which reports the "cost" of a cover */
char *print_cost(F)
pcover F;
{
    static char s[200];
    cost_t cost;
    cover_cost(F, &cost);
    (void) sprintf(s, "c=%d(%d) in=%d out=%d tot=%d",
	cost.cubes, cost.cubes - cost.primes, cost.in, cost.out,
	cost.in + cost.out);
    return s;
}

/* copy_cost -- copy a cost function from s to d */
void copy_cost(s, d)
pcost s, d;
{
    d->cubes = s->cubes;
    d->in = s->in;
    d->out = s->out;
    d->primes = s->primes;
}

/* cost_better -- compare two cost functions for improvement */
bool cost_better(old, new)
pcost old, new;
{
    if (new->cubes < old->cubes) return TRUE;
    else if (new->cubes > old->cubes) return FALSE;
    else if (new->in+new->out != old->in+old->out) return TRUE;
    else return FALSE;
}

/* size_stamp -- print single line giving the size of a cover */
void size_stamp(T, name)
IN pcover T;
IN char *name;
{
    printf("# %s\tCost is %s\n", name, print_cost(T));
    (void) fflush(stdout);
}

/* print_trace -- print a line reporting size and time after a function */
void print_trace(T, name, time)
IN pcover T;
IN char *name;
IN double time;
{
    printf("# %s\tTime was %s, cost is %s\n", 
	name, print_time(time), print_cost(T));
    (void) fflush(stdout);
}

/* totals -- add time spent in the function into the totals */
void totals(time, index, T, cost)
IN double time;
IN int index;
IN pcover T;
OUT pcost cost;
{
    time = ptime() - time;
    total_time[index] += time;
    total_calls[index]++;
    cover_cost(T, cost);
    if (trace) 
	printf("# %s\tTime was %s, cost is c=%d(%d) in=%d out=%d tot=%d\n",
	    total_name[index], print_time(time), cost->cubes, cost->cubes
	    -cost->primes, cost->in, cost->out, cost->in+cost->out);
    (void) fflush(stdout);
}


/* chk_arglist -- check for extra arguments on command line */
void chk_arglist(argc, argv)
int *argc;
char *argv[];
{
    int i;
    if (*argc > 1) {
	fprintf(stderr, "Extraneous arguments ignored:");
	for(i = 1; i < *argc; i++)
	    fprintf(stderr, " \"%s\"\n", argv[i]);
    }
}


/* delete_arg -- delete an argument from the argument list */
void delete_arg(argc, argv, num)
int *argc, num;
register char *argv[];
{
    register int i;
    (*argc)--;
    for(i = num; i < *argc; i++)
	argv[i] = argv[i+1];
}


/* check_arg -- scan argv for an argument, and return TRUE if found */
bool check_arg(argc, argv, s)
int *argc;
register char *argv[], *s;
{
    register int i;
    for(i = 1; i < *argc; i++)
        if (equal(argv[i], s)) {
	    delete_arg(argc, argv, i);
            return TRUE;
        }
    return FALSE;
}


struct {
    char *key;
    int val;
} debug_flags[] = {
    "-d", EXPAND + ESSEN + IRRED + REDUCE + SPARSE + GASP + SHARP + MINCOV,
    "-compl",	COMPL,	"-essen",	ESSEN,
    "-expand",	EXPAND,	"-expand1",	EXPAND1|EXPAND,
    "-gasp",	GASP,   "-irred",	IRRED,
    "-reduce",	REDUCE,	"-reduce1",	REDUCE1|REDUCE,
    "-mincov",	MINCOV,	"-mincov1",	MINCOV1|MINCOV,
    "-sparse",	SPARSE, "-sharp",	SHARP,
    "-taut",	TAUT,   "-ftaut",	FTAUT,
    0,
};

void chk_debug(argc, argv)
int *argc;
char *argv[];
{
    register int i, j;
    debug = 0;
    verbose_debug = FALSE;
    for(i = 1; i < *argc; i++)
	for(j = 0; debug_flags[j].key != 0; j++)
	    if (equal(argv[i], debug_flags[j].key)) {
		delete_arg(argc, argv, i--);
		debug |= debug_flags[j].val;
		break;
	    } else if (equal(argv[i], "-v")) {
		verbose_debug = TRUE;
		delete_arg(argc, argv, i--);
	    }
}
