#ifndef _MISC_H
#define _MISC_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "stm32f10x.h"

#define _BV(x) (1UL << (x))
#define _dbg() printf("%d %s\n", __LINE__, __FILE__)
#define ok() printf("ok\n");
#define swap(x, y) do { typeof(x) _t=(x); (x)=(y); (y)=_t;} while(0)

unsigned char ByteReverse(unsigned char byte);
void _delay_us(volatile int n);
void _delay_ms_loop(volatile int n);
void _delay_ms_systick(volatile int n);
void _delay_nms_mm(void);

#define _delay_ms _delay_ms_systick

#endif
