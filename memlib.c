#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<unistd.h>
#include<sys/mman.h>
#include<string.h>
#include<errno.h>

#include"csapp.h"
#include"memlib.h"

#define MAX_HEAP (20*(1<<20))

static char *mem_heap;
static char *mem_brk;
static char *mem_max_addr;

void mem_init(void)
{
	mem_heap=(char *) Malloc(MAX_HEAP);
	mem_brk=(char *)mem_heap;
	mem_max_addr(char *)(mem_heap+MAX_HEAP);
}

void *mem_sbrk(int incr)
{
	char *old_brk=mem_brk;
	if((incr<0)||((mem_brk+incr)>mem_max_addr))
	{
		errno=ENOMEM;
		fprintf(stderr,"ERROR:mem_sbrk failed.Ran out of memory..\n");
		return (void *)-1;
	}	
	mem_brk += incr;
	return (void *)old_brk;
}

void mem_deinit(void)
{
}

void mem_reset_brk()
{
	mem_brk=(char *)mem_heap;
}

void *mem_heap_lo
{
	return (void *)mem_heap;
}
void *mem_heap_hi()
{
	return (void *)(mem_brk-1);
}
size_t mem_heapsize()
{
	return (size_t)((void *)mem_brk-(void *)mem_heap);
}
size_t mem_pagesize()
{
	return (size_t)getpagesize();
}
