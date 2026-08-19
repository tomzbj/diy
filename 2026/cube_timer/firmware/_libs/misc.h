#ifndef _MISC_H
#define _MISC_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f1xx.h"
#include "stm32f1xx_hal_conf.h"

/* Release build: both 0. Debug over UART: set both to 1 and rebuild. */
#ifndef APP_LOG_ENABLE
#define APP_LOG_ENABLE 1
#endif
#ifndef APP_USART_ENABLE
#define APP_USART_ENABLE 1
#endif

#define Error_Handler() while(1)

#if APP_LOG_ENABLE
#define app_log(...) printf(__VA_ARGS__)
#define _dbg() printf("%d %s\n", __LINE__, __FILE__)
#define print_log() \
  printf("[%-10lu %-20s %3d]\n", DWT->CYCCNT, __FILE__, __LINE__)
#else
#define app_log(...) do { } while(0)
#define _dbg() do { } while(0)
#define print_log() do { } while(0)
/* Silence leftover printf/puts call sites without editing every line. */
#undef printf
#define printf(...) ((void)0)
#undef puts
#define puts(...) ((void)0)
#endif

#define test_if(x) do { if((x) != HAL_OK) while(1); } while(0)

void _delay_us(volatile int n);
uint32_t millis(void);
void millis_tick(void);

static inline int saturate(int val, int min, int max)
{
  val = (val < min) ? min : val;
  val = (val > max) ? max : val;
  return val;
}

void DWT_ON(void);

#ifdef __cplusplus
extern "C" {
#endif
void SystemClock_Config(void);
#ifdef __cplusplus
}
#endif

#endif
