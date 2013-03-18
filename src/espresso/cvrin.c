/*
    module: cvrin.c
    purpose: cube and cover input routines
*/

#include "espresso.h"

#define strsav(s)	strcpy(alloc(strlen(s)+1), s)
#define abs(i)		((i) > 0 ? (i) : -(i))

static bool echo_comments = TRUE, echo_unknown_commands = TRUE;
static bool line_length_error;
static int lineno;

void skip_line(fpin, fpout, echo)
register FILE *fpin, *fpout;
register bool echo;
{
    register int ch;
    while ((ch=getc(fpin)) != EOF && ch != '\n')
	if (echo)
	    putc(ch, fpout);
    if (echo)
	putc('\n', fpout);
    lineno++;
}

char *get_word(fp, word)
register FILE *fp;
register char *word;
{
    register int ch, i = 0;
    while ((ch = getc(fp)) != EOF && ! isspace(ch))
	word[i++] = ch;
    word[i++] = '\0';
    return word;
}
/*
    cube_setup -- assume that the fields "num_vars", "num_binary_vars", and
    part_size[num_binary_vars .. num_vars-1] are setup, and initialize the
    rest of cube and cdata.

    If a part_size is < 0, then the field size is abs(part_size) and the
    field read from the input is symbolic.
*/
cube_setup()
{
    register int i, j, var;
    register pcube p;

    cube.size = 0;
    cube.first_part = (int *) alloc(cube.num_vars * sizeof(int));
    cube.last_part = (int *) alloc(cube.num_vars * sizeof(int));
    cube.first_word = (int *) alloc(cube.num_vars * sizeof(int));
    cube.last_word = (int *) alloc(cube.num_vars * sizeof(int));
    for(var = 0; var < cube.num_vars; var++) {
	if (var < cube.num_binary_vars)
	    cube.part_size[var] = 2;
	cube.first_part[var] = cube.size;
	cube.first_word[var] = WHICH_WORD(cube.size);
	cube.size += abs(cube.part_size[var]);
	cube.last_part[var] = cube.size - 1;
	cube.last_word[var] = WHICH_WORD(cube.size - 1);
    }

    cube.var_mask = (pset *) alloc(cube.num_vars * sizeof(pset));
    cube.sparse = (int *) alloc(cube.num_vars * sizeof(int));
    cube.binary_mask = new_cube();
    cube.mv_mask = new_cube();
    for(var = 0; var < cube.num_vars; var++) {
	p = cube.var_mask[var] = new_cube();
	for(i = cube.first_part[var]; i <= cube.last_part[var]; i++)
	    set_insert(p, i);
	if (var < cube.num_binary_vars) {
	    INLINEset_or(cube.binary_mask, cube.binary_mask, p);
	    cube.sparse[var] = 0;
	} else {
	    INLINEset_or(cube.mv_mask, cube.mv_mask, p);
	    cube.sparse[var] = 1;
	}
    }
    if (cube.num_binary_vars == 0)
	cube.inword = -1;
    else {
	cube.inword = cube.last_word[cube.num_binary_vars - 1];
	cube.inmask = cube.binary_mask[cube.inword] & DISJOINT;
    }

    cube.temp = (pset *) alloc(CUBE_TEMP * sizeof(pset));
    for(i = 0; i < CUBE_TEMP; i++)
	cube.temp[i] = new_cube();
    cube.fullset = set_fill(new_cube(), cube.size);
    cube.emptyset = new_cube();

    if (cube.label == NULL) {
	cube.label = (char ***) alloc(cube.num_vars * sizeof(char **));
	for(i = 0; i < cube.num_vars; i++) {
	    cube.label[i] = (char **) 
		alloc( (abs(cube.part_size[i])+1) * sizeof(char *));
	    for(j = 0; j < abs(cube.part_size[i]); j++)
		cube.label[i][j] = NULL;
	}
    }

    cdata.part_zeros = (int *) alloc(cube.size * sizeof(int));
    cdata.var_zeros = (int *) alloc(cube.num_vars * sizeof(int));
    cdata.parts_active = (int *) alloc(cube.num_vars * sizeof(int));
    cdata.is_unate = (int *) alloc(cube.num_vars * sizeof(int));
}
/*
    set_phase -- given a "cube" which describes which phases of the output
    are to be implemented, compute the appropriate on-set and off-set
*/
pPLA set_phase(PLA)
INOUT pPLA PLA;
{
    pcover F1, R1;
    register pcube last, p, outmask;
    register pcube temp=cube.temp[0], phase=PLA->phase, phase1=cube.temp[1];

    outmask = cube.var_mask[cube.num_vars - 1];
    set_diff(phase1, outmask, phase);
    set_or(phase1, set_diff(temp, cube.fullset, outmask), phase1);
    F1 = new_cover((PLA->F)->count + (PLA->R)->count);
    R1 = new_cover((PLA->F)->count + (PLA->R)->count);

    foreach_set(PLA->F, last, p) {
	if (! setp_disjoint(set_and(temp, p, phase), outmask))
	    set_copy(GETSET(F1, F1->count++), temp);
	if (! setp_disjoint(set_and(temp, p, phase1), outmask))
	    set_copy(GETSET(R1, R1->count++), temp);
    }
    foreach_set(PLA->R, last, p) {
	if (! setp_disjoint(set_and(temp, p, phase), outmask))
	    set_copy(GETSET(R1, R1->count++), temp);
	if (! setp_disjoint(set_and(temp, p, phase1), outmask))
	    set_copy(GETSET(F1, F1->count++), temp);
    }
    free_cover(PLA->F);
    free_cover(PLA->R);
    PLA->F = F1;
    PLA->R = R1;
    return PLA;
}
void read_cube(fp, PLA)
IN register FILE *fp;
INOUT pPLA PLA;
{
    register int var, i, len;
    pcube cf = cube.temp[0], cr = cube.temp[1], cd = cube.temp[2];
    bool savef = FALSE, saved = FALSE, saver = FALSE;
    char line[1024];

    set_clear(cf, cube.size);
    len = 0;

    /* Loop and read binary variables */
    for(var = 0; var < cube.num_binary_vars; var++)
	switch(line[len++] = getc(fp)) {
	    case EOF:
		goto bad_char;
	    case '\n':
		if (! line_length_error)
		    fprintf(stderr, "espresso: product term(s) %s\n",
			"span more than one line (warning only)");
		line_length_error = TRUE;
		lineno++;
	    case ' ': case '|': case '\t':
		var--;
		break;
	    case '2': case '-':
		set_insert(cf, var*2+1);
	    case '0': 
		set_insert(cf, var*2);
		break;
	    case '1':
		set_insert(cf, var*2+1);
		break;
	    default:
		goto bad_char;
	}


    /* Loop for the all but one of the multiple-valued variables */	
    for(var = cube.num_binary_vars; var < cube.num_vars-1; var++)

	/* Read a symbolic multiple-valued variable */
	if (cube.part_size[var] < 0) {
	    char token[250]; int varx = var;
	    fscanf(fp, "%s", token);
	    if (equal(token, "-"))
		set_or(cf, cf, cube.var_mask[var]);
	    else {
		if (kiss && var == cube.num_vars - 2)
		    varx = var - 1; /* use present state symbol table */
		/* Find the symbolic label in the label table */
	        for(i = 0; cube.label[varx][i] != NULL; i++)
		    if (equal(cube.label[varx][i], token)) {
			set_insert(cf, i + cube.first_part[var]);
			break;
		    }
		if (cube.label[varx][i] == NULL)
		    if (i == -cube.part_size[var]) {
			fprintf(stderr,"espresso: var %d, size too small\n",
			    var, -cube.part_size[var]);
			exit(-1);
		    } else {
			cube.label[varx][i] = strsav(token);
			set_insert(cf, i + cube.first_part[var]);
		    }
	    }
	
        } else for(i = cube.first_part[var]; i <= cube.last_part[var]; i++)
	    switch (line[len++] = getc(fp)) {
		case EOF:
		    goto bad_char;
		case '\n':
		    if (! line_length_error)
			fprintf(stderr, "espresso: product term(s) %s\n",
			    "span more than one line (warning only)");
		    line_length_error = TRUE;
		    lineno++;
		case ' ': case '|': case '\t':
		    i--;
		    break;
	 	case '1': 
		    set_insert(cf, i);
		case '0': 
		    break;
	   	default:
		    goto bad_char;
	    }

    /* Loop for last multiple-valued variable */
    if (kiss) {
	saver = savef = TRUE;
	set_xor(cr, cf, cube.var_mask[cube.num_vars - 2]);
    } else
	set_copy(cr, cf);
    set_copy(cd, cf);
    for(i = cube.first_part[var]; i <= cube.last_part[var]; i++)
	switch (line[len++] = getc(fp)) {
	    case EOF:
		goto bad_char;
	    case '\n':
		if (! line_length_error)
		    fprintf(stderr, "espresso: product term(s) %s\n",
			"span more than one line (warning only)");
		line_length_error = TRUE;
		lineno++;
	    case ' ': case '|': case '\t':
		i--;
		break;
	    case '4': case '1':
		if (PLA->pla_type & F_type)
		    set_insert(cf, i), savef = TRUE;
		break;
	    case '3': case '0': 
		if (PLA->pla_type & R_type)
		    set_insert(cr, i), saver = TRUE;
		break;
	    case '2': case '-':
		if (PLA->pla_type & D_type)
		    set_insert(cd, i), saved = TRUE;
	    case '~':
		break;
	    default:
		goto bad_char;
	}
    if (savef)
	PLA->F = sf_addset(PLA->F, cf);
    if (saved)
	PLA->D = sf_addset(PLA->D, cd);
    if (saver)
	PLA->R = sf_addset(PLA->R, cr);
    return;

bad_char:
    fprintf(stderr, "(warning): input line #%d ignored\n", lineno);
    for(i = 0; i < len; i++)
	putc(line[i], stdout);
    skip_line(fp, stdout, TRUE);
    return;
}
void parse_pla(fp, PLA)
IN FILE *fp;
INOUT pPLA PLA;
{
    register int i, var, ch;
    int np;
    char word[256];

    lineno = 1;
    line_length_error = FALSE;

loop:
    switch(ch = getc(fp)) {
	case EOF:
	    return;
	case '\n':
	    lineno++;
	case ' ': case '\t': case '\f': case '\r':
	    break;
	case '#':
	    ungetc(ch, fp);
	    skip_line(fp, stdout, echo_comments);
	    break;
	case '.':
	    /* .i gives the cube input size (binary-functions only) */
	    if (equal(get_word(fp, word), "i")) {
		if (cube.fullset != NULL)
		    fprintf(stderr, "espresso: extra .i ignored\n");
		else {
		    fscanf(fp, "%d", &cube.num_binary_vars);
		    cube.num_vars = cube.num_binary_vars + 1;
		    cube.part_size = (int *) alloc(cube.num_vars*sizeof(int));
		}

	    /* .o gives the cube output size (binary-functions only) */
	    } else if (equal(word, "o")) {
		if (cube.fullset != NULL)
		    fprintf(stderr, "espresso: extra .o ignored\n");
		else {
		    if (cube.part_size == NULL)
			fatal("espresso: .o cannot appear before .i");
		    fscanf(fp, "%d", &(cube.part_size[cube.num_vars - 1]));
		    cube_setup();
		}

	    /* .mv gives the cube size for a multiple-valued function */
	    } else if (equal(word, "mv")) {
		if (cube.fullset != NULL)
		    fprintf(stderr, "espresso: extra .mv ignored\n");
		else {
		    if (cube.part_size != NULL)
			fatal("espresso: cannot mix .i and .mv");
		    fscanf(fp,"%d %d",&cube.num_vars,&cube.num_binary_vars);
		    cube.part_size = (int *) alloc(cube.num_vars*sizeof(int));
		    for(var=cube.num_binary_vars; var < cube.num_vars; var++)
			fscanf(fp, "%d", &(cube.part_size[var]));
		    cube_setup();
		}

	    /* .p gives the number of product terms (redundant) */
	    } else if (equal(word, "p"))
		fscanf(fp, "%d", &np);
	    /* .name at one time gave a name to the pla */
	    else if (equal(word, "name"))
		get_word(fp, word);
	    /* .e and .end specify the end of the file */
	    else if (equal(word, "e") || equal(word,"end"))
		return;
	    /* .kiss turns on the kiss-hack option */
	    else if (equal(word, "kiss"))
		kiss = TRUE;

	    /* .type specifies a logical type for the PLA */
	    else if (equal(word, "type")) {
		get_word(fp, word);
		for(i = 0; pla_types[i].key != 0; i++)
		    if (equal(pla_types[i].key + 1, word)) {
			PLA->pla_type = pla_types[i].value;
			break;
		    }
		if (pla_types[i].key == 0)
		    fatal("espresso: unknown type in .type command");

	    /* .phase allows a choice of output phases */
	    } else if (equal(word, "phase")) {
		int last;
		if (cube.fullset == NULL)
		    fatal("espresso: cube size not declared before .phase");
		PLA->phase = set_save(cube.fullset);
		last = cube.last_part[cube.num_vars - 1];
		for(i = cube.first_part[cube.num_vars - 1]; i <= last; i++)
		    if ((ch = getc(fp)) == '0')
			set_remove(PLA->phase, i);
		    else if (ch != '1')
			fatal("espresso: bad character in phase description");

	    /* .pair allows for bit-pairing input variables */
	    } else if (equal(word, "pair")) {
		if (PLA->pair != NULL)
		    fprintf(stderr, "espresso: extra .pair ignored\n");
		else {
		    ppair pair;
		    PLA->pair = pair = (ppair) alloc(sizeof(pair_t));
		    fscanf(fp, "%d", &(pair->cnt));
		    pair->var1 = (int *) alloc(pair->cnt * sizeof(int));
		    pair->var2 = (int *) alloc(pair->cnt * sizeof(int));
		    for(i = 0; i < pair->cnt; i++)
			if (fscanf(fp, " ( %d %d )", &(pair->var1[i]),
			  &(pair->var2[i])) != 2)
			    fatal("espresso: syntax error in .pair");
		}
		
	    } else {
		if (echo_unknown_commands)
		    printf("%c%s ", ch, word);
		skip_line(fp, stdout, echo_unknown_commands);
	    }
	    break;
	default:
	    ungetc(ch, fp);
	    if (cube.fullset == NULL) {
/*		fatal("espresso: unknown cube size, need .i/.o or .mv");*/
		putchar('#');
		skip_line(fp, stdout, TRUE);
		break;
	    }
	    if (PLA->F == NULL) {
		PLA->F = new_cover(10);
		PLA->D = new_cover(10);
		PLA->R = new_cover(10);
	    }
	    read_cube(fp, PLA);
    }
    goto loop;
}
/*
    read_pla -- read a pla from a file
	
    Unpack an optional pla type from the command line.

    The next argument is assumed to be a filename or "-" to imply stdin.
    (stdin is also assumed if no more arguments remain)

    We then read the actual cubes from the file and massage them into
    the logical representations of an ON cover, OFF cover, and DC cover.
*/

