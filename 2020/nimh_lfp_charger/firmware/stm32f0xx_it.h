#ifndef __STM32F0XX_IT_H
#define __STM32F0XX_IT_H

#include "stm32f0xx.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void ADC1_COMP_IRQHandler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);

#endif
