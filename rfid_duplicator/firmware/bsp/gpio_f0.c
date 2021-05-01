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

    // PB1 for RC522 CS
    gis.GPIO_Pin = GPIO_Pin_1;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &gis);

    // PA5~7 as SPI for RC522
    gis.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gis);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);

    // PA2~4 as i2c for oled
    gis.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_OD;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gis);

    // PF0&1 as key
    gis.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    gis.GPIO_Mode = GPIO_Mode_IN;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_50MHz;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOF, &gis);
}
