#include "misc.h"
#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

void SysTick_Handler(void)
{
    _delay_nms_mm();            // executes every 1ms
    zt.tick();
}

void USART0_IRQHandler(void)
{
    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) != RESET) {
        u0.idle_irqhandler();
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);
    }
    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE) != RESET) {
        u0.rxne_irqhandler();
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
    }
}

void HardFault_Handler(void)
{
    while(1);
}

#ifdef __cplusplus
}
#endif
