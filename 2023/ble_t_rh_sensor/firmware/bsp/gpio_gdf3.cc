#include "misc.h"

void GPIO_Config(void)
{
    rcu_periph_clock_enable (RCU_GPIOA);
    rcu_periph_clock_enable (RCU_GPIOB);

    // PA9 & 10 for USART0
    unsigned short pins = GPIO_PIN_9 | GPIO_PIN_10;
    gpio_af_set(GPIOA, GPIO_AF_1, pins);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pins);

    // PA2 for LED
    pins = GPIO_PIN_2;
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pins);
    GPIO_BOP (GPIOA) = GPIO_PIN_2;

    // PB8&PB9 as SCL&SDA
    pins = GPIO_PIN_9 | GPIO_PIN_8;
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, pins);

    // PB3-5 as SPI0 for NRF24L01
    pins = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    gpio_af_set(GPIOB, GPIO_AF_0, pins);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pins);

    // PB6&7 as CE&CS for NRF24L01
    pins = GPIO_PIN_6 | GPIO_PIN_7;
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pins);
}
