#include "misc.h"

static unsigned long g_nms;

void _delay_ms_systick(volatile unsigned long nms)
{
    g_nms = nms;
    while(g_nms > 0)
        ;
}

void _delay_nms_mm(void)
{
    if(g_nms > 0)
        g_nms--;

}

void assert_failed()
{
}
