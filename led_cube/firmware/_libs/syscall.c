#include <stdio.h>
#include <stdarg.h>
#include <stdarg.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include "misc.h"
#include "usart_f0.h"

#undef errno
extern int errno;
extern int _end;

caddr_t _sbrk(int incr)
{
    register unsigned char *pStack asm("sp");
    static unsigned char *s_pHeapEnd; 

    if (!s_pHeapEnd) 
        s_pHeapEnd = (unsigned char *)&_end; 
    if (s_pHeapEnd + incr > pStack)
        return (caddr_t) - 1; 
    unsigned char *pOldHeapEnd = s_pHeapEnd; 
    s_pHeapEnd += incr; 
    return (caddr_t)pOldHeapEnd; 
}

int link(char *old, char *new)
{
    return -1;
}

int _close(int file)
{
    return -1;
}

int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

int _read(int file, char *ptr, int len)
{
    return 0;
}

void abort(void)
{
    while (1) ;
}

int _write(int file, char *ptr, int len)
{
    unsigned short todo;
    for (todo = 0; todo < len; todo++) {
        USART_SendData(USART3, *ptr++);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    } 
    return len;
}
