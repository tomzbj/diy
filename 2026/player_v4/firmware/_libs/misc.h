#ifndef _MISC_H
#define _MISC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f4xx.h"

#define _dbg() printf("%d %s\n", __LINE__, __FILE__)
#define print_log() printf("%d %s\n", __LINE__, __FILE__)
#define Error_Handler() while(1)

  void DWT_ON(void);
  int saturate(int val, int min, int max);
  void _delay_us(volatile int n);
  uint32_t millis(void);
  void millis_tick(void);

//#define _delay_ms _delay_ms_systick
#define _delay_ms HAL_Delay

#ifdef __cplusplus
}
#endif

#endif
