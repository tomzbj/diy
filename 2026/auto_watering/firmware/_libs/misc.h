#ifndef _MISC_H
#define _MISC_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdbool.h>

#include "stm32f1xx.h"
#include "stm32f1xx_hal_conf.h"

#define _dbg() printf("%d %s\n", __LINE__, __FILE__)
#define print_log() printf("%11lu %d %s\n", DWT->CYCCNT, __LINE__, __FILE__)
#define Error_Handler() do { _dbg(); while(1); }while(0)
#define test_if(x) do { if((x) != HAL_OK) while(1); } while(0)

void _delay_us(volatile int n);
uint32_t millis(void);
void millis_tick(void);
void DWT_ON(void);

static inline int saturate(int val, int min, int max)
{
  val = (val < min) ? min : val;
  val = (val > max) ? max : val;
  return val;
}

#endif
