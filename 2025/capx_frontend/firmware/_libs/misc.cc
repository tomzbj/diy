#include "misc.h"
#include <stdlib.h>

volatile int g_nms;
volatile uint32_t g_millis;

uint32_t millis(void)
{
  return g_millis;
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
  ++g_millis;
}
