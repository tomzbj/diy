#ifndef _MISC_H
#define _MISC_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdbool.h>

#include "stm32f0xx.h"

#define _dbg() printf("%d %s\n", __LINE__, __FILE__)
#define print_log() printf("%d %s\n", __LINE__, __FILE__)
//#define Error_Handler()  do { _dbg(); while(1); }while(0)
#define Error_Handler() while(1)
#define _delay_ms HAL_Delay

void _delay_us(volatile int n);
//void _delay_ms_loop(volatile int n);
//void _delay_ms_systick(volatile int n);
//void _delay_nms_mm(void);
uint32_t millis(void);
void millis_tick(void);

//#define _delay_ms _delay_ms_systick

#endif
