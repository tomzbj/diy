#include "misc.h"
#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

void SysTick_Handler(void)
{
    _delay_nms_mm();            // executes every 1ms
//    zt.tick();
    os_tick();
    u1.tick();
    u2.tick();
    if(0) {
        static int count;
        ++count %= 1000;
        if(!count)
            printf("########### 1s\n");
    }
}

/*
void USART2_IRQHandler(void)
{
    if(USART2->ISR & UART_FLAG_RXNE) {
        USART2->ICR = UART_FLAG_RXNE;
        u2.rxne_irqhandler();
    }
    if(USART2->ISR & UART_FLAG_ORE)
        USART2->ICR = UART_FLAG_ORE;
    if(USART2->ISR & UART_FLAG_FE)
        USART2->ICR = UART_FLAG_FE;
    if(USART2->ISR & UART_FLAG_NE)
        USART2->ICR = UART_FLAG_NE;
    if(USART2->ISR & UART_FLAG_PE)
        USART2->ICR = UART_FLAG_PE;
}

void USART1_IRQHandler(void)
{
    if(USART1->ISR & UART_FLAG_RXNE) {
        USART1->ICR = UART_FLAG_RXNE;
        u1.rxne_irqhandler();
    }
    if(USART1->ISR & UART_FLAG_ORE)
        USART1->ICR = UART_FLAG_ORE;
    if(USART1->ISR & UART_FLAG_FE)
        USART1->ICR = UART_FLAG_FE;
    if(USART1->ISR & UART_FLAG_NE)
        USART1->ICR = UART_FLAG_NE;
    if(USART1->ISR & UART_FLAG_PE)
        USART1->ICR = UART_FLAG_PE;
}
*/

void HardFault_Handler(void)
{
    while(1);
}

#ifdef __cplusplus
}
#endif
