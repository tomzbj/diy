#include "platform.h"
#include "misc.h"

void SysTick_Handler(void)
{
    _delay_nms_mm();            // executes every 1ms
    zt_tick();
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        zu_rxne_irqhandler(0);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
        zu_idle_irqhandler(0);
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    }
    if(USART_GetITStatus(USART1, USART_IT_ORE) != RESET)
        USART_ClearITPendingBit(USART1, USART_IT_ORE);
    if(USART_GetITStatus(USART1, USART_IT_FE) != RESET)
        USART_ClearITPendingBit(USART1, USART_IT_FE);
}

void HardFault_Handler(void)
{
    while(1);
}
