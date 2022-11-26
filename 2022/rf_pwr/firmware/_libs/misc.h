#ifndef _MISC_H
#define _MISC_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#include <stdbool.h>

#include "gd32f3x0.h"


#define _dbg() printf("%d %s\n", __LINE__, __FILE__)
#define _BV(x) (1UL << (x))
//#define print_log()  do {} while(0)
/*
#define print_log()  do { \
    char tmp[128];\
    sprintf(tmp, "[%-10lu %-20s %3d]\n", DWT->CYCCNT, __FILE__, __LINE__);\
    UG_ConsolePutString(tmp);\
    } while(0)
    */


#define chkret(expr) ((expr == 0) ? chk_ok() : chk_failed(expr, (char*)__FILE__, __LINE__))
#define chk_failed(x, file, line) ({printf("Err %d %s line %d\n", x, file, line); return;})
#define chk_ok() (void)0

void _delay_us(volatile int n);
void _delay_ms_loop(volatile int n);
void _delay_ms_systick(volatile int n);
void _delay_nms_mm(void);

#define _delay_ms _delay_ms_systick

#endif
