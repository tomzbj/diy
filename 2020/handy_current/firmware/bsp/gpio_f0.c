#include "misc.h"
#include "gpio_f0.h"

void GPIO_Config(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_InitTypeDef gis;

    // PB6-7 for USART1
    gis.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);

    // PA9~12, PA15 as in for keys
    gis.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
        | GPIO_Pin_15;
    gis.GPIO_Mode = GPIO_Mode_IN;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gis);

    // PB0~2 as out for LCD CS/A0/RES
    gis.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);

    // PB13&15 as SPI2 for LCD
    gis.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0);

    // PB11 as pwm for LCD
    gis.GPIO_Pin = GPIO_Pin_11;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &gis);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_2);

    // PA4&5 for DAC
    gis.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    gis.GPIO_Mode = GPIO_Mode_AN;
    gis.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &gis);

    gis.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &gis);
}

void GPIO_SetPB0(int state)
{
    if(state)
        GPIOB->BSRR = GPIO_Pin_0;
    else
        GPIOB->BRR = GPIO_Pin_0;
}

void GPIO_SetPB1(int state)
{
    if(state)
        GPIOB->BSRR = GPIO_Pin_1;
    else
        GPIOB->BRR = GPIO_Pin_1;
}

void GPIO_SetPB2(int state)
{
    if(state)
        GPIOB->BSRR = GPIO_Pin_2;
    else
        GPIOB->BRR = GPIO_Pin_2;
}
