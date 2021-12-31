#include "misc.h"
#include "platform.h"

void SysTick_Handler(void)
{
    _delay_nms_mm();
    zt_tick();
    extern int OS_TimeMS;
    OS_TimeMS++;
    DataConfig_t* pdc = DC_Get();
    pdc->data.ms++;
    pdc->data.ms %= (86400UL * 1000UL);
}

void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) {
        zu_idle_irqhandler(0);
        USART_ClearITPendingBit(USART2, USART_IT_IDLE);
    }
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        zu_rxne_irqhandler(0);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
    if(USART_GetITStatus(USART2, USART_IT_ORE) != RESET)
        USART_ClearITPendingBit(USART2, USART_IT_ORE);
    if(USART_GetITStatus(USART2, USART_IT_FE) != RESET)
        USART_ClearITPendingBit(USART2, USART_IT_FE);
}

void TIM7_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) {
        bpsk31_poll();
        static int count = 0;
        count++;
        count %= 1280;
        if(count == 0)
            ft8_poll();
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    }
}
