/// @file syscall.c
/// @brief 
/// @author Zhang Hao, stavrosatic@gmail.com
/// @version R1.5
/// @date 2017-10-12

#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
//#include "usart_f0.h"

#undef errno
extern int errno;
extern int _end;

/// @brief _sbrk 
/// @param incr
/// @retval None
caddr_t _sbrk(int incr)
{
    register unsigned char *pStack asm("sp");
    static unsigned char *s_pHeapEnd; 

    if (!s_pHeapEnd) 
        s_pHeapEnd = (unsigned char *)&_end; 

    if (s_pHeapEnd + incr > pStack)     // conflict, failed to alloc
        return (caddr_t)-1; 

    unsigned char *pOldHeapEnd = s_pHeapEnd; 
    s_pHeapEnd += incr; 
    return (caddr_t)pOldHeapEnd; 
}

/// @brief link 
/// @param old
/// @param new
/// @retval None
int link(char *old, char *new)
{
    return -1;
}

/// @brief _close 
/// @param file
/// @retval None
int _close(int file)
{
    return -1;
}

/// @brief _fstat 
/// @param file
/// @param st
/// @retval None
int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

/// @brief _isatty 
/// @param file
/// @retval None
int _isatty(int file)
{
    return 1;
}

/// @brief _lseek 
/// @param file
/// @param ptr
/// @param dir
/// @retval None
int _lseek(int file, int ptr, int dir)
{
    return 0;
}

/// @brief _read 
/// @param file
/// @param ptr
/// @param len
/// @retval None
int _read(int file, char *ptr, int len)
{
    return 0;
}

/// @brief abort 
void abort(void)
{
    /* Abort called */
    while (1) ;
}

/// @brief _write 
/// @param file
/// @param ptr
/// @param len
/// @retval None
int _write(int file, char *ptr, int len)
{
    unsigned short todo;
    for (todo = 0; todo < len; todo++) {
//        USART_SendData(USART1, *ptr++);
//        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }

    return len;
}
