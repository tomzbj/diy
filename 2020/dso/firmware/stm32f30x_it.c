#include "misc.h"
#include "platform.h"

void SysTick_Handler(void)
{
    _delay_nms_mm();
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

/*
void DMA2_Channel1_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_IT_TC1) != RESET) {
        ADCDMA_IRQHandler(DMA_IT_TC);
        DMA_ClearITPendingBit(DMA2_IT_TC1);
    }
    if(DMA_GetITStatus(DMA2_IT_HT1) != RESET) {
        ADCDMA_IRQHandler(DMA_IT_HT);
        DMA_ClearITPendingBit(DMA2_IT_HT1);
    }
}

void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line5) != RESET) {
        DataConfig_t* pdc = DC_Get();
        if(pdc->state == STATE_STANDBY && pdc->config.trig_mode == TRIG_MODE_EXTERNAL) {
            ADCDMA_StartTransmission();
        }
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}

*/
