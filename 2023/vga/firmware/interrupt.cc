#include "misc.h"
#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

void SysTick_Handler(void)
{
    _delay_nms_mm();            // executes every 1ms
    millis_tick();
    zt.tick();
}

static void usart_clear_idle(void)
{
    volatile unsigned long tmp = 0;
    tmp = USART_STAT0(USART0);
    tmp = USART_DATA(USART0);
    tmp = tmp;
}

void TIMER2_IRQHandler(void)
{
    __disable_irq();
    if(SET == timer_interrupt_flag_get(TIMER2, TIMER_INT_CH1)) {
        VGA_Handler();
        timer_interrupt_flag_clear(TIMER2, TIMER_INT_CH1);
    }
    __enable_irq();
}

void DMA1_Channel2_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA1, DMA_CH2, DMA_INT_FLAG_FTF)) {
        if(0) {
            static int count = 0;
            ++count %= 1000;
            if(count == 0)
                printf("#\n");
        }
        dma_interrupt_flag_clear(DMA1, DMA_CH2, DMA_INT_FLAG_G);
    }
}

void USART0_IRQHandler(void)
{
    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) != RESET) {
        u0.idle_irqhandler();
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);
        usart_clear_idle();
    }
    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE) != RESET) {
        u0.rxne_irqhandler();
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
    }
    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_ERR_NERR) != RESET)
        usart_clear_idle();
    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_ERR_ORERR) != RESET)
        usart_clear_idle();
    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_ERR_FERR) != RESET)
        usart_clear_idle();
}

void HardFault_Handler(void)
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
void MemManage_Handler(void)
{
    while(1);
}

#ifdef __cplusplus
}
#endif
