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
  // Busy-loop tune for ~48MHz SYSCLK (was *5 @ 144MHz)
  n *= 2;
  while(n--);
}

void _delay_ms_loop(volatile int n)
{
  while(n--) {
    _delay_us(1000);
  }
}

void DWT_ON(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;
}
