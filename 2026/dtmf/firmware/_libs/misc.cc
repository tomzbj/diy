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

void _delay_ms_loop(volatile int n)
{
  while(n--) {
    _delay_us(1000);
  }
}
