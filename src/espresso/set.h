/*
    set.h -- definitions for packed arrays of bits

    This header file describes the data structures which comprise a
    facility for efficiently implementing packed arrays of bits
    (otherwise known as sets, cf. Pascal).

    A set is a vector of bits and is implemented here as an array of
    unsigned integers.  The low order bits of set[0] give the index
    of the last word of set data.  The higher order bits of set[0] are
    used to store data associated with the set.  The set data is
    contained in elements set[1] ... set[LOOP(set)] as a packed bit
    array.

    A family of sets is a two-dimensional matrix of bits and is
    implemented with the data type "set_family".
*/

/* Define host machine characteristics of "unsigned int" */
#define BPI             32		/* # bits per integer */
#define LOGBPI		5		/* log(BPI)/log(2) */

/* Define the set type */
typedef unsigned *pset;

/* Define the set family type -- an array of sets */
typedef struct set_family {
    int wsize;			/* Size of each set in 'ints' */
    int sf_size;		/* User declared set size */
    int capacity;		/* Number of sets allocated */
    int count;                  /* The number of sets in the family */
    int active_count;		/* Number of "active" sets */
    pset data;			/* Pointer to the set data */
    struct set_family *next;	/* For garbage collection */
} set_family_t, *pset_family;

/* Macros to set and test single elements */
#define WHICH_WORD(element)	(((element) >> LOGBPI) + 1)
#define WHICH_BIT(element)	(1 << ((element) & (BPI-1)))

/* # of ints needed to allocate a set with "size" elements */
#if BPI == 32
#define SET_SIZE(size)		((size) <= 0 ? 2 : (WHICH_WORD((size)-1) + 1))
#else
#define SET_SIZE(size)		((size) <= 0 ? 3 : (WHICH_WORD((size)-1) + 2))
#endif

/*
  Three fields are maintained in the first word of the set
	LOOP is the index of the last word used for set data
	LOOPCOPY is the index of the last word in the set
	SIZE is available for general use (e.g., storing set size)
	FLAGS store general information about the set
*/
#define LOOP(set)		(set[0] & 0x03ff)
#define PUTLOOP(set, i)		(set[0] &= ~0x03ff, set[0] |= (i))
#if BPI == 32
#define LOOPCOPY(set)		LOOP(set)
#define SIZE(set)		(set[0] >> 16)
#define PUTSIZE(set, size)	(set[0] &= 0xffff, set[0] |= ((size) << 16))
#else
#define LOOPCOPY(set)		(LOOP(set) + 1)
#define SIZE(set)		(set[LOOP(set)+1])
#define PUTSIZE(set, size)	((set[LOOP(set)+1]) = (size))
#endif
#define SET(set, flag)		(set[0] |= (flag))
#define RESET(set, flag)	(set[0] &= ~ (flag))
#define TESTP(set, flag)	(set[0] & (flag))

/* Flag definitions are ... */
#define PRIME		0x8000		/* cube is prime */
#define NONESSEN	0x4000		/* cube cannot be essential prime */
#define ACTIVE		0x2000		/* cube is still active */
#define REDUND		0x1000		/* cube is redundant(at this point) */
#define COVERED		0x0800		/* cube has been covered */
#define RELESSEN	0x0400		/* cube is relatively essential */

/* Most efficient way to look at all members of a set family */
#define foreach_set(R, last, p) \
    for(p=R->data,last=p+R->count*R->wsize;p<last;p+=R->wsize)
#define foreach_remaining_set(R, last, pfirst, p) \
    for(p=pfirst+R->wsize,last=R->data+R->count*R->wsize;p<last;p+=R->wsize)
#define foreach_active_set(R, last, p) \
    foreach_set(R,last,p) if (TESTP(p, ACTIVE))

/* Another way that also keeps the index of the current set member in i */
#define foreachi_set(R, i, p) \
    for(p=R->data,i=0;i<R->count;p+=R->wsize,i++)
#define foreachi_active_set(R, i, p) \
    foreachi_set(R,i,p) if (TESTP(p, ACTIVE))

/* Return a pointer to a given member of a set family */
#define GETSET(family, index)	((family)->data + (family)->wsize * (index))

/* Allocate and deallocate sets */
#define set_new(size) /*NOSTRICT*/ \
    set_clear((pset) alloc(SET_SIZE(size)*sizeof(int)),size)
#define set_save(r)   /*NOSTRICT*/ \
    set_copy((pset) alloc((int) (SET_SIZE(LOOP(r)*BPI))*sizeof(int)), r)
#define set_free(r)   /*NOSTRICT*/ \
    mem_free((char *) r)

/* Check for set membership, remove set element and insert set element */
#define is_in_set(set, e)	(set[WHICH_WORD(e)] & WHICH_BIT(e))
#define set_remove(set, e)	(set[WHICH_WORD(e)] &= ~ WHICH_BIT(e))
#define set_insert(set, e)	(set[WHICH_WORD(e)] |= WHICH_BIT(e))

/* Inline code substitution for those places that REALLY need it on a VAX */
#define INLINEset_copy(r, a) \
{register i_=LOOPCOPY(a); do r[i_] = a[i_]; while (--i_ >= 0);}
#define INLINEset_clear(r, size) \
{register i_=WHICH_WORD(size-1); *r=i_; do r[i_] = 0; while (--i_ > 0);}
#define INLINEset_and(r, a, b) \
{register i_=LOOP(a); PUTLOOP(r,i_); do r[i_] = a[i_]&b[i_]; while (--i_>0);}
#define INLINEset_or(r, a, b) \
{register i_=LOOP(a); PUTLOOP(r,i_); do r[i_] = a[i_]|b[i_]; while (--i_>0);}
#define INLINEset_diff(r, a, b) \
{register i_=LOOP(a); PUTLOOP(r,i_); do r[i_] = a[i_]&~b[i_]; while (--i_>0);}
#define INLINEsetp_implies(a, b, when_false) \
{register i_=LOOP(a); do if (a[i_]&~b[i_]) break; while (--i_>0); \
if (i_ != 0) when_false;}

#if BPI == 32
#define count_ones(v)	(bit_count[v & 255] + bit_count[(v >> 8) & 255]	\
    + bit_count[(v >> 16) & 255] + bit_count[(v >> 24) & 255])
#else
#define count_ones(v)	(bit_count[v & 255] + bit_count[(v >> 8) & 255])
#endif

/* Table for efficient bit counting */
EXTERN int bit_count[256]
#ifdef INIT
={0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8}
#endif
;
