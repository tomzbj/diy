#include "misc.h"
#include "stm32f10x_it.h"
#include "adc_f1.h"

static unsigned long g_ms_counter;
unsigned long GetMsCounter(void)
{
    return g_ms_counter;
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
        USART_RX_IDLE_IRQHandler(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    }
}

// STM32内部ADC转换完成时把DMA_Buffer内容复制到ADC_Data数组, 调整增益
void DMA1_Channel1_IRQHandler(void)
{
    ADCDMA_Conv_Complete_IRQHandler();
    DMA_ClearITPendingBit(DMA1_IT_GL1);
}

void SysTick_Handler(void)
{
    _delay_nms_mm();        // executes every 1ms
    g_ms_counter++;
}

void PPP_IRQHandler(void)
{
}
void NMI_Handler(void)
{
}
void HardFault_Handler(void)
{
    ( {  while (1);});
}
void MemManage_Handler(void)
{
    ( {  while (1);});
}
void BusFault_Handler(void)
{
    ( {  while (1);});
}
void UsageFault_Handler(void)
{
    ( {  while (1);});
}
void SVC_Handler(void)
{
}
void DebugMon_Handler(void)
{
}
void PendSV_Handler(void)
{
}
