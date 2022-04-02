#include "misc.h"
#include "platform.h"

static void setsda_1(void) { GPIOA->BSRR = GPIO_Pin_5;}
static void setsda_0(void) { GPIOA->BRR = GPIO_Pin_5;}
static void setscl_1(void) { GPIOA->BSRR = GPIO_Pin_6;}
static void setscl_0(void) { GPIOA->BRR = GPIO_Pin_6;}

static int getsda(void)
{
//    _delay_us(1);
    return (GPIOA->IDR & GPIO_Pin_5);
}

// si5351: 1M~150M
void VFO_SetFreqFast(int freq)
{
    SI5351_SetFreq(freq);
}
void VFO_SetFreq(int freq)
{
    int a, b, c;
    a = 900000000UL / freq;
    //    b = ((900e6 / freq) - a) * 1000000UL;
    //    b = (900000000ULL * 1000000ULL / freq) % 1000000UL;
    b = ((900000000ULL << 19) / freq) & 0x7ffff;
    //    c = 1000000UL;
    c = 0x80000;
    //    printf("%d %d %d %d\n", freq, a, b, c);
    SI5351_SetMSParams(SI5351_MS2, SI5351_PLLA, a, b, c);
}

void VFO_Config(void)
{
    static zi2c_t i2c;
    i2c.setscl_1_f = setscl_1;
    i2c.setscl_0_f = setscl_0;
    i2c.setsda_1_f = setsda_1;
    i2c.setsda_0_f = setsda_0;
    i2c.getsda_f = getsda;
    i2c.addr = 0x60;
    zi2c_init(&i2c);
    SI5351_Init(&i2c);

    SI5351_SetPLL(SI5351_PLLA, 36, 0, 1);    // 25M * 36 = 900M
    SI5351_SetPLL(SI5351_PLLB, 36, 0, 1);    // 25M * 36 = 900M
    SI5351_SetMSParams(SI5351_MS0, SI5351_PLLA, 9, 0, 1);    // 900M / 9 = 100M
    SI5351_SetMSParams(SI5351_MS2, SI5351_PLLB, 12, 0, 1);    // 900M / 12 = 75M
    SI5351_SetRDiv(SI5351_MS0, SI5351_RDIV_DIV1);
    SI5351_SetRDiv(SI5351_MS2, SI5351_RDIV_DIV1);
}