pPLA read_pla(argc, argv, default_pla_type)
INOUT int *argc;
INOUT char *argv[];
IN int default_pla_type;
{
    int i, var;
    pPLA PLA;
    FILE *fp;
    double time;
    cost_t cost;

    /* Allocate and initialize the PLA structure */
    PLA = (pPLA) alloc(sizeof(PLA_t));
    PLA->F = PLA->D = PLA->R = NULL;
    PLA->phase = NULL;
    PLA->pair = NULL;
    PLA->pla_type = default_pla_type;

    /* Check if next command line argument specifies logical pla type */
    if (*argc >= 2)
	for(i = 0; pla_types[i].key != 0; i++)
	    if (equal(pla_types[i].key, argv[1])) {
		PLA->pla_type = pla_types[i].value;
		delete_arg(argc, argv, 1);
		break;
	    }

    /* Assume next argument is a filename; or read stdin if no more args */
    if (*argc >= 2 && ! equal(argv[1], "-")) {
	strcpy(PLA->filename, argv[1]);
	if ((fp = fopen(argv[1], "r")) == NULL) {
	    fprintf(stderr, "espresso: can't open %s\n", argv[1]);
	    exit(-1);
	}
	delete_arg(argc, argv, 1);
    } else {
	strcpy(PLA->filename, "(stdin)");
        fp = stdin;
    }

    /* Read the pla */
    time = ptime();
    parse_pla(fp, PLA);
    for(i = 0; i < cube.num_vars; i++)
	cube.part_size[i] = abs(cube.part_size[i]);
    if (kiss) {
	cube.part_size[cube.num_vars-2] += cube.part_size[cube.num_vars-1];
	cube.label[cube.num_vars-2] = cube.label[cube.num_vars-1];
	cube.num_vars--;
	cube_setup();
    }
    if (PLA->F == NULL)
	fatal("espresso: no PLA found on input file");
    if (trace)
	totals(time, READ_TIME, PLA->F, &cost);

    /* Decide how to break PLA into ON-set, OFF-set and DC-set */
    time = ptime();
    if (PLA->pla_type == F_type || PLA->pla_type == FD_type) {
	free_cover(PLA->R);
	PLA->R = complement(cube2list(PLA->F, PLA->D));
    } else if (PLA->pla_type == FR_type) {
	pcover X;
	free_cover(PLA->D);
	/* hack, why not? */
	X = d1merge(sf_join(PLA->F, PLA->R), cube.num_vars - 1);
	PLA->D = complement(cube1list(X));
	free_cover(X);
    } else if (PLA->pla_type == R_type || PLA->pla_type == DR_type) {
	free_cover(PLA->F);
	PLA->F = complement(cube2list(PLA->D, PLA->R));
    }
    if (trace)
	totals(time, COMPL_TIME, PLA->R, &cost);

    if (pos) {
	pcover onset = PLA->F;
	PLA->F = PLA->R;
	PLA->R = onset;
    } else if (PLA->phase != NULL)
	set_phase(PLA);
    if (PLA->pair != NULL)
	set_pair(PLA);

    if (trace || summary) {
	printf("# PLA is %s", PLA->filename);
	if (cube.num_binary_vars == cube.num_vars - 1)
	    printf(" with %d inputs and %d outputs\n",
		cube.num_binary_vars, cube.part_size[cube.num_vars - 1]);
	else {
	    printf(" with %d variables (%d binary, mv sizes",
		cube.num_vars, cube.num_binary_vars);
	    for(var = cube.num_binary_vars; var < cube.num_vars; var++)
		printf(" %d", cube.part_size[var]);
	    printf(")\n");
	}
	printf("# ON-set cost is  %s\n", print_cost(PLA->F));
	printf("# OFF-set cost is %s\n", print_cost(PLA->R));
	printf("# DC-set cost is  %s\n", print_cost(PLA->D));
	if (PLA->phase != NULL)
	    printf("# phase is %s\n", pc1(PLA->phase));
	if (PLA->pair != NULL) {
	    printf("# two-bit encoders with pairing");
	    for(i = 0; i < PLA->pair->cnt; i++)
		printf(" (%d %d)", PLA->pair->var1[i], PLA->pair->var2[i]);
	    printf("\n");
	}
	(void) fflush(stdout);
    }

    return PLA;
}
