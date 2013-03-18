/*
    module: cvrout.c
    purpose: cube and cover output routines
*/

#include "espresso.h"

void fprint_pla(fp, PLA, output_type)
INOUT FILE *fp;
IN pPLA PLA;
IN int output_type;
{
    register pcube last, p;
    char s[1024];
    int num, var, firstpart, lastpart, outvar, i, col;

    if (output_type != F_type) {
	fprintf(fp, ".type ");
	if (output_type & F_type) putc('f', fp);
	if (output_type & D_type) putc('d', fp);
	if (output_type & R_type) putc('r', fp);
	putc('\n', fp);
    }

    if (cube.num_binary_vars == cube.num_vars - 1) {
	fprintf(fp, ".i %d\n", cube.num_binary_vars);
	fprintf(fp, ".o %d\n", cube.part_size[cube.num_vars-1]);
    } else {
	fprintf(fp, ".mv %d %d", cube.num_vars, cube.num_binary_vars);
	for(var = cube.num_binary_vars; var < cube.num_vars; var++)
	    fprintf(fp, " %d", cube.part_size[var]);
	fprintf(fp, "\n");
	for(var = 0; var < cube.num_vars; var++)
	    if (cube.label[var][0] != NULL) {
		col = 14;
		fprintf(fp, ".label var=%d", var);
		for(i = 0; cube.label[var][i] != NULL; i++) {
		    if ((col + strlen(cube.label[var][i]) + 1) > 78)
			putc('\n', fp), col = 0;
		    else
			putc(' ', fp);
		    fprintf(fp, "%s", cube.label[var][i]);
		    col += strlen(cube.label[var][i]) + 1;
		}
		putc('\n', fp);
	    }
    }

    if (PLA->phase != NULL) {
	outvar = cube.num_vars - 1;
	firstpart = cube.first_part[outvar]; 
	lastpart = cube.last_part[outvar];
	fprintf(fp, "#.phase ");
	for(i = firstpart; i <= lastpart; i++)
	    putc(is_in_set(PLA->phase,i) ? '1' : '0', fp);
	fprintf(fp, "\n");
    }

    num = 0;
    if (output_type & F_type) num += (PLA->F)->count;
    if (output_type & D_type) num += (PLA->D)->count;
    if (output_type & R_type) num += (PLA->R)->count;
    fprintf(fp, ".p %d\n", num);

    /* quick patch 01/17/85 to support TPLA ! */
    if (output_type == F_type) {
 	foreach_set(PLA->F, last, p)
	    fprintf(fp, "%s\n", fmt_cube(p, "01", s));
        fprintf(fp, ".e\n");
	return;
    }

    if (output_type & F_type)
 	foreach_set(PLA->F, last, p)
	    fprintf(fp, "%s\n", fmt_cube(p, "~1", s));
    if (output_type & D_type)
	foreach_set(PLA->D, last, p)
	    fprintf(fp, "%s\n", fmt_cube(p, "~2", s));
    if (output_type & R_type)
	foreach_set(PLA->R, last, p)
	    fprintf(fp, "%s\n", fmt_cube(p, "~0", s));
    fprintf(fp, ".end\n");
}


char *fmt_cube(c, out_map, s)
register pcube c;
register char *out_map, *s;
{
    register int i, var, val, len = 0;
    for(var = 0; var < cube.num_binary_vars; var++) {
	val = (is_in_set(c,var*2+1) != 0)*2 + (is_in_set(c,var*2) != 0);
	s[len++] = "?01-" [val];
    }
    for(var = cube.num_binary_vars; var < cube.num_vars - 1; var++) {
	s[len++] = ' ';
	for(i = cube.first_part[var]; i <= cube.last_part[var]; i++)
	    s[len++] = "01" [is_in_set(c, i) != 0];
    }
    if (cube.num_binary_vars != cube.num_vars) {
	var = cube.num_vars - 1;
	s[len++] = ' ';
	for(i = cube.first_part[var]; i <= cube.last_part[var]; i++)
	    s[len++] = out_map [is_in_set(c, i) != 0];
    }
    s[len] = '\0';
    return s;
}

char *pc1(c) pcube c;
{static char s1[256];return fmt_cube(c, "01", s1);}
char *pc2(c) pcube c;
{static char s2[256];return fmt_cube(c, "01", s2);}
char *pc3(c) pcube c;
{static char s3[256];return fmt_cube(c, "01", s3);}


void debug_print(T, name, level)
pcube *T;
char *name;
int level;
{
    register pcube *T1, p, temp;
    register int cnt;

    cnt = CUBELISTSIZE(T);
    temp = new_cube();
    if (verbose_debug && level == 0)
	printf("\n");
    printf("%s[%d]: ord(T)=%d\n", name, level, cnt);
    if (verbose_debug) {
	printf("cofactor=%s\n", pc1(T[0]));
	for(T1 = T+2, cnt = 1; (p = *T1++) != NULL; cnt++)
	    printf("%4d. %s\n", cnt, pc1(set_or(temp, p, T[0])));
    }
    free_cube(temp);
}


void debug1_print(T, name, num)
pcover T;
char *name;
int num;
{
    register int cnt = 1;
    register pcube p, last;

    if (verbose_debug && num == 0)
	printf("\n");
    printf("%s[%d]: ord(T)=%d\n", name, num, T->count);
    if (verbose_debug)
	foreach_set(T, last, p)
	    printf("%4d. %s\n", cnt++, pc1(p));
}


void cprint(T)
pcover T;
{
    register pcube p, last;

    foreach_set(T, last, p)
	printf("%s\n", pc1(p));
}


void sf_debug_print(T, name, level)
pset_family T;
char *name;
int level;
{
    printf("%s[%d]: ord(T)=%d\n", name, level, T->count);
    if (verbose_debug)
	sf_print(T);
}
