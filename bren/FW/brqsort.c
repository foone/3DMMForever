/*
 * $Id: brqsort.c 1.9 1995/05/25 13:23:09 sam Exp $
 * $Locker:  $
 *
 * Local quicksort for brender
 *
 * Based on a public domain verion by Raymond Gardner,
 * Englewood CO  February 1991
 */

#include "brender.h"

#if !defined(__TARGET_MSC__) && (defined(__WATCOMC__) || defined(_MSC_VER))
#include <stdlib.h>

void BR_PUBLIC_ENTRY BrQsort(void *basep, unsigned int nelems, unsigned int size, br_qsort_cbfn *comp)
{
    qsort(basep, nelems, size, comp);
}

#elif __IBMC__
/*
 * Pull qsort from a BRender specific DLL
 */
void _System BrSupport1(br_qsort_cbfn *comp, unsigned int size, void *basep, unsigned int nelems);

void BR_PUBLIC_ENTRY BrQsort(void *basep, unsigned int nelems, unsigned int size, br_qsort_cbfn *comp)
{
    BrSupport1(comp, size, basep, nelems);
}
#else

static void swap_chars(char *, char *, unsigned int);

#define SWAP_INTS 1

/*
** Compile with -DSWAP_INTS if your machine can access an int at an
** arbitrary location with reasonable efficiency.  (Some machines
** cannot access an int at an odd address at all, so be careful.)
*/

#ifdef SWAP_INTS
static void swap_ints(char *, char *, unsigned int);
static void swap_int_1(char *ap, char *bp, unsigned int nints);

#define SWAP(a, b) (swap_func((char *)(a), (char *)(b), width))
#else
#define SWAP(a, b) (swap_chars((char *)(a), (char *)(b), size))
#endif

#define COMP(a, b) ((*comp)((void *)(a), (void *)(b)))

#define T 7 /* subfiles of T or fewer elements will */
/* be sorted by a simple insertion sort */
/* Note!  T must be at least 3          */

void BR_PUBLIC_ENTRY BrQsort(void *basep, unsigned int nelems, unsigned int size, br_qsort_cbfn *comp)
{
    char *stack[40], **sp;                           /* stack and stack pointer        */
    char *i, *j, *limit;                             /* scan and limit pointers        */
    unsigned int thresh;                             /* size of T elements in bytes    */
    char *base;                                      /* base pointer as char *         */

#ifdef SWAP_INTS
    unsigned int width;                              /* width of array element         */
    void (*swap_func)(char *, char *, unsigned int); /* swap func pointer*/

    width = size;           /* save size for swap routine     */
    swap_func = swap_chars; /* choose swap function           */
    if (size == sizeof(int))
    {                           /* size is == int */
        swap_func = swap_int_1; /* use int swap function    */
    }
    else if (size % sizeof(int) == 0)
    {                          /* size is multiple of ints */
        width /= sizeof(int);  /* set width in ints        */
        swap_func = swap_ints; /* use int swap function    */
    }
#endif

    base = (char *)basep;         /* set up char * base pointer     */
    thresh = T * size;            /* init threshold                 */
    sp = stack;                   /* init stack pointer             */
    limit = base + nelems * size; /* pointer past end of array      */
    for (;;)
    { /* repeat until break...          */
        if ((unsigned)(limit - base) > thresh)
        {   /* if more than T elements  */
            /*   swap base with middle  */
            SWAP((((limit - base) / size) / 2) * size + base, base);
            i = base + size;       /* i scans left to right    */
            j = limit - size;      /* j scans right to left    */
            if (COMP(i, j) > 0)    /* Sedgewick's              */
                SWAP(i, j);        /*    three-element sort    */
            if (COMP(base, j) > 0) /*        sets things up    */
                SWAP(base, j);     /*            so that       */
            if (COMP(i, base) > 0) /*      *i <= *base <= *j   */
                SWAP(i, base);     /* *base is pivot element   */
            for (;;)
            {                  /* loop until break         */
                do             /* move i right             */
                    i += size; /*        until *i >= pivot */
                while (COMP(i, base) < 0);
                do             /* move j left              */
                    j -= size; /*        until *j <= pivot */
                while (COMP(j, base) > 0);
                if (i > j)  /* if pointers crossed      */
                    break;  /*     break loop           */
                SWAP(i, j); /* else swap elements, keep scanning*/
            }
            SWAP(base, j); /* move pivot into correct place  */
            if (j - base > limit - i)
            {                 /* if left subfile larger */
                sp[0] = base; /* stack left subfile base  */
                sp[1] = j;    /*    and limit             */
                base = i;     /* sort the right subfile   */
            }
            else
            {                  /* else right subfile larger*/
                sp[0] = i;     /* stack right subfile base */
                sp[1] = limit; /*    and limit             */
                limit = j;     /* sort the left subfile    */
            }
            sp += 2; /* increment stack pointer  */
        }
        else
        { /* else subfile is small, use insertion sort  */
            for (j = base, i = j + size; i < limit; j = i, i += size)
                for (; COMP(j, j + size) > 0; j -= size)
                {
                    SWAP(j, j + size);
                    if (j == base)
                        break;
                }
            if (sp != stack)
            {            /* if any entries on stack  */
                sp -= 2; /* pop the base and limit   */
                base = sp[0];
                limit = sp[1];
            }
            else /* else stack empty, done   */
                break;
        }
    }
}

/*
**  swap nbytes between a and b
*/

static void swap_chars(char *a, char *b, unsigned int nbytes)
{
    char tmp;
    do
    {
        tmp = *a;
        *a++ = *b;
        *b++ = tmp;
    } while (--nbytes);
}

#ifdef SWAP_INTS

/*
**  swap nints between a and b
*/

static void swap_ints(char *ap, char *bp, unsigned int nints)
{
    int *a = (int *)ap, *b = (int *)bp;
    int tmp;
    do
    {
        tmp = *a;
        *a++ = *b;
        *b++ = tmp;
    } while (--nints);
}

static void swap_int_1(char *ap, char *bp, unsigned int nints)
{
    int *a = (int *)ap, *b = (int *)bp;
    int tmp;

    tmp = *a;
    *a++ = *b;
    *b++ = tmp;
}

#endif
#endif
