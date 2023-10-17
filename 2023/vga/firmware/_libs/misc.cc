#include "misc.h"
#include <stdlib.h>

volatile int g_nms, g_millis;

int millis(void)
{
    return g_millis;
}

void millis_tick(void)
{
    g_millis++;
}

void _delay_us(volatile int n)
{
    n *= 5;
    while(n--);
}

void _delay_ms_loop(volatile int n)
{
    while(n--) {
        _delay_us(1000);
    }
}

void _delay_ms_systick(volatile int nms)
{
    g_nms = nms;
    while(g_nms > 0);
}

void _delay_nms_mm(void)
{
    if(g_nms > 0)
        g_nms--;
}

static size_t __get_free_mem(size_t start, size_t end)
{
    unsigned char* p;
    size_t size = (start + end) / 2;

    if(start == end - 1)
        return start - 1;

    p = (unsigned char*)malloc(size);
    if(p != NULL) {     // malloc succeeded
        p[0] = 0xff;
        p[size - 1] = 0xff;
        free(p);
        return __get_free_mem(size + 1, end);
    }
    else {              // malloc failed
        return __get_free_mem(start, size);
    }
}

size_t get_free_mem(void)
{
    return __get_free_mem(0, 32700UL);
}
