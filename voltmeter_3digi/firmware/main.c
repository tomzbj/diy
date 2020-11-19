#include "misc.h"
#include "platform.h"

typedef struct {
    GPIO_TypeDef* gpiox;
    GPIO_Pin_TypeDef pin;
} gpio_t;

const gpio_t segments[8] = {
//
    {GPIOD, GPIO_PIN_4},    //
    {GPIOC, GPIO_PIN_4},    //
    {GPIOC, GPIO_PIN_7},    //
    {GPIOD, GPIO_PIN_2},    //
    {GPIOD, GPIO_PIN_3},    //
    {GPIOD, GPIO_PIN_5},    //
    {GPIOC, GPIO_PIN_6},    //
    {GPIOC, GPIO_PIN_5}    //
};

const gpio_t digits[3] = {
//
    {GPIOA, GPIO_PIN_3},    //
    {GPIOB, GPIO_PIN_5},    //
    {GPIOB, GPIO_PIN_4}    //
};

void set_digit(unsigned int digit, int state)
{
    if(digit < 3) {
        if(state)
            digits[digit].gpiox->ODR |= digits[digit].pin;
        else
            digits[digit].gpiox->ODR &= ~(digits[digit].pin);
    }
}

void set_segment(unsigned int segment, int state)
{
    if(segment < 8) {
        if(state)
            segments[segment].gpiox->ODR |= segments[segment].pin;
        else
            segments[segment].gpiox->ODR &= ~(segments[segment].pin);
    }
}

void zss_n(void)
{
    static int count = 0, count2 = 0;
    count %= 1000;
    count2 %= 3;
    zss_set_num(count);
    zss_set_dot(count2);
    count++;
    count2++;
}

void zss_n_raw(void)
{
    static int count = 0;
    const unsigned char* raw[] = {
    //
        "\xff\xff\xff",// 888
        "\x74\x5c\x78",    // hot
        "\x77\x7c\x39",    // abc
        "\x79\x50\x50"    // err
        };
    count %= sizeof(raw) / sizeof(raw[0]);
    zss_set_raw(raw[count]);
    count++;
}

void adc(void)
{
    DataConfig_t* pdc = DC_Get();
    unsigned long n = pdc->data.data * 3300UL / 1023UL / 5;
    int dot_pos = 0;
    if(n < 1000) {
    }
    else {
        dot_pos = 3;
        while(n >= 1000) {
            n /= 10;
            dot_pos--;
        }
    }
    zss_set_num(n);
    zss_set_dot(dot_pos);
}

void init(void)
{
    disableInterrupts();
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
    DC_Config();
    GPIO_Config();
    TIMER_Config();
    ADC_Config();
    enableInterrupts();

    {
        static zss_cfg_t cfg;
        cfg.set_digit_f = set_digit;
        cfg.set_segment_f = set_segment;
        cfg.digits = 3;
        cfg.segments = 8;
        zss_init(&cfg);
    }

    zt_bind(zss_update, 1, 1);
    zt_bind(adc, 500, 1);
//    zt_bind(zss_n, 500, 1);
//    zt_bind(zss_n_raw, 500, 0);
}

int main(void)
{
    init();

    while(1) {
        zt_poll();
        ADC_Poll();
    }
}
