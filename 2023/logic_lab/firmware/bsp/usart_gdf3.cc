#include "platform.h"
#include "misc.h"

void USART0_WriteByte(unsigned char c)
{
    usart_data_transmit(USART0, c);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
}

unsigned char USART0_ReadByte(void)
{
    return USART_RDATA(USART0);
}

static void _usart_config(unsigned long usartx, unsigned long baudrate)
{
    usart_disable(usartx);
    usart_deinit(usartx); /* USART configure */
    usart_word_length_set(usartx, USART_WL_8BIT);
    usart_stop_bit_set(usartx, USART_STB_1BIT);
    usart_parity_config(usartx, USART_PM_NONE);
    usart_baudrate_set(usartx, baudrate);
    usart_receive_config(usartx, USART_RECEIVE_ENABLE);
    usart_transmit_config(usartx, USART_TRANSMIT_ENABLE);
    usart_enable(usartx);
    usart_interrupt_enable(usartx, USART_INT_ERR);
    usart_interrupt_enable(usartx, USART_INT_RBNE);
    usart_interrupt_enable(usartx, USART_INT_IDLE);
}

void USART_Config(void)
{
    rcu_periph_clock_enable (RCU_USART0); /* enable USART clock */
//    rcu_periph_clock_enable (RCU_USART1); /* enable USART clock */

    __disable_irq();
    _usart_config(USART0, 500000UL);
//    _usart_config(USART1, 9600UL);
    nvic_irq_enable(USART0_IRQn, 1);
//    nvic_irq_enable(USART1_IRQn, 1);
    __enable_irq();

    printf("\n\n");
    printf("[%s: %d] USART Initialized.\n", __FILE__, __LINE__);
}
