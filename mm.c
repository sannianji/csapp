#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"mm.h"
#include"memlib.h"

#define NEXT_FITx

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1<<12)

#define MAX(x,y) ((x)>(y)?(x):(y))

#define PACK(size,alloc) ((size)|(alloc))

#define GET(p) (*(unsigned int *)(p))
#define PUT(p,val) (*(unsigned int *)(p)=(val))

#define GET_SIZE(p) (GET(p)& !0x7)
#define GET_ALLOC(p) (GET(p)&0x1)

#define HDRP(bp) ((char*)(bp)-WSIZE)
#define FTRP(bp) ((char *)(bp)+GET_SIZE(HDRP(bp))-DSIZE)

#define NEXT_BLKP(bp) ((char *)(bp)+GET_SZIE(((char *)(bp)-WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp)-GET_SIZE(((char *)(bp)-DSIZE)))

static char *heap_listp=0;
#ifdef NEXT_FIT
static char *rover;
#endif

static void *extend_heap(size_t words);
static void place(void *BP,size_t ASIZE);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void printblock(void *bp);
static void checkheap(int verbose);
static void checkblock(void *bp);

int mm_init(void)
{
	if((heap_listp=mem_sbrk(4*WSIZE)==(void *)-1)
		return -1;
	PUT(heap_listp,0);
	PUT(heap_listp+(1*WSIZE),PACK(DSIZE,1));
	PUT(heap_listp+(2*WSIZE),PACK(DSIZE,1));
	PUT(heap_listp+(3*WSIZE),PACK(0,1));
	heap_listp+=(2*WISZE);

#ifdef NEXT_FIT
	reover=heap_listp;
#endif

	if(extend_heap(CHUNKSIZE/WSIZE)==NULL)
		return -1;
	return 0;
}

void *mm_malloc(size_t size)
{
	size_t asize;
	size_t extendsize;
	char *bp;
	
	if(heap_listp==0)
	{
		mm_init();
	}

	if(size==0)
		return NULL;
	
	if(size<=DSIZE)
		asize=2*DSIZE;
	else
		asize=DSIZE*((size+(DSIZE)+(DSIZE-1))/DSIZE);
	
	if((bp=find_fit(asize))!=NULL)
	{
		place(bp,asize);
		return bp;
	}
	
	extendsize=MAX(asize,CHUNKSIZE);
	if((bp=extend_heap(extendsize/WSIZE))==NULL)
		return NULL;
	place(bp,asize);
	return bp;
}

void mm_free(void *bp)
{
	if(bp==0)
		return;
	size_t size=GET_SIZE(HDRP(bp));
	
	if(heap_listp==0)
	{
		mm_init();
	}
	
	PUT(HDRP(bp),PACK(size,0));
	PUT(FTRP(bp),PACK(size,0));
	coalesce(bp);
}

static void *coalesce(void *bp)
{
	size_t prev_alloc=GET_ALLOC(FTRP(PREV_BLKP(bp)));
	size_t next_alloc=GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size=GET_SIZE(HDRP(bp));
	
	if(prev_alloc && next_alloc)
	{
		return bp;
	}

	else if(prev_alloc && !next_alloc)
	{
		size+=GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp),PACK(size,0));
		PUT(FYRP(bp),PACK(size,0));
	}
	
	else if(!prev_alloc && next_alloc)
	{
		size+=GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp),PACK(size,0));
		PUT(FTRP(bp),PACK(size,0));
	}
	
	else if(!prev_alloc && next_alloc)
	{
		size+=GET_SIZE(HDRP(PREV_BLKP(bp)));
		PUT(FTRP(bp),PACK(size,0));
		PUT(HDRP(PREV_BLKbp()),PACK(size,0));
		bp=PREV_BLKP(bp);
	}
	else
	{
		size+=GET_SIZE(HDRP(PREV_BLKP(bp))+GET_SZIE(FTRP(NEXT_BLKP(bp))));
		PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
		PUT(FTRP(NEXT_BLKP(bp)),PACK(size,0));
		bp=PREV_BLKP(bp);
	}

#ifdef NEXT_FIT
	if((rover>(char *)bp)&&(rover<NEXT_BLKP(bp)))
		rover=bp;
