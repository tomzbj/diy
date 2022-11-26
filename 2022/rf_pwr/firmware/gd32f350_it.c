#include "misc.h"
#include "platform.h"

void SysTick_Handler(void)
{
    _delay_nms_mm();            // executes every 1ms
    zt_tick();
}

void DMA_Channel0_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA_CH0, DMA_INT_FLAG_FTF)) {
        ADCDMA_IRQHandler();
        dma_interrupt_flag_clear(DMA_CH0, DMA_INT_FLAG_G);
    }
}

void HardFault_Handler(void)
{
    while(1);
}
