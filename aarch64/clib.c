#include <stdlib.h>
#include "swis.h"

void memcpy(char *dst, const char *src, int size)
{
    while (size--)
        *dst++ = *src++;
}


void memset(char *dst, int c, int size)
{
    while (size--)
        *dst++ = c;
}


char *strcpy(char *dst, const char *src)
{
    char *odst = dst;
    while (1)
    {
        int c = *src++;
        *dst++ = c;
        if (!c)
            break;
    }
    return odst;
}


size_t strlen(char *s)
{
    char *os = s;
    while (*s++ != '\0')
        ;
    return s - os - 1;
}


char *strchr(char *s, char want)
{
    while (*s != want)
        if (*s == '\0')
            return NULL;
        s++;
    return s;
}


int clock(void)
{
    _kernel_swi_regs regs;
    _kernel_swi(OS_ReadMonotonicTime, NULL, &regs);
    return regs.r[0];
}
