#include "misc.h"

void GPIO_Config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);

    unsigned short pins = GPIO_PIN_9 | GPIO_PIN_10;

    // PA9-10 for USART0
    gpio_af_set(GPIOA, GPIO_AF_1, pins);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pins);

    // PB5-7 as zi2c for OLED
    pins = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_10MHZ, pins);
    gpio_bit_set(GPIOB, pins);

    // PB0-1 as input for keys
    pins = GPIO_PIN_0 | GPIO_PIN_1;
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_10MHZ, pins);
    gpio_bit_set(GPIOB, pins);

    // PA0-7 as output
    pins = 0xff;
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pins);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, pins);
    gpio_bit_reset(GPIOA, pins);
}
