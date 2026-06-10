#ifndef _USER_MISC_H
#define _USER_MISC_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "py32f0xx.h"

#define ok() printf("ok\n")
//#define _dbg() printf("%4lu %4d %s\n", millis(), __LINE__, __FILE__)
#define _dbg() printf("%4d %s\n", __LINE__, __FILE__)
#define print_log() printf("%4lu %4d %s\n", millis(), __LINE__, __FILE__)
#define Error_Handler()  while(1) //do {_dbg(); while(1);} while(0);

void _delay_us(volatile int n);

void millis_tick(void);
uint32_t millis(void);

static inline int saturate(int val, int min, int max)
{
  val = (val < min) ? min : val;
  val = (val > max) ? max : val;
  return val;
}

#endif
