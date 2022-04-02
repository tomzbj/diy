#include "platform.h"

void SysTick_Handler(void)
{
    _delay_nms_mm();            // executes every 1ms
    zt_tick();
}

void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        zu_rxne_irqhandler(0);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//        _dbg();
    }
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) {
        zu_idle_irqhandler(0);
//        _dbg();
        USART_ClearITPendingBit(USART2, USART_IT_IDLE);
    }
//    if(USART_GetITStatus(USART2, USART_IT_TC) != RESET) { zu_tc_irqhandler(1); USART_ClearITPendingBit(USART2, USART_IT_TC); }
    if(USART_GetITStatus(USART2, USART_IT_ORE) != RESET)
        USART_ClearITPendingBit(USART2, USART_IT_ORE);
    if(USART_GetITStatus(USART2, USART_IT_FE) != RESET)
        USART_ClearITPendingBit(USART2, USART_IT_FE);
    if(USART_GetITStatus(USART2, USART_IT_NE) != RESET)
        USART_ClearITPendingBit(USART2, USART_IT_NE);
}

void TIM3_IRQHandler(void)
{
    static int count;
    count++;
    count %= 1000;
    if(count == 0)
        printf(".\n");
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

void HardFault_Handler(void)
{
    while(1);
}

void DMA1_Channel2_3_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC3)) {
//        ze_post(EVENT_AUDIO_TC);
        AUDIO_event_handler(EVENT_AUDIO_TC);
        DMA_ClearITPendingBit(DMA1_IT_TC3);
    }
    else if(DMA_GetITStatus(DMA1_IT_HT3)) {
        AUDIO_event_handler(EVENT_AUDIO_HT);
//        ze_post(EVENT_AUDIO_HT);
        DMA_ClearITPendingBit(DMA1_IT_HT3);
    }
}

