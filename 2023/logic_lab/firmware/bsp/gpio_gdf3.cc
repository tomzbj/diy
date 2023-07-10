#include "misc.h"

void GPIO_Config(void)
{
    rcu_periph_clock_enable (RCU_GPIOA);
    rcu_periph_clock_enable (RCU_GPIOB);
    rcu_periph_clock_enable (RCU_GPIOC);
    rcu_periph_clock_enable (RCU_GPIOF);

    // PA9 & 10 for USART0
    unsigned short pins = GPIO_PIN_9 | GPIO_PIN_10;
    gpio_af_set(GPIOA, GPIO_AF_1, pins);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, pins);

    // PF0&7, PC13, PA3,6,7,11, PB2,4,7,12,15 as output (11 in total)
    pins = GPIO_PIN_0 | GPIO_PIN_7;
    gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, pins);
    pins = GPIO_PIN_13;
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, pins);
    pins = GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11;
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, pins);
    pins = GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_7 | GPIO_PIN_12 | GPIO_PIN_15;
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, pins);

    // PF1, 6, PA0, 1, 2, 4, 5, 8, 12, 15, PB8, 9, 10, 11, 0, 1, 5, 3, 13, 14,
    // PC14, 15 as input (20 in total)
    pins = GPIO_PIN_1 | GPIO_PIN_6;
    gpio_mode_set(GPIOF, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, pins);
    pins = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_12
        | GPIO_PIN_15 | GPIO_PIN_8;
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, pins);
    pins = GPIO_PIN_0 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
        | GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_3 | GPIO_PIN_1;
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, pins);
    pins = GPIO_PIN_14 | GPIO_PIN_15;
    gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, pins);
}
