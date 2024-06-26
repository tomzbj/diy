#ifndef _MISC_H
#define _MISC_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdbool.h>

#include "cmsis_gcc.h"
#include "stm32f0xx_hal_conf.h"
#include "stm32f0xx.h"

#define _dbg() printf("%d %s\n", __LINE__, __FILE__)

void _delay_us(volatile int n);
void _delay_ms_loop(volatile int n);
void _delay_ms_systick(volatile int n);
void _delay_nms_mm(void);

#define _delay_ms _delay_ms_systick

#endif
