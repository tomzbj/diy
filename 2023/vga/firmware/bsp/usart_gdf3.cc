#include "platform.h"
#include "misc.h"

void USART0_WriteByte(unsigned char c)
{
    usart_data_transmit(USART0, c);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
}

unsigned char USART0_ReadByte(void)
{
    return USART_DATA(USART0);
}

void USART_Config(void)
{
    rcu_periph_clock_enable (RCU_GPIOA);
    // PA9&10 as USART0
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    rcu_periph_clock_enable (RCU_USART0); /* enable USART clock */
    rcu_periph_clock_enable (RCU_AF);
    __disable_irq();
    usart_disable (USART0);
    usart_deinit(USART0); /* USART configure */
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_baudrate_set(USART0, 500000UL);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
    usart_interrupt_enable(USART0, USART_INT_ERR);
    usart_interrupt_enable(USART0, USART_INT_RBNE);
    usart_interrupt_enable(USART0, USART_INT_IDLE);
//    usart_interrupt_enable(USART0, USART_INT_TC);
    nvic_irq_enable(USART0_IRQn, 0, 2);
    __enable_irq();

    printf("\n\n");
    print_log();
}
