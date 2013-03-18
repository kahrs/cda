/*
    module: memory.c 
    purpose: Efficient memory management scheme for large blocks of memory
*/
#include "espresso.h"

#ifdef UNIX
#define malloc(n) sbrk((int)n)
#endif

#define BINS 64
union memory_block {
    union memory_block *nextblock;	/* Pointer to next block on chain */
    int binnum;				/* remember bin number */
} *binptr[BINS];
static int num_alloc[BINS], num_active[BINS], alloc_size; 


/* mem_stats -- report memory usage statistics */
void mem_stats()
{
    register int i, size, active_bytes = 0, alloc_bytes = 0;
    for(i = BINS-1; i >= 0; i--) {
	size = (i % 2 == 0 ? 3 : 4) << (i/2 - 2);
  	alloc_bytes += num_alloc[i] * size;
	active_bytes += num_active[i] * size;
    }
    active_bytes = (int) (active_bytes / 1024.0 + 0.5);
    alloc_bytes = (int) (alloc_bytes / 1024.0 + 0.5);
    printf("# %dK bytes active out of %dK bytes allocated\n",
	active_bytes, alloc_bytes);
}

/*
    mem_alloc -- allocate an array of "num" objects each of size "size".  Also
    return the actual number of array elements allocated
*/
char *mem_alloc(num, size, actual)
int num, size, *actual;
{
    char *p = alloc(num*size);
    *actual = (int) (alloc_size - sizeof(union memory_block)) / size;
    return p;
}
/* alloc -- efficiently allocate a (large) array of elements */
char *alloc(bytes)
int bytes;
{
    register int log, index, n, request=bytes+sizeof(union memory_block *);
    register union memory_block *p;

    /* compute log such that 2**log >= request */
    for(n = request-1, log = 1; (n >>= 1) > 0; log++)
	;
    log -= 2;
    /* determine the bin number and actual alloc size (based on the log) */
    if (request <= (3 << log))
	index = log*2, alloc_size = 3 << log;
    else
	index = log*2 + 1, alloc_size = 4 << log;

    /* See if we have a free block in the garbage list for this bin */
    if ((p = binptr[index]) == NULL) {
	/* Must allocate a new block -- no free space in garbage list */
        p = (union memory_block *) malloc((unsigned) alloc_size);
        if (p == NULL) {
	    /* Check if a larger and free block exists */
	    for(n = index+1, index = 0; n < BINS; n++)
	  	if (binptr[n] != NULL) {
		    index=n; p=binptr[index]; binptr[index]=p->nextblock;
		    break;
		}
	    if (index == 0) {
		fprintf(stderr, "espresso: failed allocating %d (%d) bytes\n",
		    request, alloc_size);
		exit(-1);
	    }
        }
        num_alloc[index]++;

    } else
	/* We can reuse the first entry on the garbage list for this size */
        binptr[index] = p->nextblock;

    num_active[index]++;
    p->binnum = index | 0x5500;
    return (char *) (p+1);
}

/* mem_free -- free the memory allocated */
void mem_free(block)
char *block;
{
    register union memory_block *p = (union memory_block *) block - 1;
    register int index = p->binnum;

    if (((index & 0xFF00) != 0x5500) || ((index &= 0x00FF) >= BINS)) {
	fprintf(stderr, "%s: corrupt pointer returned, addr=%x, index=%x\n",
	    "memory.c/mem_free", p, index);
   	exit(-1);
    }

    /* Link p into the garbage list for this block size */
    p->nextblock = binptr[index];
    binptr[index] = p;
    num_active[index]--;
}
