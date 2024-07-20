
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Some memory */
static char memory[1024 * 180];
static char *freep = memory;
static int left = sizeof(memory);

__THROW __attribute_malloc__
void *malloc (size_t size)
{
    void *mem = (void*)freep;
    freep += size;
    left -= size;
    if (left < 0)
    {
#ifndef __riscos
        printf("OUT OF MEMORY - need %i bytes\n", size);
#endif
        exit(1);
    }
    return mem;
}

void *calloc(size_t count, size_t size)
{
    int total = count * size;
    void *mem = malloc(total);
    memset(mem, 0, size);
    return mem;
}

void free(void *block)
{
    /* Nothing to do */
}
