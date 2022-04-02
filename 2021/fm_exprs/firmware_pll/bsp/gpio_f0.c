#include "misc.h"

void GPIO_Config(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);

    GPIO_InitTypeDef gis;
    GPIO_StructInit(&gis);

    // PA9&PA10 for USART
    gis.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gis);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    // PA5~7 as i2c for oled
    gis.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_OD;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gis);

    // PA0&1 as key
    gis.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    gis.GPIO_Mode = GPIO_Mode_IN;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gis);

    // PA4 as T14_C1 for RF_BIAS
    gis.GPIO_Pin = GPIO_Pin_4;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gis);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_4);

    // PA3 as ADC
    gis.GPIO_Pin = GPIO_Pin_3;
    gis.GPIO_Mode = GPIO_Mode_AN;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gis);

}
