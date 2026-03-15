#ifndef _MISC_H
#define _MISC_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdbool.h>

#include "stm32f1xx.h"
#include "stm32f1xx_hal_conf.h"
#define Error_Handler() while(1) //do { print_log(); while(1); }while(0)

#define _dbg() printf("%d %s\n", __LINE__, __FILE__)
#define print_log()    printf("[%-10lu %-20s %3d]\n", DWT->CYCCNT, __FILE__, __LINE__)
#define test_if(x) do { if((x) != HAL_OK) while(1); } while(0)

void _delay_us(volatile int n);
uint32_t millis(void);
void millis_tick(void);

static inline int saturate(int val, int min, int max)
{
  val = (val < min) ? min : val;
  val = (val > max) ? max : val;
  return val;
}

#endif
