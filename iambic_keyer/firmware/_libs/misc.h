#ifndef _MISC_H
#define _MISC_H

#include <stdint.h>
#include <stdbool.h>

#include "stm32f0xx.h"

#define _dbg() {}
//#define _dbg() printf("%d %s\n", __LINE__, __FILE__)

void _delay_us(volatile unsigned long n);
void _delay_ms_loop(volatile unsigned long n);
void _delay_ms_systick(volatile unsigned long n);
void _delay_nms_mm(void);

#define _delay_ms _delay_ms_systick

#endif
