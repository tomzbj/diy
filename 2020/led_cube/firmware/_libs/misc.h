#ifndef _USER_MISC_H
#define _USER_MISC_H

#include <inttypes.h> 
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "stm32f0xx.h"

#define _BV(x) (1UL << (x))
#define bit_is_set(x, y) ((x)->IDR & (y))
#define bit_is_clear(x, y) (!((x)->IDR & (y)))
#define bit_is_reset(x, y) (!((x)->IDR & (y)))
#define ok() printf("ok\n")

void _delay_us(volatile int n);
void _delay_ms_loop(volatile int n);
void _delay_ms_systick(volatile int n);
void _delay_nms_mm(void);
size_t get_free_mem(void);

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define abs(x) ((x) > 0 ? (x) : (-(x)))
#define swap(x, y) \
    do { typeof(x) __tmp = (x); (x) = (y); (y) = __tmp;}while(0)

#define _delay_ms _delay_ms_systick
#define _dbg() printf("%d %s\n", __LINE__, __FILE__)

#endif
