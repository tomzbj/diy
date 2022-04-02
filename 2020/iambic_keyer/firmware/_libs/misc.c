#include "misc.h"

volatile unsigned long g_nms;

void _delay_us(volatile unsigned long n)
{
    n *= 5;
    while (n--) ;
}

void _delay_ms_loop(volatile unsigned long n)
{
    while (n--) {
        _delay_us(1000);
    }
}

void _delay_ms_systick(volatile unsigned long nms)
{
    g_nms = nms;
    while (g_nms > 0) ;
}

void _delay_nms_mm(void)
{
    if (g_nms > 0)
        g_nms--;
}
