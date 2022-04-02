#include "misc.h"
#include "platform.h"

#define usart_clearflags() do { volatile int tmp=tmp;tmp=USART1->SR; tmp=USART1->DR;}while(0)

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
        zu_idle_irqhandler(0);
        usart_clearflags();
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    }
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        zu_rxne_irqhandler(0);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
    if(USART_GetITStatus(USART1, USART_IT_ORE) != RESET)
        usart_clearflags();
    if(USART_GetITStatus(USART1, USART_IT_FE) != RESET)
        usart_clearflags();
}

void DMA1_Channel1_IRQHandler(void)
{
    ADCDMA_Conv_Complete_IRQHandler();
    DMA_ClearITPendingBit(DMA1_IT_GL1);
}

void SysTick_Handler(void)
{
    _delay_nms_mm();            // executes every 1ms
    zt_tick();
}

void HardFault_Handler(void)
{
    while(1);
}