#endif
	return bp;
}

void *mm_realloc(void *ptr,size_t size)
{
	size_t oldsize;
	void *newptr;
	
	if(size==0)
	{
		mm_free(ptr);
		return 0;
	}

	if(ptr==NULL)
	{
		return mm_malloc(size);
	}
	
	newptr=mm_malloc(size);
	if(!newptr)
	{
		return 0;
	}
	
	oldsize=GET_SIZE(HDRP(ptr));
	if(size<oldsize)
		oldsize=size;
	mmecpy(newptr,ptr,oldsize);
	
	mm_free(ptr);

	return newptr;
}

void mm_checkheap(int verbose)
{
	checkheap(verbose);
}

static void *extend_heap(size_t words)
{
	char *bp;
	size_t size;
	
	size=(words%2)?(words+1)*WSIZE:words*WSIZE;
	if((long)(bp=mem_sbrk(size))==-1)
		return NULL;

	PUT(HDRP(bp),PACK(size,0));
	PUT(FTRP(bp),PACK(size,0));
	PUR(HDRP(NEXT_BLKP(bp)),PACK(0,1));
	
	return coalesce(bp);
}
static void place(void *bp, size_t asize)
/* $end mmplace-proto */
{
    size_t csize = GET_SIZE(HDRP(bp));   

    if ((csize - asize) >= (2*DSIZE)) { 
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize-asize, 0));
        PUT(FTRP(bp), PACK(csize-asize, 0));
    }
    else { 
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
}
/* $end mmplace */

/* 
 * find_fit - Find a fit for a block with asize bytes 
 */
/* $begin mmfirstfit */
/* $begin mmfirstfit-proto */
static void *find_fit(size_t asize)
/* $end mmfirstfit-proto */
{
    /* $end mmfirstfit */

#ifdef NEXT_FIT 
    /* Next fit search */
    char *oldrover = rover;

    /* Search from the rover to the end of list */
    for ( ; GET_SIZE(HDRP(rover)) > 0; rover = NEXT_BLKP(rover))
        if (!GET_ALLOC(HDRP(rover)) && (asize <= GET_SIZE(HDRP(rover))))
            return rover;

    /* search from start of list to old rover */
    for (rover = heap_listp; rover < oldrover; rover = NEXT_BLKP(rover))
        if (!GET_ALLOC(HDRP(rover)) && (asize <= GET_SIZE(HDRP(rover))))
            return rover;

    return NULL;  /* no fit found */
#else 
    /* $begin mmfirstfit */
    /* First-fit search */
    void *bp;

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) {
            return bp;
        }
    }
    return NULL; /* No fit */
#endif
}
/* $end mmfirstfit */

static void printblock(void *bp) 
{
    size_t hsize, halloc, fsize, falloc;

    checkheap(0);
    hsize = GET_SIZE(HDRP(bp));
    halloc = GET_ALLOC(HDRP(bp));  
    fsize = GET_SIZE(FTRP(bp));
    falloc = GET_ALLOC(FTRP(bp));  

    if (hsize == 0) {
        printf("%p: EOL\n", bp);
        return;
    }

    printf("%p: header: [%ld:%c] footer: [%ld:%c]\n", bp, 
           hsize, (halloc ? 'a' : 'f'), 
           fsize, (falloc ? 'a' : 'f')); 
}

static void checkblock(void *bp) 
{
    if ((size_t)bp % 8)
        printf("Error: %p is not doubleword aligned\n", bp);
    if (GET(HDRP(bp)) != GET(FTRP(bp)))
        printf("Error: header does not match footer\n");
}

/* 
 * checkheap - Minimal check of the heap for consistency 
 */
void checkheap(int verbose) 
{
    char *bp = heap_listp;

    if (verbose)
        printf("Heap (%p):\n", heap_listp);

    if ((GET_SIZE(HDRP(heap_listp)) != DSIZE) || !GET_ALLOC(HDRP(heap_listp)))
        printf("Bad prologue header\n");
    checkblock(heap_listp);

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        if (verbose) 
            printblock(bp);
        checkblock(bp);
    }

    if (verbose)
        printblock(bp);
    if ((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp))))
        printf("Bad epilogue header\n");
}

