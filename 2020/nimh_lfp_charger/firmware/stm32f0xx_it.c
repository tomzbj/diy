#include <stdio.h>
#include "stm32f0xx_it.h"
#include "usart.h"
#include "adc_f0.h"
#include "led.h"

void SysTick_Handler(void)
{
    static int count = 0;
    _delay_nms_mm();            // executes every 1ms
    count %= 500;
    if(count == 0)
        LED_Show();
    count++;
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
        USART_RX_IDLE_IRQHandler(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    }
}

void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) {
        USART_RX_IDLE_IRQHandler(USART2);
        USART_ClearITPendingBit(USART2, USART_IT_IDLE);
    }
}

void EXTI4_15_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line14) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
}

void DMA1_Channel1_IRQHandler(void)
{
    ADCDMA_Conv_Complete_IRQHandler();
    DMA_ClearITPendingBit(DMA1_IT_GL1);
}

void PPP_IRQHandler(void) { while (1) { } } 
void NMI_Handler(void) { while (1) { } } 
void HardFault_Handler(void) { while (1) { } } 
void SVC_Handler(void) { while (1) { } } 
void PendSV_Handler(void) { while (1) { } }
