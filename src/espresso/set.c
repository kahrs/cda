/*
    module: set.c
    purpose: routines for manipulating sets
*/

#include "espresso.h"
static pset_family set_family_garbage = NULL;

/* set_ord -- count number of elements in a set */
int set_ord(a)
register pset a;
{
    register int i, sum = 0; register unsigned int val;
    for(i = LOOP(a); i > 0; i--)
	if ((val = a[i]) != 0)
	    sum += count_ones(val);
    return sum;
}

/* set_dist -- distance between two sets (# elements in common) */
int set_dist(a, b)
register pset a, b;
{
    register int i, sum = 0; register unsigned int val;
    for(i = LOOP(a); i > 0; i--)
	if ((val = a[i] & b[i]) != 0)
	    sum += count_ones(val);
    return sum;
}

/* set_clear -- make "r" the empty set of "size" elements */
pset set_clear(r, size)
register pset r;
int size;
{
    register int i = WHICH_WORD(size - 1);
    *r = i; do r[i] = 0; while (--i > 0);
    return r;
}

/* set_fill -- make "r" the universal set of "size" elements */
pset set_fill(r, size)
register pset r;
register int size;
{
    register int n, i = WHICH_WORD(size - 1);
    *r = i; do r[i] = ~ 0; while (--i > 0);
    for(n = (*r) * BPI, i = size; i < n; i++) 
	set_remove(r, i);
    return r;
}

/* set_copy -- copy set a into set r */
pset set_copy(r, a)
register pset r, a;
{
    register int i = LOOPCOPY(a);
    do r[i] = a[i]; while (--i >= 0);
    return r;
}

/* set_and -- compute intersection of sets "a" and "b" */
pset set_and(r, a, b)
register pset r, a, b;
{
    register int i = LOOP(a);
    PUTLOOP(r,i); do r[i] = a[i] & b[i]; while (--i > 0);
    return r;
}

/* set_or -- compute union of sets "a" and "b" */
pset set_or(r, a, b)
register pset r, a, b;
{
    register int i = LOOP(a);
    PUTLOOP(r,i); do r[i] = a[i] | b[i]; while (--i > 0);
    return r;
}

/* set_diff -- compute difference of sets "a" and "b" */
pset set_diff(r, a, b)
register pset r, a, b;
{
    register int i = LOOP(a);
    PUTLOOP(r,i); do r[i] = a[i] & ~b[i]; while (--i > 0);
    return r;
}

/* set_xor -- compute exclusive-or of sets "a" and "b" */
pset set_xor(r, a, b)
register pset r, a, b;
{
    register int i = LOOP(a);
/*    PUTLOOP(r,i); do r[i] = a[i] ^ b[i]; while (--i > 0);*/
    PUTLOOP(r,i); do r[i] = (a[i]&~b[i]) | (~a[i]&b[i]); while (--i > 0);
    return r;
}

/* set_merge -- compute "a" & "mask" | "b" & ~ "mask" */
pset set_merge(r, a, b, mask)
register pset r, a, b, mask;
{
    register int i = LOOP(a);
    PUTLOOP(r,i); do r[i] = (a[i]&mask[i]) | (b[i]&~mask[i]); while (--i > 0);
    return r;
}

/* setp_empty -- check if the set "a" is empty */
bool setp_empty(a)
register pset a;
{
    register int i = LOOP(a);
    do if (a[i]) return FALSE; while (--i > 0);
    return TRUE;
}

/* setp_equal -- check if the set "a" equals set "b" */
bool setp_equal(a, b)
register pset a, b;
{
    register int i = LOOP(a);
    do if (a[i] != b[i]) return FALSE; while (--i > 0);
    return TRUE;
}

/* setp_disjoint -- check if intersection of "a" and "b" is empty */
bool setp_disjoint(a, b)
register pset a, b;
{
    register int i = LOOP(a);
    do if (a[i] & b[i]) return FALSE; while (--i > 0);
    return TRUE;
}

/* setp_implies -- check if "a" implies "b" ("b" contains "a") */
bool setp_implies(a, b)
register pset a, b;
{
    register int i = LOOP(a);
    do if (a[i] & ~b[i]) return FALSE; while (--i > 0);
    return TRUE;
}

/* sf_active -- make all members of the set family active */
pset_family sf_active(A)
pset_family A;
{
    register pset p, last;
    foreach_set(A, last, p)
	SET(p, ACTIVE);
    A->active_count = A->count;
    return A;
}

/* sf_inactive -- remove all inactive cubes in a set family */
pset_family sf_inactive(A)
pset_family A;
{
    register pset p, last, pdest;
    pdest = A->data;
    foreach_set(A, last, p)
	if (TESTP(p, ACTIVE)) {
	    if (pdest != p)
		INLINEset_copy(pdest, p);
	    pdest += A->wsize;
	} else
	    A->count--;
    return A;
}

/* sf_copy -- copy a set family */
pset_family sf_copy(R, A)
pset_family R, A;
{
    register int i, n = A->wsize * A->count;
    R->count = A->count;
    for(i = 0; i < n; i++) R->data[i] = A->data[i];
    return R;
}

