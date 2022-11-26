#include "misc.h"

void GPIO_Config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);

    unsigned short pins;// = GPIO_PIN_2 | GPIO_PIN_3;

    /*
     // PA9-10 for USART0
     gpio_af_set(GPIOA, GPIO_AF_1, pins);
     gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, pins);
     gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pins);
     */

    // PA8-10 as zi2c for OLED
    pins = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, pins);
//    pins = GPIO_PIN_4;
//    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pins);
    gpio_bit_set(GPIOA, pins);

    /*
     // PA5-6 as input for keys
     pins = GPIO_PIN_5 | GPIO_PIN_6;
     gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, pins);
     gpio_output_options_set(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_10MHZ, pins);
     gpio_bit_set(GPIOA, pins);
     */

    // PA0-2 as adc for current
    pins = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, pins);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, pins);

    /*
     // PB1 for PWM
     pins = GPIO_PIN_1;
     gpio_af_set(GPIOB, GPIO_AF_0, pins);
     gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, pins);
     gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pins);

     // PB8 for LED
     pins = GPIO_PIN_8;
     gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pins);
     gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, pins);
     */
}
