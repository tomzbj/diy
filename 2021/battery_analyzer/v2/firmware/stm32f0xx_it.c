#include "platform.h"
#include "misc.h"

void SysTick_Handler(void)
{
    _delay_nms_mm();            // executes every 1ms
    zt_tick();
    if(0){
        static int count;
        count++;
        if(count%1000==0) {
            printf("1s\n");
        }
    }
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
    if(USART_GetITStatus(USART1, USART_IT_TC) != RESET) {
        zu_tc_irqhandler(0);
        USART_ClearITPendingBit(USART1, USART_IT_TC);
    }
    if(USART_GetITStatus(USART1, USART_IT_ORE) != RESET)
        USART_ClearITPendingBit(USART1, USART_IT_ORE);
    if(USART_GetITStatus(USART1, USART_IT_FE) != RESET)
        USART_ClearITPendingBit(USART1, USART_IT_FE);
}

void DMA1_Channel1_IRQHandler(void)
{
    ADCDMA_Conv_Complete_IRQHandler();
    DMA_ClearITPendingBit(DMA1_IT_GL1);
}

void HardFault_Handler(void)
{
    while(1);
}
