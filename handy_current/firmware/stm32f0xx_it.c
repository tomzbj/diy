#include <stdio.h>
#include "stm32f0xx_it.h"
#include "usart_f0.h"
#include "disp.h"
#include "key.h"

static unsigned int g_usart_idle_count = 0;

void SysTick_Handler(void)
{
    static int count = 0;
    _delay_nms_mm();            // executes every 1ms
    count++;
    g_usart_idle_count++;
    if(count % 5 == 0) {
        KEY_Read();
    }
    if(count % 25 == 0 && g_usart_idle_count > 500) {
        DISP_SetUpdate();
    }
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
        USART_RX_IDLE_IRQHandler(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    }
}

void EXTI4_15_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line14) != RESET) {
//        ADC_Conversion_Complete_IRQHandler();
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
}

void PPP_IRQHandler(void) { while (1) { } } 
void NMI_Handler(void) { while (1) { } } 
void HardFault_Handler(void) { while (1) { } } 
void SVC_Handler(void) { while (1) { } } 
void PendSV_Handler(void) { while (1) { } }
