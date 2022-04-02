#include "misc.h"

void GPIO_Config(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    GPIO_InitTypeDef gis;
    GPIO_StructInit(&gis);

    // PA2 & PA3 as USART2
    gis.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gis);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

    // PB13~15 as spi2 for w25
    gis.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gis);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0);

    // PB12 as cs for w25
    gis.GPIO_Pin = GPIO_Pin_12;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gis);

    // PC13~15 for fm1906
    gis.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &gis);

    // PB9 as vlcd for fm1906
    gis.GPIO_Pin = GPIO_Pin_9;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gis);
    GPIOB->BSRR = GPIO_Pin_9;

    // PB4 as T3_C1 for audio out
    gis.GPIO_Pin = GPIO_Pin_4;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gis);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_1);

    // PB5 as NS4150_EN
    gis.GPIO_Pin = GPIO_Pin_5;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gis);

    // PA5~6 for keys
    gis.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
    gis.GPIO_Mode = GPIO_Mode_IN;
    gis.GPIO_OType = GPIO_OType_OD;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gis);
}
