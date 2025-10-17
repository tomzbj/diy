#ifndef _MISC_H
#define _MISC_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdbool.h>

#include "gd32f4xx.h"

#define _dbg() printf("%d %s\n", __LINE__, __FILE__)
#define print_log()    printf("[%-10lu %-20s %3d]\n", DWT->CYCCNT, __FILE__, __LINE__)

void _delay_us(volatile int n);
void _delay_ms_loop(volatile int n);
void _delay_ms_systick(volatile int n);
void _delay_nms_mm(void);

static inline int saturate(int val, int min, int max)
{
  val = (val < min) ? min : val;
  val = (val > max) ? max : val;
  return val;
}

#define _delay_ms _delay_ms_systick

#endif
