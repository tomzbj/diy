#ifndef _USER_MISC_H
#define _USER_MISC_H

#include <cstdint>
#include <cstdbool>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "py32f0xx.h"
#include "py32f0xx_hal.h"
#include "py32f0xx_hal_conf.h"

#define ok() printf("ok\n")
#define _dbg() printf("%d %s\n", __LINE__, __FILE__)
#define print_log() printf("%d %s\n", __LINE__, __FILE__)
#define Error_Handler()  do {_dbg(); while(1);} while(0);

void _delay_us(volatile int n);
void millis_tick(void);
uint32_t millis(void);

#endif
