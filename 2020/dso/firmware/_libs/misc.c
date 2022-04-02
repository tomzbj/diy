#include "misc.h"
#include <stdio.h>

static volatile int g_nms;

void _delay_us(volatile int n)
{
    n *= 8;
    while(n--);
}

void _delay_ms_loop(volatile int n)
{
    while(n--)
        _delay_us(1000);
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
