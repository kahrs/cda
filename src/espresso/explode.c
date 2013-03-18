#include "espresso.h"

static pcube Gcube;
static pset Gminterm;

pset minterms(T)
pcover T;
{
    int size, var;
    register pcube last;

    size = 1;
    for(var = 0; var < cube.num_vars; var++)
	size *= cube.part_size[var];
    Gminterm = set_new(size);

    foreach_set(T, last, Gcube)
	explode(cube.num_vars-1, 0);

    return Gminterm;
}


void explode(var, z)
int var, z;
{
    int i, last = cube.last_part[var];
    for(i=cube.first_part[var], z *= cube.part_size[var]; i<=last; i++, z++)
	if (is_in_set(Gcube, i))
	    if (var == 0)
		set_insert(Gminterm, z);
	    else
		explode(var-1, z);
}


static int mapindex[16] = {0,1,3,2,4,5,7,6,12,13,15,14,8,9,11,10};

void map(T)
pcover T;
{
    register int i, j, offset, p;
    pset m = minterms(T);

    for(p = 0; p < cube.part_size[cube.num_vars-1]; p++) {
	printf("\nOutput space # %d\n", p);
        for(j = 0; j < 16; j += 4) {
	    offset = p*64;
	    for(i = j; i < j+4; i++)
		putchar(is_in_set(m,mapindex[i]+offset) ? '1' : '.');
	    putchar(' ');
	    offset = p*64 + 16;
	    for(i = j; i < j+4; i++)
		putchar(is_in_set(m,mapindex[i]+offset) ? '1' : '.');
	    putchar('\n');
	}
	putchar('\n');
        for(j = 0; j < 16; j += 4) {
	    offset = p*64 + 32;
	    for(i = j; i < j+4; i++)
		putchar(is_in_set(m,mapindex[i]+offset) ? '1' : '.');
	    putchar(' ');
	    offset = p*64 + 48;
	    for(i = j; i < j+4; i++)
		putchar(is_in_set(m,mapindex[i]+offset) ? '1' : '.');
	    putchar('\n');
	}
    }
}
