#ifndef _MISC_H
#define _MISC_H

#include <inttypes.h>

//#include "stm32f0xx.h"
#include "stm32f10x.h"

#define sbi(x, y) (x)->BSRR = (y)
#define cbi(x, y) (x)->BRR = (y)
#define _BV(x) (1<<(x))
#define bit_is_set(x, y) (x)->IDR & (y)

#define _dbg() uprintf(USART1, "%d %s\n", __LINE__, __FILE__)
//#define _dbg() ({})

extern GPIO_InitTypeDef    GPIO_InitStructure;
extern SPI_InitTypeDef     SPI_InitStructure;
extern DMA_InitTypeDef     DMA_InitStructure;
extern NVIC_InitTypeDef    NVIC_InitStructure;
extern EXTI_InitTypeDef    EXTI_InitStructure;
extern USART_InitTypeDef   USART_InitStructure;

extern u8 ByteReverse(u8 byte);
extern void _delay_us(vu32 n);
extern void _delay_ms_loop(vu32 n);
extern void _delay_ms_systick(vu32 n);
extern void _delay_nms_mm(void);

#define _delay_ms _delay_ms_systick

typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;
typedef const int32_t sc32;     /*!< Read Only */
typedef const int16_t sc16;     /*!< Read Only */
typedef const int8_t sc8;       /*!< Read Only */
typedef volatile int32_t vs32;
typedef volatile int16_t vs16;
typedef volatile int8_t vs8;
typedef volatile const int32_t vsc32;   /*!< Read Only */
typedef volatile const int16_t vsc16;   /*!< Read Only */
typedef volatile const int8_t vsc8; /*!< Read Only */
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef const uint32_t uc32;    /*!< Read Only */
typedef const uint16_t uc16;    /*!< Read Only */
typedef const uint8_t uc8;      /*!< Read Only */
typedef volatile uint32_t vu32;
typedef volatile uint16_t vu16;
typedef volatile uint8_t vu8;
typedef volatile const uint32_t vuc32;  /*!< Read Only */
typedef volatile const uint16_t vuc16;  /*!< Read Only */
typedef volatile const uint8_t vuc8;    /*!< Read Only */

typedef int64_t s64;
typedef const int64_t sc64;
typedef volatile int64_t vs64;
typedef volatile const int64_t vsc64;
typedef uint64_t u64;
typedef const uint64_t uc64;
typedef volatile uint64_t vu64;
typedef volatile const uint64_t vuc64;

#endif
