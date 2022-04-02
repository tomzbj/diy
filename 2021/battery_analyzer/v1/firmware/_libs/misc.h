#ifndef _USER_MISC_H
#define _USER_MISC_H

#include <inttypes.h> 
#include <stdbool.h>
#include <stdlib.h>
#include "stm32f10x.h"

//#include "usart.h"
//#include "disp.h"

unsigned char ByteReverse(unsigned char byte);
void _delay_us(volatile unsigned long n);
void _delay_ms_loop(volatile unsigned long n);
void _delay_ms_systick(volatile unsigned long n);
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
