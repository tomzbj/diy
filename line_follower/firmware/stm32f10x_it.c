#include "misc.h"
#include "platform.h"

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
        USART_RX_IDLE_IRQHandler(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    }
}

void DMA1_Channel1_IRQHandler(void)
{
    ADCDMA_Conv_Complete_IRQHandler();
    DMA_ClearITPendingBit(DMA1_IT_GL1);
}

void SysTick_Handler(void)
{
    _delay_nms_mm();    // executes every 1ms
    zt_tick();
}

void HardFault_Handler(void)
{
    while(1);
}
