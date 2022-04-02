#include "misc.h"
#include "platform.h"

extern USB_OTG_CORE_HANDLE g_udev;

void SysTick_Handler(void)
{
    _delay_nms_mm();
    zt_tick();
}

void SDIO_IRQHandler(void)
{
    SD_ProcessIRQSrc();
}

void SD_SDIO_DMA_IRQHANDLER(void)
{
    SD_ProcessDMAIRQ();
}

void DMA1_Stream4_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_Stream4, DMA_IT_TCIF4) == SET) {
        zplay_isr_tc();
        DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_TCIF4);
    }
    if(DMA_GetITStatus(DMA1_Stream4, DMA_IT_HTIF4) == SET) {
        zplay_isr_ht();
        DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_HTIF4);
    }
}

void OTG_FS_WKUP_IRQHandler(void)
{
    if(g_udev.cfg.low_power) {
        SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk
            | SCB_SCR_SLEEPONEXIT_Msk));
        RCC_HSEConfig(RCC_HSE_ON);
        while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
        RCC_PLLCmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetSYSCLKSource() != 0x08);
        USB_OTG_UngateClock(&g_udev);
    }
    EXTI_ClearITPendingBit(EXTI_Line18);
}

void OTG_FS_IRQHandler(void)
{
    USBD_OTG_ISR_Handler(&g_udev);
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

void NMI_Handler(void)
{
}
void HardFault_Handler(void)
{
    while(1);
}
void MemManage_Handler(void)
{
    while(1);
}
void BusFault_Handler(void)
{
    while(1);
}
void UsageFault_Handler(void)
{
    while(1);
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
