#include "misc.h"

void GPIO_Config(void)
{
    GPIO_InitTypeDef gis;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    // PB10~11 for USART3
    gis.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_7);    //GPIOA9复用为USART1
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_7);    //GPIOA10复用为USART1

    // PB1 for vdd en, PB11 for hv en, PB8 for lcd backlight
    gis.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_11;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);

    GPIOB->BSRR = GPIO_Pin_8;
    GPIOB->BRR = GPIO_Pin_1 | GPIO_Pin_11;

    _delay_us(1000);
    GPIOB->BSRR = GPIO_Pin_11;    // hv en
    _delay_us(1000);
    GPIOB->BSRR = GPIO_Pin_1;    // vdd en
    _delay_us(1000);
    GPIOB->BRR = GPIO_Pin_8;    // lcd backlight en

    // PA11, 12, 15 as cs, reset, rs for lcd
    gis.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gis);

    // PB3~5 as spi3 for lcd
    gis.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_6);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_6);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_6);

    // PB13-15 as spi2 for spiflash
    gis.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_15;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_5);

    // PB0 as T3_C3 for test signal
    gis.GPIO_Pin = GPIO_Pin_0;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_2);

    // PA0 as AN for ADC
    gis.GPIO_Pin = GPIO_Pin_0;
    gis.GPIO_Mode = GPIO_Mode_AN;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &gis);

    // PA5 & PA7 as spi1 for HC595
    gis.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gis);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_5);    // AF5 for spi1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_5);    // AF5 for spi1
    // PA6 as cs for HC595
    gis.GPIO_Pin = GPIO_Pin_6;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gis);
    GPIOA->BSRR = GPIO_Pin_6;

    // PA9 & PA10 as in for keys
    gis.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    gis.GPIO_Mode = GPIO_Mode_IN;
    gis.GPIO_Speed = GPIO_Speed_2MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gis);

    gis.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    gis.GPIO_Mode = GPIO_Mode_AN;
    gis.GPIO_Speed = GPIO_Speed_2MHz;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gis);
}
