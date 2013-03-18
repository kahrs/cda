/*
    espresso.h -- Common header file for Espresso-mv
*/

#ifndef EXTERN
#define EXTERN extern
#endif
#ifndef UNIX
#define void int
#endif

#include <stdio.h>
#include <ctype.h>
#include "set.h"


/* Define a boolean type */
#define bool int
#define FALSE 0
#define TRUE  1
#define MAYBE 2
#define print_bool(x) ((x) == 0 ? "FALSE" : ((x) == 1 ? "TRUE" : "MAYBE"))

/* Map many cube/cover types/routines into equivalent set types/routines */
#define pcube			pset
#define new_cube()		set_new(cube.size)
#define free_cube(r)		mem_free((char *) r)
#define pcover			pset_family
#define new_cover(i)		sf_new(i, cube.size)
#define free_cover(r)		sf_free(r)
#define free_cubelist(T)	mem_free((char *) T[0]), mem_free((char *) T)

/* The cost structure stores the cost of a cover */
typedef struct cost_struct {
    int cubes, in, out, primes;
} cost_t, *pcost; 

/* The pla_type field describes the input format of the PLA */
#define F_type	1
#define D_type	2
#define R_type	4
#define FD_type (F_type | D_type)
#define FR_type (F_type | R_type)
#define DR_type (D_type | R_type)
#define FDR_type (F_type | D_type | R_type)

EXTERN struct {
    char *key;
    int value;
} pla_types[]
#ifdef INIT
 = {"-f", F_type, "-r", R_type, "-d", D_type, "-fd", FD_type,
   "-fr", FR_type, "-dr", DR_type, "-fdr", FDR_type, 0, 0}
#endif
;

/* pair_t describes bit-paired variables */
typedef struct {
    int cnt, *var1, *var2;
} pair_t, *ppair;

/* PLA_t stores the logical representation of a PLA */
typedef struct {
    pcover F, D, R;
    char filename[200];		/* Filename */
    int pla_type;		/* logical PLA format */
    pcube phase;		/* phase to split into on-set and off-set */
    ppair pair;			/* how to pair variables */
} PLA_t, *pPLA;

#define min(a,b)	((a)>(b) ? (b) : (a))
#define max(a,b)	((a)<(b) ? (b) : (a))
#define equal(a,b)	(strcmp(a,b) == 0)

#define CUBELISTSIZE(T)		(((pcube *) T[1] - T) - 3)

/* For documentation purposes */
#define IN
#define OUT
#define INOUT

/* Definitions for the debug variable */
#define COMPL       	0x0001
#define ESSEN           0x0002
#define EXPAND      	0x0004
#define EXPAND1     	0x0008
#define GASP     	0x0010
#define IRRED       	0x0020
#define REDUCE      	0x0040
#define REDUCE1     	0x0080
#define SPARSE      	0x0100
#define TAUT        	0x0200
#define FTAUT		0x0400
#define MINCOV 		0x0800
#define MINCOV1		0x1000
#define SHARP		0x2000

#define VERSION	\
    "UC Berkeley, Espresso Version #2.1, Release date 2/28/85"

/* Define constants used for recording program statistics */
#define TIME_COUNT	12
#define READ_TIME	0
#define COMPL_TIME	1
#define ESSEN_TIME	2
#define EXPAND_TIME	3
#define IRRED_TIME	4
#define REDUCE_TIME	5
#define GEXPAND_TIME	6
#define GIRRED_TIME	7
#define GREDUCE_TIME	8
#define MV_REDUCE_TIME	9
#define RAISE_IN_TIME	10
#define VERIFY_TIME	11

#define EXEC(fct, name, S)	{double t=ptime();fct;\
if(trace)print_trace(S,name,ptime()-t);}

#define EXECUTE(fct,i,S)	{double t=ptime();fct;totals(t,i,S,&cost);}

#define EXECUTE_BREAK(fct,i,S)	{double t=ptime();fct;totals(t,i,S,&cost);\
if(! cost_better(&best_cost, &cost))break;}

/* We need some global variables */
EXTERN unsigned int debug;		/* debug parameter */
EXTERN bool verbose_debug;		/* -v:  whether to print a lot */
EXTERN char *total_name[TIME_COUNT];	/* basic function names */
EXTERN double total_time[TIME_COUNT];	/* time spent in basic fcts */
EXTERN int total_calls[TIME_COUNT];	/* # calls to each fct */

EXTERN bool mincov_exact;		/* -exact command line option */
EXTERN bool force_irredundant;		/* -nirr command line option */
EXTERN bool pos;			/* -pos command line option */
EXTERN bool kiss;			/* -kiss command line option */
EXTERN bool remove_essential;		/* -ness command line option */
EXTERN bool single_expand;		/* -fast command line option */
EXTERN bool summary;			/* -s command line option */
EXTERN bool trace;			/* -t command line option */

#define CUBE_TEMP	10
EXTERN struct {
    int size;			/* set size of a cube */
    int num_vars;		/* number of variables in a cube */
    int num_binary_vars;	/* number of binary variables */
    int *first_part;		/* first element of each variable */
    int *last_part;		/* first element of each variable */
    int *part_size;		/* number of elements in each variable */
    int *first_word;		/* first word for each variable */
    int *last_word;		/* last word for each variable */
    pset binary_mask;		/* Mask to extract binary variables */
    pset mv_mask;		/* mask to get mv parts */
    pset *var_mask;		/* mask to extract a variable */
    pset *temp;			/* an array of temporary sets */
    pset fullset;		/* a full cube */
    pset emptyset;		/* an empty cube */
    unsigned int inmask;	/* mask to get odd word of binary part */
    int inword;			/* which word number for above */
    int *sparse;		/* should this variable be sparse? */
    char ***label;		/* label each part of each var */
} cube;

EXTERN struct {
    int *part_zeros;		/* count of zeros for each element */
    int *var_zeros;		/* count of zeros for each variable */
    int *parts_active;		/* number of "active" parts for each var */
    bool *is_unate;		/* indicates given var is unate */
    int vars_active;		/* number of "active" variables */
    int vars_unate;		/* number of unate variables */
    int best;			/* best "binate" variable */
} cdata;

#if BPI == 32
#define DISJOINT 0x55555555
#else 
#define DISJOINT 0x5555
#endif

/* system function declarations */
extern char *malloc(), *strcpy(), *sprintf();
extern int printf(), fprintf(), scanf(), fscanf(), fflush(), ungetc();
extern int fclose(), exit(), strlen(), strcmp();
extern long atol();

#include "esptype.h"
