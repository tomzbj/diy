#ifndef _MISC_H
#define _MISC_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "stm32f0xx.h"

#define xprintf printf

#define _dbg() xprintf("%d %s\n", __LINE__, __FILE__)

void _delay_us(volatile int n);
void _delay_ms_loop(volatile int n);
void _delay_ms_systick(volatile int n);
void _delay_nms_mm(void);

#define _delay_ms _delay_ms_systick

#endif
