#include "misc.h"

void GPIO_Config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);

    unsigned short pins;

    // PB6-7 for USART0
    pins = GPIO_PIN_6 | GPIO_PIN_7;
    gpio_af_set(GPIOB, GPIO_AF_0, pins);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pins);

    // PA8 as ckout for rf-out
    pins = GPIO_PIN_8;
    gpio_af_set(GPIOA, GPIO_AF_0, pins);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, pins);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, pins);
//    rcu_ckout_config(RCU_CKOUTSRC_CKSYS, RCU_CKOUT_DIV1);
    rcu_ckout_config(RCU_CKOUTSRC_HXTAL, RCU_CKOUT_DIV1);

    // PA4 as T13_C0 for FM
    pins = GPIO_PIN_4;
//    gpio_af_set(GPIOA, GPIO_AF_4, pins);
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, pins);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, pins);
}
