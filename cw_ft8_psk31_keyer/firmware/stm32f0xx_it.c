//#include <stdio.h>
#include "platform.h"

void SysTick_Handler(void)
{
    _delay_nms_mm();            // executes every 1ms
    zt_tick();
    DataConfig_t* pdc = DC_Get();
    pdc->data.ms++;
    pdc->data.ms %= (86400UL * 1000UL);
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        USART_RXNE_IRQHandler(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
        USART_RX_IDLE_IRQHandler(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    }
    if(USART_GetITStatus(USART1, USART_IT_ORE) != RESET) {
        USART_ClearITPendingBit(USART1, USART_IT_ORE);
    }
    if(USART_GetITStatus(USART1, USART_IT_FE) != RESET) {
        USART_ClearITPendingBit(USART1, USART_IT_FE);
    }
}

void TIM7_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) {
        bpsk31_poll();
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    }
}

void TIM17_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM17, TIM_IT_Update) != RESET) {
        ft8_poll();
        TIM_ClearITPendingBit(TIM17, TIM_IT_Update);
    }
}

void PPP_IRQHandler(void) { while(1); }
void NMI_Handler(void) { while(1); }
void HardFault_Handler(void) { while(1); }
void SVC_Handler(void) { while(1); }
void PendSV_Handler(void) { while(1); }
void USB_IRQHandler(void) { USB_Istr(); }
void EXTI4_15_IRQHandler(void) { }