/* sf_join -- join A and B into a single set_family */
pset_family sf_join(A, B)
pset_family A, B;
{
    pset_family R = sf_new(A->count + B->count, A->sf_size);
    register unsigned int i, n, *p, *pdest = R->data;
    R->count = A->count + B->count;
    for(i=0, p=A->data, n=A->count*A->wsize; i < n; i++)
	*pdest++ = *p++;
    for(i=0, p=B->data, n=B->count*B->wsize; i < n; i++)
	*pdest++ = *p++;
    return R;
}

/* sf_append -- append the sets of B to the end of A */
pset_family sf_append(A, B)
pset_family A, B;
{
    if (A->count + B->count > A->capacity) {
	pset_family R = sf_join(A, B);
	sf_free(A);
	A = R;
    } else {
	register unsigned int i, n, *pdest, *p;
	pdest = A->data + A->count*A->wsize;
	for(i=0, p=B->data, n=B->count*B->wsize; i < n; i++)
	    *pdest++ = *p++;
	A->count += B->count;
    }
    sf_free(B);
    return A;
}

/* set_adjcnt -- adjust the counts for a set by "weight" */
pset set_adjcnt(a, count, weight)
pset a;
int *count, weight;
{
    register int i, base; register unsigned int val;
    for(i = LOOP(a); i > 0; )
	for(val = a[i], base = --i << LOGBPI; val != 0; base++, val >>= 1)
	    if (val & 1)
		count[base] += weight;
    return a;
}

/* sf_count -- perform a column sum over a set family */
int *sf_count(A)
pset_family A;
{
    register pset p, last;
    register int i, base, *count = (int *) alloc(A->sf_size*sizeof(int));
    register unsigned int val;
    for(i = 0, base = A->sf_size; i < base;) count[i++] = 0;
    foreach_set(A, last, p)
	for(i = LOOP(p); i > 0; )
	    for(val = p[i], base = --i << LOGBPI; val != 0; base++, val >>= 1)
		if (val & 1)
		    count[base]++;
    return count;
}

/* sf_new -- allocate "num" sets of "size" elements each */
pset_family sf_new(num, size)
int num, size;
{
    pset_family A;
    if (set_family_garbage == NULL)
	A = (pset_family) alloc(sizeof(set_family_t));
    else {
	A = set_family_garbage;
	set_family_garbage = A->next;
    }
    A->sf_size = size;
    A->wsize = SET_SIZE(size);
    A->data = (pset) mem_alloc(num, A->wsize*sizeof(int), &(A->capacity));
    A->count = 0;
    return A;
}

/* sf_save -- create a duplicate copy of a set family */
pset_family sf_save(A)
register pset_family A;
{   return sf_copy(sf_new(A->count, A->sf_size), A);}

/* sf_free -- free the storage allocated for a set family */
void sf_free(A)
pset_family A;
{
    mem_free((char *) A->data);
    A->next = set_family_garbage;
    set_family_garbage = A;
}

/* sf_addset -- add a set to the end of a set family */
pset_family sf_addset(A, s)
pset_family A;
pset s;
{
    pset_family B;
    if (A->count >= A->capacity) {
	B = sf_new(A->count+1, A->sf_size);
	sf_copy(B, A);
  	sf_free(A);
	A = B;
    }
    set_copy(GETSET(A, A->count++), s);
    return A;
}

/* sf_delset -- delete a set from a set family */
void sf_delset(A, i)
pset_family A;      
int i;
{   set_copy(GETSET(A,i), GETSET(A, A->count--));}

/* sf_print -- provide debugging detail of a set family */
void sf_print(A)
pset_family A;
{
    register pset p; register int i;
    foreachi_set(A, i, p)
	printf("A[%d] = %s\n", i, ps1(p));
}

/* write_set_family -- output a set family in an unintelligable manner */
void write_set_family(fp, A)
FILE *fp;
pset_family A;
{
    register int j; register pset p, last;
    fprintf(fp, "%d %d\n", A->count, A->sf_size);
    foreach_set(A, last, p) {
	for(j = 0; j <= LOOP(p); j++)
	    fprintf(fp, "%d ", p[j]);
	fprintf(fp, "\n");
    }
}

/* read_set_family -- read a set family written by write_set_family */
pset_family read_set_family(fp)
FILE *fp;
{
    int i, j; register pset p, last;
    pset_family A;
    fscanf(fp, "%d %d\n", &i, &j);
    A = sf_new(i, j);
    A->count = i;
    foreach_set(A, last, p) {
	fscanf(fp, "%d", p);
	for(j = 1; j <= LOOP(p); j++)
	    fscanf(fp, "%d", p+j);
    }
    return A;
}

/* ps1 -- convert a set into a printable string */
char *ps1(a)
register pset a;
{
    register int i, num, l, len = 0, n = LOOP(a)*BPI;
    char temp[20];
    static char s1[160];
    bool first = TRUE;

    s1[len++] = '[';
    for(i = 0; i < n; i++)
	if (is_in_set(a,i)) {
	    if (! first)
		s1[len++] = ',';
	    first = FALSE; num = i;
	    /* Generate digits (reverse order) */
	    l = 0; do temp[l++] = num % 10 + '0'; while ((num /= 10) > 0);
	    /* Copy them back in correct order */
	    do s1[len++] = temp[--l]; while (l > 0);
	    if (len > 160-15) {
		s1[len++] = '.'; s1[len++] = '.'; s1[len++] = '.'; 
		break;
	    }
	}

    s1[len++] = ']';    
    s1[len++] = '\0';
    return s1;
}
