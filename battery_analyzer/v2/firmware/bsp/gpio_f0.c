#include "misc.h"

void GPIO_Config(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_InitTypeDef gis;
    GPIO_StructInit(&gis);

    // PA9 & PA10 as USART1
    gis.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gis);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    // PB0~2 as i2c for oled
    gis.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_OD;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gis);

    // PA0,1,2,5 as adc, PA4 as dac
    gis.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5;
    gis.GPIO_Mode = GPIO_Mode_AN;
    gis.GPIO_OType = GPIO_OType_OD;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gis);
    gis.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOA, &gis);

    // PA11~12 as input for keys
    gis.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    gis.GPIO_Mode = GPIO_Mode_IN;
    gis.GPIO_OType = GPIO_OType_OD;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gis);
}
