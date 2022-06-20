#include "misc.h"
#include "platform.h"

void SysTick_Handler(void)
{
    _delay_nms_mm();            // executes every 1ms
    zt_tick();
}

void DMA_Channel1_2_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA_CH2, DMA_INT_FLAG_FTF))
        DDS_DMA_FTF();
    if(dma_interrupt_flag_get(DMA_CH2, DMA_INT_FLAG_HTF))
        DDS_DMA_HTF();
    dma_interrupt_flag_clear(DMA_CH2, DMA_INT_FLAG_G);
}

void USART0_IRQHandler(void)
{
    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE) != RESET) {
        zu_rxne_irqhandler(0);
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
    }
    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) != RESET) {
        zu_idle_irqhandler(0);
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);
    }
    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_ERR_FERR) != RESET)
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_ERR_FERR);
    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_ERR_ORERR) != RESET)
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_ERR_ORERR);
}

void HardFault_Handler(void)
{
    while(1);
}
