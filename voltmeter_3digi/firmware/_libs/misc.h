#ifndef _MISC_H
#define _MISC_H

#include "stm8s.h"

#define _BV(x) (1UL << (x))
#define sbi(x, y) (x) |= (y)
#define cbi(x, y) (x) &= ~(y)
#define _delay_ms _delay_ms_systick
#define _dbg() xprintf("%d %s\n", __LINE__, __FILE__)

#define _delay_us(x) \
    do {\
        volatile unsigned long _x = ((float)F_CPU / 4e6) * x; \
        while(_x--); \
    } while(0)

#define _delay_ms_loop(x) \
    do {\
        volatile unsigned long _x = ((float)F_CPU / 4e3) * x; \
        while(_x--); \
    } while(0)

void _delay_nms_mm(void);
void _delay_ms_systick(volatile unsigned long nms);

#endif
