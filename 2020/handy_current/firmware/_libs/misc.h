/// @file misc.h
/// @brief 
/// @author Zhang Hao, stavrosatic@gmail.com
/// @version R1.5
/// @date 2017-10-13
#ifndef _MISC_H
#define _MISC_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "stm32f0xx.h"
#include "config.h"

#define sbi(x, y) (x)->BSRR = (y)
#define cbi(x, y) (x)->BRR = (y)
#define _BV(x) (1UL << (x))
#define bit_is_set(x, y) GPIO_ReadInputDataBit((x), (y))
#define bit_is_clear(x, y) !GPIO_ReadInputDataBit((x), (y))
#define _dbg() printf("%d %s\n", __LINE__, __FILE__)

unsigned char ByteReverse(unsigned char byte);
void _delay_us(volatile unsigned long n);
void _delay_ms_loop(volatile unsigned long n);
void _delay_ms_systick(volatile unsigned long n);
void _delay_nms_mm(void);
unsigned char intlog2(int n);

#define _delay_ms _delay_ms_systick

#endif
