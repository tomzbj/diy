#include "misc.h"
#include "platform.h"

void USART_WriteByte(unsigned char c, int source)
{
    while(usart_flag_get(USART0, USART_FLAG_TBE) == RESET);
    USART_TDATA (USART0) = c;
    while(usart_flag_get(USART0, USART_FLAG_TC) == RESET);
}

unsigned char USART_ReadByte(int source)
{
    return USART_RDATA(USART0);
}

void USART_Config(void)
{
    rcu_periph_clock_enable(RCU_USART0);

    // USART0 for USB
    usart_deinit(USART0);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_baudrate_set(USART0, 500000UL);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
    usart_interrupt_enable(USART0, USART_INT_IDLE);
    usart_interrupt_enable(USART0, USART_INT_ERR);
    usart_interrupt_enable(USART0, USART_INT_RBNE);
    nvic_irq_enable(USART0_IRQn, 0, 0);

    printf("\n");
    print_log();
}
