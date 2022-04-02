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

void TIM16_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET) {
        DataConfig_t* pdc = DC_Get();
        VFO_SetFreqFast(pdc->data.freq * 1000UL + ADC1->DR - 2048);
        TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
    }
}

void HardFault_Handler(void)
{
    while(1);
}
