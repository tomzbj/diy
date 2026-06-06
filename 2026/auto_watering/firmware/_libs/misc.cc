#include "misc.h"
#include <stdlib.h>

volatile uint32_t g_millis;

void millis_tick(void)
{
  ++g_millis;
}

uint32_t millis(void)
{
  return g_millis;
}

void _delay_us(volatile int n)
{
  n *= 5;
  while(n--);
}

void DWT_ON(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;
}
