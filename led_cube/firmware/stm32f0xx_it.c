#include "platform.h"

void SysTick_Handler(void)
{
    _delay_nms_mm();            // executes every 1ms
    zt_tick();
}

void USART1_IRQHandler(void)
{
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

void USB_IRQHandler(void)
{
    USB_Istr();
}

void PPP_IRQHandler(void) { while(1); }
void NMI_Handler(void) { while(1); }
void HardFault_Handler(void) { while(1); }
void SVC_Handler(void) { while(1); }
void PendSV_Handler(void) { while(1); }
